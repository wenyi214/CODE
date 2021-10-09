#pragma once 
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>


class Server{
  private:
    int _port;
    int _lsock;
  public:
    Server(int port = 8080)
      :_port(port)
      ,_lsock(-1)
    {}
    void initServer(){
      //自定义SIGCHLG信号的捕捉方式为忽略，子进程自动回收
      signal(SIGCHLD,SIG_IGN);
      _lsock = socket(AF_INET, SOCK_STREAM, 0);
      if(_lsock < 0){
        std::cerr <<"socket error"<<std::endl;
        exit(1);
      }
      struct sockaddr_in local;
      local.sin_family = AF_INET;
      local.sin_port = htons(_port);
      local.sin_addr.s_addr = htonl(INADDR_ANY);
      if(bind(_lsock,(struct sockaddr *)&local, sizeof(local)) < 0){
        std::cerr<<"bind error"<<std::endl;
        exit(2);
      }
      if(listen(_lsock, 5) < 0){
        std::cerr<<"listen error"<<std::endl;
        exit(3);
      }
    }
    void EchoServer(int sock){
      char request[2048];
      size_t s = recv(sock, request, sizeof(request), 0);
      if(s > 0){
        request[s] = 0;
        std::cout<<request<<std::endl;
        std::string response;
        response += "HTTP/1.1 200 ok\n";
        response += "Content-Type: text/html\n";
        response += "\n";
        response += "<html><body><h1>Welcome</h1><p>hello </p></body></html>";
        send(sock, response.c_str(), response.size(), 0);
        close(sock);

      }

    }
    void Start(){
      struct sockaddr_in peer;
      while(1){
        socklen_t len = sizeof(peer);
        int sock = accept(_lsock, (struct sockaddr *)&peer, &len);
        if(sock < 0){
          continue;
        }
        std::cout<<"get a link.."<<std::endl;
        if(fork() == 0){
          //child
          close(_lsock);
          EchoServer(sock);


          exit(0);
        }
        close(sock);


      }
    

    }

    ~Server(){
      close(_lsock);
    }
};
