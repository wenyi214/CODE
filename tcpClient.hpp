#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class tcpClient{
  private:
    std::string _ip;
    int _port;
    int _sock;
  public:
    tcpClient(std::string ip = "127.0.0.1", int port = 8080)
      :_ip(ip)
      ,_port(port)
      ,_sock(-1)
      {}
    void initClient(){
      _sock = socket(AF_INET,SOCK_STREAM,0);
      if(_sock < 0){
        std::cout<<"socket error"<<std::endl;
        exit(1);
      }
      //客户端不需要绑定，监听和接收连接
      //而是要去连接服务器
      //保存服务器的ip和port
      struct sockaddr_in sev;
      sev.sin_family = AF_INET;
      sev.sin_port = htons(_port);
      sev.sin_addr.s_addr = inet_addr(_ip.c_str());
      if(connect(_sock, (struct sockaddr *)&sev,sizeof(sev)) < 0){
        std::cerr <<"connect error"<<std::endl;
        exit(2);
      }
    
    }
    void start(){
      while(1){
        std::cout<<"Please Enter #";
        fflush(stdout);
        char tmp[64];
        size_t n =read(0, tmp, sizeof(tmp));
        //去掉/n
        if(n > 0){
          tmp[n-1] = 0;
          send(_sock, tmp, strlen(tmp),0);
          char buf[128];
          recv(_sock, buf, sizeof(buf), 0);
          std::cout<<"server #"<<buf<<std::endl;
        }
      }
    }
    ~tcpClient(){
      close(_sock);
    }
};


