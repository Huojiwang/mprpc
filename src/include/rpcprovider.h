#pragma once
#include"google/protobuf/service.h"
#include<memory>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/TcpConnection.h>
#include<functional>
#include<google/protobuf/descriptor.h>
#include "zookeeperutil.h"

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