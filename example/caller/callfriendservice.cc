#include<iostream>
#include"mprpcapplication.h"
#include "friend.pb.h"


int main(int argc, char** argv){
    //整个程序启动后，想使用mprpc框架来享受rpc服务调用，一定需要西安调用rpc的初始化函数且值初始化一次
    MprpcApplication::Init(argc,argv);
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    //rpc方法的请求参数
    fixbug::GetFriendListRequest request;
    request.set_userid(1000);
    // request.set_pwd("123456");
    //rpc方法的响应
    fixbug::GetFriendListResponese response;
    //发起rpc方法的调用，同步rpc方法的调用过程
    MprpcController contorller;
   

    stub.GetFriendList(&contorller,&request,&response,nullptr);//调用的是RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    if(contorller.Failed()){
        std::cout<<contorller.ErrorText()<<std::endl;
        exit(EXIT_FAILURE);
    }
    if(0 == response.result().errcode()){
        std::cout<<"rpc GetFriendList response success"<<std::endl;
        int size = response.friend_list_size();
        for(int i =0 ; i < size; ++i){
            std::cout<<"index:"<<(i+1)<<"name"<<response.friend_list(i).name()<<std::endl;
        }
    }else{
        //+log
        std::cout<<"rpc GetFriendList response error:"<<response.result().errmsg()<<std::endl;
    }

   
    return 0;
}
