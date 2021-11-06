#pragma once
#include <iostream>
#include <string>
#include <sys/time.h>
//日志级别
enum Level{
  Notice = 1,//正常
  Warning,   //警告，不影响，不急着处理
  Error,     //错误，急着处理
  Fatal      //进程奔溃
};
//退出码
enum ErrMsg{
  SocketErr = 1,
  BindErr,
  ListenErr,
};

//__FILE__获得文件名，__LINE__获得调用行
#define Log(level, message) log(#level, message, __FILE__, __LINE__)

void log(std::string level, std::string message, std::string filename, size_t line){
    struct timeval time;
    gettimeofday(&time, nullptr);
    //日志级别+描述+时间戳+文件名+行号
    std::cout<<"[ "<<level<<" ]"<<"[ "<<message<<" ]"<<"[ "<<time.tv_sec<<" ]"<<"[ "<<filename<<" ]"<<"[ "<<line<<" ]"<<std::endl;
}



