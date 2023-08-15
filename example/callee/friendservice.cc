#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"
#include <vector>

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t userid)
    {
        std::cout << " Do GetFriendList Service! userid" << userid<<std::endl;

        std::vector<std::string> vec;

        vec.push_back("gaoyang");
        vec.push_back("liuyang");
        vec.push_back("wangshuo");

        return vec;
    }
    //override baseClass method
    void GetFriendList(::google::protobuf::RpcController *controller,
                       const ::fixbug::GetFriendListRequest *request,
                       ::fixbug::GetFriendListResponese *response,
                       ::google::protobuf::Closure *done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendList = GetFriendList(userid);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        
        for(std::string &name: friendList){
            fixbug::User *user = response->add_friend_list();
            user->set_name(name);
            user->set_age(18+rand()%3);
            user->set_sex(fixbug::User::MAN);
        }

        done->Run();
    }
};

int main(int argc,char **argv){
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
    MprpcApplication::Init(argc,argv);
    RpcProvider provider;//是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    provider.NotifyService(new FriendService());
    //启动一个rpc服务发布节点
    provider.Run();
    return 0;
}