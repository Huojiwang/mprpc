#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include "logger.h"

void global_watcher(zhandle_t *zh, int type,
                    int state, const char *path, void *watcherCtx)
{
    if(type == ZOO_SESSION_EVENT){  //callback message type是和session relation的
        if(state == ZOO_CONNECTED_STATE){   //connnect success
            sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : m_zhandle(nullptr)
{
}
ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle);
    }
}
// 启动zkServer
void ZkClient::Start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    /*
    zookeeper_mt 多线程版本 3个线程
    API调用线程
    网络IO线程  pthread_create poll
    watcher回调线程 pthread_create
    */
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle)
    {
        LOG_ERR("zookeeper [error] :%s", "zookeeper_init error");
        exit(EXIT_FAILURE);
    }
    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout << "zookeeperi init success! " << std::endl;
}
// 在zkServer上根据path创建znode节点
void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buf[128];
    int buf_len = sizeof(path_buf);
    int flag;

    flag = zoo_exists(m_zhandle,path,0,nullptr);
    if(ZNONODE == flag){
        flag = zoo_create(m_zhandle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,path_buf,buf_len);

        if(ZOK == flag){
            std::cout<<"Znode create success... path"<<path<<std::endl;
        }else{
            std::cout<<"flag : "<<flag<<std::endl;
            std::cout<<"Znode create error.. path"<<path<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
// 根据参数指定的znode节点路径或者znode节点的值
std::string ZkClient::GetData(const char *path)
{
    char buf[64];
    int buflen = sizeof(buf);
    int flag = zoo_get(m_zhandle,path,0,buf,&buflen,nullptr);
    if(flag != ZOK){
        std::cout<<"Get znode error... path "<<path<<std::endl;
        return "";
    }else{
        return buf;
    }
}