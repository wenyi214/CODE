#pragma once 

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BLACKLOG 5
using namespace std;

class Sock{
  public:
  static int Socket(){
    int sock = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
      cerr << "socket error"<<endl;
      exit(1);
    }
    return sock;
  }
  static void Bind(int sock, int port){
    struct sockaddr_in local;
    
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0){
      cerr << "bind error" <<endl;
      exit(3);
    }
  }
  static void Listen(int sock){
    if(listen(sock, BLACKLOG) < 0){
      cerr << "listen error"<<endl;
      exit(4);
    }

  }
  static int Accept(int lsock){
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    return  accept(lsock, (struct sockaddr *)&peer, &len);
  }


};
