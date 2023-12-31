#include<iostream>
#include"mprpcapplication.h"
#include "user.pb.h"
#include"mprpcchannel.h"


int main(int argc, char** argv){
    //整个程序启动后，想使用mprpc框架来享受rpc服务调用，一定需要西安调用rpc的初始化函数且值初始化一次
    MprpcApplication::Init(argc,argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    //rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    //rpc方法的响应
    fixbug::LoginResponse response;
    //发起rpc方法的调用，同步rpc方法的调用过程
    stub.Login(nullptr,&request,&response,nullptr);//调用的是RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    if(0 == response.result().errcode()){
        std::cout<<"rpc login response:"<<response.success()<<std::endl;
    }else{
        //+log
        std::cout<<"rpc login response error:"<<response.result().errmsg()<<std::endl;
    }

    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("zhangsan");
    req.set_pwd("123456");
    //rpc方法的响应
    fixbug::RegisterResponse rsp;
    //发起rpc方法的调用，同步rpc方法的调用过程
    stub.Register(nullptr,&req,&rsp,nullptr);//调用的是RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    if(0 == rsp.result().errcode()){
        std::cout<<"rpc login response:"<<rsp.success()<<std::endl;
    }else{
        //+log
        std::cout<<"rpc login response error:"<<rsp.result().errmsg()<<std::endl;
    }
    return 0;
}
