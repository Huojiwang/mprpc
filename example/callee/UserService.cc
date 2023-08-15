#include<iostream>
#include<string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"


class UserService : public fixbug::UserServiceRpc{
    public:
        bool Login(std::string name, std::string pwd){
            std::cout<<"doing local service : Login"<<std::endl;
            std::cout<<"name:"<<name<<" pwd : "<<pwd<<std::endl;
            return true;
        }
        bool Register(uint32_t id,std::string name,std::string pwd){
            std::cout<<"Register service : Register"<<std::endl;
            std::cout<<"id"<<id<<"name:"<<name<<" pwd : "<<pwd<<std::endl;
            return true;
        }
        //重写基类UserService的虚函数，下面这些方法都是框架直接调用的
        void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done){
                            //框架给业务上报了请求参数LoginRequest，应用获取相应的数据做本地业务
                            std::string name = request->name();
                            std::string pwd = request->pwd();

                            //做本地业务
                            bool Login_res =  Login(name,pwd);
                            //把响应写入
                            fixbug::ResultCode *code = response->mutable_result();
                            code->set_errcode(0);
                            code->set_errmsg("");
                            response->set_success(Login_res);
                            //执行回调
                            done->Run();//执行响应对象的数据的序列化和网络发送（都是由框架来完成的）
                       }
        void Register(::google::protobuf::RpcController* controller,
                        const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done){
                        uint32_t id = request->id();
                        std::string name = request->name();
                        std::string pwd = request->pwd();

                        bool ret = Register(id,name,pwd);

                        response->mutable_result()->set_errcode(0);
                        response->mutable_result()->set_errmsg("");
                        response->set_success(ret);

                        done->Run();
                       }

        
};

int main(int argc,char **argv){
    //调用框架初始化操作
    MprpcApplication::Init(argc,argv);
    RpcProvider provider;//是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    provider.NotifyService(new UserService());
    
    //启动一个rpc服务发布节点
    provider.Run();
    return 0;
}