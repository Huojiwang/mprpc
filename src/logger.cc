#include "logger.h"
#include<time.h>
#include<iostream>
Logger::Logger(){
    //启动线程写日志
    std::thread writeLogTask([&](){
        for(;;){
            //获取当天的日期，然后取日志信息，写入响应的日志文件当中
            time_t now = time(NULL);
            tm *nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name,"%d-%d-%d-log.txt",nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);

            FILE *pf = fopen(file_name,"a+");
            if(nullptr == pf){
                std::cout<<"lgger file open error"<<std::endl;
                exit(EXIT_FAILURE);
            }
            char time_buf[128] = {0};
            sprintf(time_buf,"%d:%d:%d==>[%s]",nowtm->tm_hour,nowtm->tm_min,nowtm->tm_sec,m_loglevel == INFO?"info":"error");
            std::string msg = m_Lqueue.pop();
            msg.insert(0,time_buf);
            msg.append("\n");
            fputs(msg.c_str(),pf);
            fclose(pf);
        }
    });
    writeLogTask.detach();
}

void Logger::setLogLevel(LogLevel level){
    m_loglevel = level;
}
void Logger::Log(std::string msg){
    //把日志信息写到lockqueue中
    m_Lqueue.push(msg);
}
Logger& Logger::GetInstance(){
    static Logger log;
    return log;
}