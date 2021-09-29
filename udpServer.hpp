#pragma once 
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


class udpServer{
  public:
    udpServer(const char *ip = "127.0.0.1", int port = 8080)
      :_ip(ip)
      ,_port(port)
      {}
    void InitServer(){
      //创建套接字
      _sock = socket(AF_INET, SOCK_DGRAM, 0);
      //用struct sockaddr_in保存ipv4的套接字
      struct sockaddr_in local;
      local.sin_family = AF_INET;
      local.sin_port = htons(_port);//端口号，字节序转网络字节序
      local.sin_addr.s_addr = inet_addr(_ip.c_str());//string装C语言字符串，在装整数，在转字节序
      //绑定,注意强转,将创建的套接字和文件绑定
      if(bind(_sock,(struct sockaddr *)&local, sizeof(local)) < 0){
        std::cerr<<"bind error"<<std::endl;     
        exit(1);
      }
    }

    void Start(){
      while(1){
        char buf[64];
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        //从网络读,返回值，实际读到多少字节数据
        ssize_t n=recvfrom(_sock, buf, sizeof(buf), 0, (struct sockaddr *)&peer, &len);
        buf[n]='\0';
        std::cout<<buf<<std::endl;
        //加字符串
        char str[128] = "Server send #";
        int x = strlen(str);
        int l = x +n;
        for(int i=x; i<l; i++){
          str[i] = buf[i-x];
        }
        str[l]='\0';
        //发送
        sendto(_sock, str, l, 0, (struct sockaddr *)&peer, len);
        
      }
      
    }
    
    ~udpServer(){

    }

  
  private:
    std::string _ip;//IP地址
    int _port;//端口号
    int _sock;//创建套接字返回的文件描述符

};
