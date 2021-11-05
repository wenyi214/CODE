#pragma once 

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define BACKLOG 5
using namespace std;

class Sock{
  public:
    static int Socket(){
      int sock = 0;
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if(sock < 0){
        cerr<<"socket error"<<endl;
        exit(1);
      }
      return sock;
    }
    static void Bind(int sock, int port){
      struct sockaddr_in local;
      local.sin_family = AF_INET;
      local.sin_port = htons(port);
      local.sin_addr.s_addr = htons(INADDR_ANY);
      if(bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0){
        cerr << "bind error"<<endl;
        exit(2);
      }
    }

    static void Listen(int sock){
      if(listen(sock,BACKLOG) < 0){
        cerr << "listen error"<<endl;
        exit(3);
      }
    }
    static int Accept(int sock){
      struct sockaddr_in peer;
      socklen_t len = sizeof(peer);
      return accept(sock, (struct sockaddr *)&peer, &len);
    }
    static void SetSockOpt(int sock){
      int opt =1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }
};
