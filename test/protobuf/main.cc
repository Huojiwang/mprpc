#include "test.pb.h"
#include<iostream>
#include<string>
using namespace fixbug;

int main(){
    LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    std::string send_str;
    if(request.SerializeToString(&send_str)){
        std::cout<<send_str<<std::endl;
    }
    LoginResponse rsp;
    // rsp.set_success();
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(1);
    rc->set_errmsg("login failure");

    GetFriendListResponese gfrsp;
    ResultCode *gfrc = gfrsp.mutable_result();
    rc->set_errcode(0);
    rc->set_errmsg("");

    User *user1 = gfrsp.add_friend_list();
    user1->set_name("zhangsan");
    user1->set_age(12);
    user1->set_sex(User::MAN);

    User *user2 = gfrsp.add_friend_list();
    user2->set_name("zhangsan");
    user2->set_age(12);
    user2->set_sex(User::MAN);

    std::cout<<gfrsp.friend_list_size()<<std::endl;

    
    return 0;
}

/*
int main(){
    LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    std::string send_str;
    if(request.SerializeToString(&send_str)){
        std::cout<<send_str<<std::endl;
    }
    LoginRequest request_R;
    if(request_R.ParseFromString(send_str)){
        std::cout<<request_R.name()<<std::endl;
        std::cout<<request_R.pwd()<<std::endl;
    }
    return 0;
}
*/