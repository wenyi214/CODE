#pragma once 
#include <iostream>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include<sys/socket.h>

#define NUM 5
struct Task{
  int _sock;
  Task(){};
  Task(int sock)
  :_sock(sock)
  {}
  //Task(int x, int y){
  //  _x=x;
  //  _y=y;
  //}
  ~Task(){
    close(_sock);
  };
  void Add(){
      char bufer[64];
      ssize_t n = recv(_sock, bufer, sizeof(bufer), 0); 
      if(n > 0){
        bufer[n] = 0;
        //读到数据
        std::cout<<"client #"<<bufer<<std::endl;

        //发送数据
        //建立连接了发送时不需要客户端ip和port
        send(_sock, bufer, sizeof(bufer), 0);
      }
      else if(n == 0){
        //客户端退出，没写入数据，服务器读到0
        //退出服务，去连接客户端
        std::cout<<"client quit"<<std::endl;
      }
      else{
        //读取失败
      }
  }

};

class mythreadpool{
  private:
    void ThreadLock(){
      pthread_mutex_lock(&_lock);
    }
    void ThreadUnlock(){
      pthread_mutex_unlock(&_lock);
    }
    void ThreadWait(){
      pthread_cond_wait(&_cond, &_lock);
    }
    void ProductorWait(){
      pthread_cond_wait(&_pcond, &_lock);
    }
    void ProductorSignal(){
      pthread_cond_signal(&_pcond);
    }
    void ThreadSignal(){
      pthread_cond_signal(&_cond);
    }
    void ThreadSignalAll(){
      pthread_cond_broadcast(&_cond);
    }
    bool IsEmpty(){
      return _q.empty();
    }
    void ThreadExit(){
      _numthread--;
      printf("%lu is exit\n",pthread_self());
      ProductorSignal();
      pthread_exit(0);
    }

  public:
    mythreadpool(size_t num = NUM)
    :_numthread(num)
     ,_quit(false)
    {}
    //成员函数，由this指针作为参数，
    //线程函数，只能有一个参数，所以要加static
    static void *handler(void *arg){
      mythreadpool *p_this = (mythreadpool *)arg;
      while(1){
        //usleep(100);
        p_this->ThreadLock();
        while(!(p_this->_quit)&&(p_this->IsEmpty())){
          p_this->ThreadWait();
        }
        if(p_this->_quit&&p_this->IsEmpty()){
          p_this->ThreadUnlock();
          p_this->ThreadExit();
        }
        //为了拿到真正的Task，后面要释放空间
        Task *t = new Task();
        p_this->Get(&t);
        p_this->ThreadUnlock();
        
        t->Add();
        delete t;
        //std::cout<<pthread_self()<<":"<<t._x<<"+"<<t._y<<"="<<res<<std::endl;

      }

    }
    void threadinit(){
      pthread_mutex_init(&_lock, nullptr);
      pthread_cond_init(&_cond, nullptr);
      pthread_cond_init(&_pcond, nullptr);
      pthread_t td[_numthread]; 
      size_t i=0;
      for(i=0; i<_numthread; i++){
        pthread_create(td+i, nullptr, handler, (void *)this);
      }
      for(i=0; i<_numthread; i++){
        pthread_detach(td[i]);
      }


    }
    void Get(Task **out){
     Task *t = _q.front();
     _q.pop();
     *out=t;
    }

    void Put(Task& in){
      ThreadLock();
      _q.push(&in);
      ThreadUnlock();
      ThreadSignal();
    }

    ~mythreadpool(){
      pthread_mutex_destroy(&_lock);
      pthread_cond_destroy(&_cond);
      pthread_cond_destroy(&_pcond);

    }
    //线程退出
    void ThreadQuit(){
      ThreadLock();
      _quit = true;
      ThreadSignalAll();
      while(_numthread!=0){
        ProductorWait();
      }
      ThreadUnlock();
    }
  private:
    std::queue<Task*> _q;//任务队列
    size_t _numthread;//线程个数
    pthread_mutex_t _lock;//锁
    pthread_cond_t _cond;//消费者在此等待
    pthread_cond_t _pcond;//生产者在此等待
    
    bool _quit;//是否退出
};
