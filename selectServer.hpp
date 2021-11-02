#pragma once 

#include "Sock.hpp"

#define NUM sizeof(fd_set)*8//数组大小=最多能监视文件个数
#define DET_FD -1//数组默认文件描述符 

class SelectServer{
  private:
    int _lsock;//套接字
    int _port;//端口号
    int array[NUM];//保存要监视的文件描述符
  public:
    SelectServer(int lsock = -1, int port = 8080)
    :_lsock(lsock)
    ,_port(port)
    {}
    void InitServer(){
      for(size_t i =  0; i < NUM; i++){
        array[i] = DET_FD;
      }
      _lsock = Sock::Socket();
      Sock::Bind(_lsock, _port);
      Sock::Listen(_lsock);
      array[0] = _lsock;

    }
    void AddtoArray(int index){
      //找到没有数组没有占用的位置
      size_t i = 0;
      for(; i < NUM; i++){
        if(array[i] == DET_FD){
          break;
        }
      }
      //满了
      if(i >= NUM){
        cout<<"select is full, close fd"<<endl;
        close(index);
      }
      else{
        array[i] = index;
      }
    }
    void Delete(size_t index){
      if(index >= 0 && index < NUM){
        array[index] = DET_FD;
        
      }

    }
    void Handle(int i){
      //IO条件就绪
      char buf[10240];
      ssize_t n = recv(array[i], buf, sizeof(buf), 0);
      if(n > 0){
        buf[n] = 0;
        cout<<buf<<endl;
      }
      else if(n == 0){
        //对端关闭
        cout<<"client close..."<<endl;
        close(array[i]);
        //文件已经关闭，还需要将数组文件描述符删除
        Delete(i);
      }
      else{
        cerr << "read error"<<endl;
        close(array[i]);
        Delete(i);
      }


    }
    void Start(){
      while(1){
        int maxfd = DET_FD;
        //重新设定，需要等待的文件
        fd_set readfds;
        //初始化fd_set
        FD_ZERO(&readfds);
        //找文件描述符，将要监视的fd_set对应位设置为1
        for(size_t i =0; i < NUM; i++){
          if(array[i] == DET_FD){
            continue;
          }
          cout <<array[i];
          FD_SET(array[i], &readfds);
          //找文件描述符最大值
          if(maxfd < array[i]){
            maxfd = array[i];
          }
        }
        cout<<endl;
        //struct timeval timeout = {5, 0};
        //调用 select 等待多个文件
        //阻塞等待
        int fdn = select(maxfd+1, &readfds, nullptr, nullptr, nullptr);
        if(fdn > 0){
          //有文件就绪
          //找哪个文件就绪
          for(size_t i =0; i < NUM; i++){
            if(array[i] != DET_FD && FD_ISSET(array[i] , &readfds)){
              if(array[i] == _lsock){
                //有新连接
                int sock = Sock::Accept(array[i]);
                if(sock >= 0){
                  cout << "get a link...."<<endl;
                  //加入到数组中
                  AddtoArray(sock);
                }
                
              }
              else{
                //进行IO操作
                Handle(i);
              }
            }
          }

        }
        else if(fdn == 0){
          cerr << "select timeout..."<<endl;

        }
        else{
          cerr <<"fdn:"<<fdn<< "select error"<<endl;
        }


      }
    }

    ~SelectServer(){
      for(size_t i = 0; i < NUM; i++){
        if(array[i] != DET_FD){
          close(array[i]);
        }
      }
    }



};
