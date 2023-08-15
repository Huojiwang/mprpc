#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include "mprpcchannel.h"
#include<mprpcapplication.h>
#include"rpcheader.pb.h"
#include<zookeeperutil.h>
#include"mprpccontroller.h"

#define CLOSE_SETCONTROLLER(fd,msg) do{\
    close(fd);\
    controller->SetFailed(msg);\
}while(0)


void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller, const google::protobuf::Message *request,
                              google::protobuf::Message *response, google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();


    //获取参数的序列化字符串长度 args_size
    std::string args_str;
    uint32_t args_size;
    if(request->SerializeToString(&args_str)){
        args_size = args_str.size();
    }else{
        std::cout<<"Serialize request error!"<<std::endl;
        return ;
    }
    mprpc::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);
    rpcheader.set_method_name(method_name);
    rpcheader.set_args_size(args_size);

    std::string rpc_header_str;
    uint32_t header_size = 0;
    if(rpcheader.SerializeToString(&rpc_header_str)){
        header_size = rpc_header_str.size();
    }else{
        controller->SetFailed("Serialize error");
        return;
    }
    //组织待发送的rpc请求的字符串
    std::string send_rpc_str;

    char header[4];
    header[0] = (header_size >> 24) & 0xFF;
    header[1] = (header_size >> 16) & 0xFF;
    header[2] = (header_size >> 8) & 0xFF;
    header[3] = header_size & 0xFF;
    // 将头部信息插入到字符串开头
    send_rpc_str.insert(0, header, 4);
   
    std::cout<<"rpc_send_str_head_size : "<<send_rpc_str.size()<<std::endl;
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    std::cout<<"rpc_header_str : "<<rpc_header_str<<std::endl;
    std::cout<<"service_name : "<<service_name<<std::endl;
    std::cout<<"method_name : "<<method_name<<std::endl;
    std::cout<<"args_str : "<<args_str<<std::endl;

    //Shiyong tcp编程完成rpc远程调用
    int clientfd = socket(PF_INET,SOCK_STREAM,0);
    if(-1 ==  clientfd){
        std::cout<<"errno = "<<errno<<std::endl;
        exit(EXIT_FAILURE);
    }

    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    //rpc调用方想要调用service_name的method服务，需要查询zk上该服务所在host信息
    ZkClient zkcli;
    zkcli.Start();

    std::string method_path = "/"+service_name+"/"+method_name;
    std::string host_data = zkcli.GetData(method_path.c_str());
    if("" == host_data){
        controller->SetFailed(method_path + "is not exist!");
        return ;
    }
    int pos = host_data.find(":");
    if(-1 == pos){
        controller->SetFailed(method_path+"address is invalid!");
        return ;
    }
    
    std::string ip = host_data.substr(0,pos);
    uint16_t port = atoi(host_data.substr(pos+1,host_data.size()-pos).c_str());

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    if(-1 ==  connect(clientfd,(struct sockaddr*)&servaddr,sizeof(servaddr))){
        CLOSE_SETCONTROLLER(clientfd,"connection error! errno is ");
        exit(EXIT_FAILURE);
    }
    //发送rpc请求
    if(-1 == send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0)){
        CLOSE_SETCONTROLLER(clientfd,"send error!  errno : ");
        return ;
    }
    //接收rpc响应数据
    char recvbuf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd,recvbuf,1024,0))){
        CLOSE_SETCONTROLLER(clientfd,"recv error!  errno : ");
        return ;
    }
    //读取返回结果
    // std::string response_srt(recvbuf,0,recv_size);//recvbuf中遇到\0导致反序列化失败
    if(!response->ParseFromArray(recvbuf,recv_size)){
        // std::cout<<"response parse error!  errno : "<<errno<<std::endl;
        CLOSE_SETCONTROLLER(clientfd,"response parse error!  errno : ");
        return;
    }
    //反序列化rpc调用的结果
}
