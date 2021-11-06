#pragma once

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <signal.h>
#include "Log.hpp"

#define BACKLOG 5

class Sock{
  public:
    static int Socket(){
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      if(sock < 0){
        Log(Fatal, "socket fail");
        exit(SocketErr);
      }
      return sock;
    }

    static void Bind(int sock, int port){
      struct sockaddr_in local;
      local.sin_family = AF_INET;
      local.sin_port = htons(port);
      local.sin_addr.s_addr = htons(INADDR_ANY);

      if(bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0){
        Log(Fatal, "bind fail");
        exit(BindErr);
      }
    }
    static void Listen(int sock){
      if(listen(sock, BACKLOG) < 0){
        Log(Fatal, "listen fail");
        exit(ListenErr);
      }
    }
    static void SetSockOpt(int sock){
      int opt = 1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }
    static int Accept(int lsock){
      struct sockaddr_in peer;
      socklen_t len = sizeof(peer);
      int sock = accept(lsock, (struct sockaddr *)&peer, &len);
      if(sock < 0){
        Log(Warning, "accept fail");
      }
      return sock;
    }
    //读取一行
    static void GetLine(int sock, std::string& line){
      //按字符读取
      char c = 'X';
      while(c != '\n'){
        //以阻塞方式读取一个字符
        //换行符\n,\r,\r\n -> \n
        ssize_t s = recv(sock, &c, 1, 0);
        //读到\r处理
        if(s > 0){
            if( c == '\r' ){
            //探测，是否是\r\n，不取数据，只是拿上来检测
            ssize_t l = recv(sock, &c, 1, MSG_PEEK);
            if(l > 0 &&c == '\n'){
              // \r\n,直接都上来替换
               recv(sock, &c, 1, 0);
            }
            else{
              // \r
               c = '\n';
            }

          }
          if(c != '\n'){
            line.push_back(c);
          }  
        }
        //else{
        //  std::cout<<s<<std::endl;
        //  Log(Warning, "recv fail");
        //}
      }
    }
      

};

