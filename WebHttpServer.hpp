#pragma once

#include "Sock.hpp"
#include "Log.hpp"
#include "Protocol.hpp"
#include "ThreadPool.hpp"

class httpserver{
  private:
    int _port;
    int _lsock;
    ThreadPool *tp;

  public:
    httpserver(int port = 8081)
    :_port(port)
    ,_lsock(-1)
    {}
    
    void InitServer(){
      signal(SIGPIPE, SIG_IGN);
      _lsock = Sock::Socket();
      Sock::SetSockOpt(_lsock);
      Sock::Bind(_lsock, _port);
      Sock::Listen(_lsock);
      //
      tp = ThreadPool::GetInstance();
      tp->InitThreadPool();
      
    }
    void Start(){

      while(true){
        int sock = Sock::Accept(_lsock);
        if(sock < 0){
          continue;
        }
        Log(Notice, "get a link...");
        Task *tk = new Task(sock);
        tp->PushTask(tk);

        //多线程方案
        //在堆上申请空间，保存sock。
        //直接传入sock地址，当来了其它连接，会修改sock值
        //int *sockp = new int(sock);
        //处理协议后需要通过套接字接送数据和发送数据，所以用套接字做参数
        //pthread_t tid;
        //pthread_create(&tid, nullptr, Entry::handlerhttp, (void *)sockp);
        //线程分离，自动回收，避免等待
        //pthread_detach(tid);
      }

    }

    ~httpserver(){
      if(_lsock >= 0){
        close(_lsock);
      }
    }


};
