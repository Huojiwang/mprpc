#pragma once
#include<semaphore.h>
#include<string>
#define THREADED
#include<zookeeper/zookeeper.h>

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