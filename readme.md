# RPC通信框架

## protoubuf

* 生成c++service类型需要的

``` proto
syntax = "proto3";  //指定版本
package fixbug; //namespace
option cc_generic_services = true;
```

* protobuf中定义描述rpc方法的类型

``` proto
service UserServiceRpc{
    rpc Login(LoginRequest) returns (LoginResponse);//名字最好与本地的方法保持一致
    rpc Register(RegisterRequest) returns (RegisterResponse);
}
```

* 编译生成cc文件

```bash
protoc *.proto --cpp_out=./
example : protoc user.proto --cpp_out=./
```

* 对于被调用方 生成的UserServiceRpc类

``` cpp
/*
重写Login 和 Register函数，实现业务，一般来讲，使用UserService继承UserServiceRpc
在Login和register中调用业务方法，主要任务还是在这两个函数中进行序列化和反序列化的任务，将序列化的数据传递给自己写的方法中
 */
virtual void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done);
/*
done是执行完从request中反序列化数据，并且设置Response后的回调函数对象，他只有一个virtual void Run() = 0;
他是一个虚基类，所以需要定义一个类继承自它然后实现它的run方法
*/
virtual void Register(::google::protobuf::RpcController* controller,
                        const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)

void CallMethod(const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method,
                  ::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                  const ::PROTOBUF_NAMESPACE_ID::Message* request,
                  ::PROTOBUF_NAMESPACE_ID::Message* response,
                  ::google::protobuf::Closure* done);

const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* GetDescriptor();
```

* mprpc基础类(单例模式)

``` cpp
class MprpcApplication{
    public:
        static void Init(int argc,char **argv); //getopt初始化命令行参数 将从.conf中读到的数据存放到class m_config类中的unordered_map中
        static MprpcApplication& GetInstance(); //获取单例实例
        static MprpcConfig& GetConfig();    //m_config也是单例
    private:
        static MprpcConfig m_config;

        MprpcApplication(){}
        MprpcApplication(const MprpcApplication&)=delete;
        MprpcApplication(MprpcApplication&&)=delete;
};

//MprpcConfig类
class MprpcConfig{
    public:
    //解析加载配置文件
        void LoadConfigFile(const char *config_file);
    //查询配置项信息
        std::string Load(const std::string &key);
    //去掉字符串前后的空格
        void Trim(std::string &src_buf);
    private:
        std::unordered_map<std::string,std::string> m_configfile;
};
```

* RpcProvider Rpc服务提供者

``` cpp
class RpcProvider{
    public:
        //这里时框架提供给外部使用的，可以发布rpc方法的函数接口
        void NotifyService(google::protobuf::Service *service);
        //启动rpc服务节点，开始提供Remote网络调用服务
        void Run();

    private:
        struct ServiceInfo
        {
            google::protobuf::Service *m_service;   //保存服务对象
            std::unordered_map<std::string,const  google::protobuf::MethodDescriptor *> m_methodMap;    //保存服务方法
        };
        //存储注册成功的服务对象和调用方法
        std::unordered_map<std::string,ServiceInfo> m_serviceMap;
        
        
        //组合eventloop
        muduo::net::EventLoop m_eventLoop;
        void OnConnection(const muduo::net::TcpConnectionPtr &conn);
        //已建立链接用户的读写事件回调
        void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*,muduo::Timestamp);

        //回调操作closure，用于序列化rpc的响应和网络发送
        void SendRpcResponse(const muduo::net::TcpConnectionPtr&,google::protobuf::Message*);
};
```

* 对于调用方 生成的UserServiceRpc_Stub类

``` cpp
//类中包含两个函数
virtual void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done);
                       
virtual void Register(::google::protobuf::RpcController* controller,
                        const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)

```

* zookeeper 3.8.2安装 c/c++API

``` bash
    # 首先先下载release版本apache-zookeeper-<version>.tar.gz
    tar -xzf apache-zookeeper-x.x.x.tar.gz

    #需要的组件
    apt-get install libcppunit-dev
    apt-get install python-setuptools python2.7-dev
    apt-get install openssl libssl-dev
    apt-get install libsasl2-modules-gssapi-mit libsasl2-modules libsasl2-dev

    #进入
    cd apache-zookeeper-x.x.x
    cd zookeeper-client/zookeeper-client-c/
    mvn clean -Pfull-build
    mvn install -Pfull-build -DskipTests

    #具体可以查看当前命令下的README，刚开始是没有configure这个文件的
    ACLOCAL="aclocal -I /usr/local/share/aclocal" autoreconf -if
    ./configure 
    sudo make 
    sudo make install

    #使用的时候需要加一行
    #define THREADED    exitst    
    #不加的话使用的是    aexist
```

* zookeeper 作为分布式节点的中间件

``` cpp
    //封装的ck客户端类
class ZkClient{
    public:
        ZkClient();
        ~ZkClient();
        //启动zkServer
        void Start();
        //在zkServer上根据path创建znode节点
        void Create(const char *path, const char *data,int datalen, int state = 0);
        //根据参数指定的znode节点路径或者znode节点的值
        std::string GetData(const char *path);
    private:
        zhandle_t *m_zhandle;//zk客户端句柄
};
```

![Alt text](RPC%E9%A1%B9%E7%9B%AE%E6%95%B4%E4%BD%93%E6%B5%81%E7%A8%8B.png)
