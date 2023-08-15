#pragma once
#include "lockqueue.h"
// mprpc日志系统
enum LogLevel
{
    INFO,
    ERROR
};

class Logger
{
public:
    void setLogLevel(LogLevel level);
    void Log(std::string msg);
    static Logger &GetInstance();

private:
    Logger();
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    int m_loglevel;
    LockQueue<std::string> m_Lqueue; // 日志缓冲队列
};

#define LOG_INFO(logmsgformat, ...)                     \
    do                                                  \
    {                                                   \
        Logger::GetInstance().setLogLevel(INFO);        \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger::GetInstance().Log(c);                   \
    } while (0);
#define LOG_ERR(logmsgformat, ...)                      \
    do                                                  \
    {                                                   \
        Logger::GetInstance().setLogLevel(ERROR);       \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger::GetInstance().Log(c);                   \
    } while (0);