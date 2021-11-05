#pragma once 

#include "Sock.hpp"

#define SIZE 20

class PollServer{
  private:
    int _lsock;
    int _port;
    struct pollfd fds[SIZE];//保存监视的文件
  public:
    PollServer(int lsock = -1, int port = 8081)
      :_lsock(lsock)
      ,_port(port)
    {}

    void InitServer(){
      _lsock = Sock::Socket();
      Sock::SetSockOpt(_lsock);
      Sock::Bind(_lsock, _port);
      Sock::Listen(_lsock);
      //初始化监视的文件
      for(int i=0; i < SIZE; i++){
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
      }
      //加入连接套接字
      fds[0].fd = _lsock;
      fds[0].events = POLLIN;
      fds[0].revents = 0;

    }
    //找到文件描述符为-1的加入到数组中
    void AddSock2Fds(int sock){
      int i =0;
      for(; i < SIZE; i++){
        if(fds[i].fd == -1){
          break;
        }
      }
      if(i >= SIZE){
        cout<<"full"<<endl;
        close(sock);
      }
      else{
        fds[i].fd = sock;
        fds[i].events = POLLIN;//设置为读事件
        fds[i].revents = 0;

      }

    }
    //删除一个不用监视的文件
    void DelSock(int i){
      fds[i].fd = -1;
      fds[i].events = 0;
      fds[i].revents = 0;
    }
    void Handler(){
      for(int i = 0; i < SIZE; i++){
        //用revents判断是否就绪
        if(fds[i].revents & POLLIN){
      
          if(fds[i].fd == _lsock){
            //有连接来了，加入到fds中
            int sock = Sock::Accept(_lsock);
            if(sock >= 0){
              cout <<"get a link..."<<endl;
              AddSock2Fds(sock);
            }
            else{
              cerr << "accept error"<<endl;
            }

          }
          else{
            //IO就绪
            char buf[10240] ={0};
            int n = recv(fds[i].fd, buf, sizeof(buf),0);
            if(n > 0){
              cout << "client# "<<buf<<endl;
            }
            else if(n == 0){
              cout<<"client close..."<<endl;
              close(fds[i].fd);
              DelSock(i);
            }
            else{
              cerr << "recv error"<<endl;
              close(fds[i].fd);
              DelSock(i);
            }

          }
        }
      }

    }
    void Start(){
      int timeout = -1;
      while(1){
        int num = poll(fds, SIZE, timeout);
        if(num > 0){
          Handler();

        }
        else if(num == 0){
          //超时
          cerr << "timeout..."<<endl;
        }
        else{
          cerr << "poll error..."<<endl;
        }
      }

    }

    ~PollServer(){
      for(int i =0; i < SIZE; i++){
        if(fds[i].fd != -1){
          close(fds[i].fd);
        }
      }
    }
};
