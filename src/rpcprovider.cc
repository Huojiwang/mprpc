#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
#include "logger.h"

void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserverdesc = service->GetDescriptor();
    std::string service_name = pserverdesc->name();
    // 获取服务service的方法数量
    int methodCount = pserverdesc->method_count();
    LOG_INFO("service_name : %s",service_name.c_str());
    ServiceInfo service_info;

    for (int i = 0; i < methodCount; ++i)
    {
        const google::protobuf::MethodDescriptor *pmethodDesc = pserverdesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        // std::cout << "Service name : " << method_name << std::endl;
        LOG_INFO("method_name : %s",method_name.c_str());
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}
// 启动rpc服务节点，开始提供Remote网络调用服务
void RpcProvider::Run()
{
    //     std::unique_ptr<muduo::net::TcpServer> m_tcpserverptr;
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    // 绑定连接回调和消息读写回调方法  分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));

    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    // 设置muduo库的线程数量
    server.setThreadNum(8);
    // 启动网路哦服务
    ZkClient zkclient;  //1/3timeout事件发送ping消息心跳包
    zkclient.Start();

    for(auto &sp : m_serviceMap){
        std::string service_path = "/"+sp.first;
        zkclient.Create(service_path.c_str(),nullptr,0);
        for(auto &mp : sp.second.m_methodMap){
            std::string method_path = service_path+"/"+mp.first;
            char method_path_data[128] = {0};
            sprintf(method_path_data,"%s:%d",ip.c_str(),port);
            zkclient.Create(method_path.c_str(),method_path_data,strlen(method_path_data),ZOO_EPHEMERAL);
        }
    }

    std::cout << "RpcProvider start service at ip : " << ip << " port : " << port << std::endl;
    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        conn->shutdown();
    }
}

/*
在框架内部，RPCprovide和RPCconsumer要协商好通信用的protobuf数据类型
通过service对象找到 method map，定义proto message类型，进行数据头的序列化和反序列化
service_name   method_name  args_size args
header_size + header_str + args_str
*/
// 已建立链接用户的读写事件回调，如果远程有一个rpc服务的调用请求，那么OnMeeage方法就会响应
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp timestamp)
{
    std::string recv_buf = buffer->retrieveAllAsString();

    uint32_t header_size = 0;
    // recv_buf.copy((char *)&header_size, 4, 0);
    header_size = (static_cast<unsigned char>(recv_buf[0]) << 24) |
                      (static_cast<unsigned char>(recv_buf[1]) << 16) |
                      (static_cast<unsigned char>(recv_buf[2]) << 8) |
                      (static_cast<unsigned char>(recv_buf[3]));

    
    // 根据header_size读取数据头的原始字符流
    std::cout<<"recv header size is:"<<header_size<<std::endl;
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    // 反序列化数据得到rpc请求的详细信息
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(rpc_header_str))
    {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else
    {
        //+log
        std::cout << "rpc_header_str " << rpc_header_str << " parse error!" << std::endl;
    }
    std::string args_str = recv_buf.substr(4 + header_size, args_size);
    std::cout << "header size : " << header_size << std::endl;
    std::cout << "rpc_header_str : " << rpc_header_str << std::endl;
    std::cout << "service_name : " << service_name << std::endl;
    std::cout << "method_name : " << method_name << std::endl;
    std::cout << "args_str : " << args_str << std::endl;

    // 获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end())
    {
        std::cout << "service name : " << service_name << " is not exist" << std::endl;
        return;
    }
    // 获取service对象
    google::protobuf::Service *service = it->second.m_service;

    auto methodit = it->second.m_methodMap.find(method_name);
    if (methodit == it->second.m_methodMap.end())
    { // 方法不存在
        std::cout << "method name : " << service_name << " is not exist" << std::endl;
        return;
    }
    // 获取method方法
    const google::protobuf::MethodDescriptor *method = methodit->second;

    // 生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *requst = service->GetRequestPrototype(method).New();
    if (!requst->ParseFromString(args_str))
    {
        std::cout << "request parse error, content" << args_str << std::endl;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    // 框架上根据远端rpc请求，调用当前rpc节点上发布的方法
    // new UserService().Login(controller,request,responese,done)

    // 给下面的method方法调用，绑定一个closure的回调函数
    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr &, google::protobuf::Message *>(this, &RpcProvider::SendRpcResponse, conn, response);
    service->CallMethod(method, nullptr, requst, response, done);
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
{
    std::string response_str;

    if (response->SerializeToString(&response_str))
    {
        // 序列化成功后，通过网络把rpc方法执行的结果发送回rpc调用方
        conn->send(response_str);
    }
    else
    {
        std::cout << "response error" << response_str << std::endl;
    }
    conn->shutdown(); // 模拟http的短链接服务，由rpcprovider主动断开连接
}