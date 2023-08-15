#pragma once

#include<google/protobuf/service.h>
#include<string>

class MprpcController : public google::protobuf::RpcController{
    public:
        MprpcController();
        void Reset();
        bool Failed() const;
        std::string ErrorText() const;
        void SetFailed(const std::string& reason);
        

        void StartCancel();
        bool IsCanceled() const;
        void NotifyOnCancel(google::protobuf::Closure* callback);

    private:
        bool m_failed;  //rpc执行过程中的失败状态
        std::string m_errtext;

};