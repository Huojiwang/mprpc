#include"mprpcconfig.h"
#include<iostream>
#include<string>
//解析加载配置文件

void MprpcConfig::LoadConfigFile(const char *config_file){
    FILE *pf  = fopen(config_file,"r");
    if(nullptr == pf){
        std::cout<<config_file<< " is not exist!"<<std::endl;
        exit(EXIT_FAILURE);
    }
    //1.注释 2 正确的配置项 3 去掉开头的空格
    while (!feof(pf))
    {
        /* code */
        char buf[512] = {0};
        fgets(buf,512,pf);

        //remove place
        std::string  src_buf(buf);
        Trim(src_buf);
        if(src_buf[0] == '#' || src_buf.empty()){
            continue;
        }
        
        //判断配置项
        int idx = src_buf.find("=");
        if(idx == -1){
            //+log
            continue;
        }
        std::string key;
        std::string value;
        key = src_buf.substr(0,idx);
        Trim(key);

        int endidx = src_buf.find('\n',idx);
        value = src_buf.substr(idx+1,endidx-idx-1);
        Trim(value);
        m_configfile.insert({key,value});
    }
    
}
    //查询配置项信息
std::string MprpcConfig::Load(const std::string &key){
    auto it = m_configfile.find(key);
    if( it == m_configfile.end()){
        return "";
    }
    return it->second;
}

void MprpcConfig::Trim(std::string &src_buf){
    int idx = src_buf.find_first_not_of(' ');
        if(idx != -1){
            src_buf = src_buf.substr(idx,src_buf.size()-idx);
        }
        idx = src_buf.find_last_not_of(' ');
        if(idx != -1){
            src_buf = src_buf.substr(0,idx+1);
        }

       

}