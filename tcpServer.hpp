#pragma once 

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BOCKLOG 5
class tcpServer{
  private:
    int _port;
    int _lsock;

  public:
    tcpServer(int port = 8080)
      :_port(port)
       ,_lsock(-1)
    {}
    void initServer(){
      //创建套接字
      _lsock = socket(AF_INET, SOCK_STREAM, 0);
      if(_lsock < 0){
        std::cerr<<"socket error"<<std::endl;
        exit(1);
      }
      struct sockaddr_in local;
      local.sin_family = AF_INET;
      local.sin_port = htons(_port);//保存在内存中是主机字节序，要转成网络字节序
      local.sin_addr.s_addr = htons(INADDR_ANY);
      //绑定
      if(bind(_lsock,(struct sockaddr *)&local, sizeof(local))<0){
        std::cerr<<"bind error"<<std::endl;
        exit(2);
      }
      //设为监听状态
      if(listen(_lsock, BOCKLOG) < 0){
        std::cerr<<"listen error"<<std::endl;
        exit(1);
      }


    }
    void  Server(int sock){
      while(1){
        char bufer[64];
        ssize_t n = recv(sock, bufer, sizeof(bufer), 0); 
        if(n > 0){
          bufer[n] = 0;
          //读到数据
          std::cout<<"client #"<<bufer<<std::endl;

          //发送数据
          //建立连接了发送时不需要客户端ip和port
          send(sock, bufer, sizeof(bufer), 0);
        }
        else if(n == 0){
          //客户端退出，没写入数据，服务器读到0
          //退出服务，去连接客户端
          std::cout<<"client quit"<<std::endl;
          break;
        }
        else{
          //读取失败
          break;
        }
      }
      //关闭accept创建的文件
      close(sock);
    }
    
    void start(){
      signal(SIGCHLD, SIG_IGN);
      while(1){
        //接收连接
        struct sockaddr_in cli;
        socklen_t len = sizeof(cli);
        int sock = accept(_lsock, (struct sockaddr *)&cli, &len);
        if(sock < 0){
          std::cerr<<"accept error"<<std::endl;
        }
        std::string cli_infor = inet_ntoa(cli.sin_addr);
        cli_infor += ':';
        cli_infor += std::to_string(ntohs(cli.sin_port));

        std::cout<<"get a link...."<<"client :"<<cli_infor<<std::endl;
        pid_t id = fork();
        if(id == 0){//子进程提供服务
          //提供服务
          //子进程退出会向父进程发送SIGCHLD信号
          //如果处理动作为忽略系统会自动释放子进程资源
          close(_lsock);//关闭_lsock,不使用，不影响父进程
          Server(sock);
          exit(0);

        }
        close(sock);//关闭sock，不使用，不影响子进程
        //父进程去链接进程，不需要等待
      }

    }
    ~tcpServer(){
      close(_lsock);
    }

};
