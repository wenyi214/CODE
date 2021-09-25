#pragma once 
#include <iostream>
#include <queue>
#include <pthread.h>
#define NUM 5
struct Task{
  Task(){};
  Task(int x, int y){
    _x=x;
    _y=y;
  }
  ~Task(){};
  int _x;
  int _y;
  int Add(){
    return _x + _y;
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
    void ThreadSignal(){
      pthread_cond_signal(&_cond);
    }
    bool IsEmpty(){
      return _q.empty();
    }

  public:
    mythreadpool(size_t num = NUM)
    :_numthread(num)
    {}
    //成员函数，由this指针作为参数，
    //线程函数，只能有一个参数，所以要加static
    static void *handler(void *arg){
      mythreadpool *p_this = (mythreadpool *)arg;
      while(1){
        p_this->ThreadLock();
        while(p_this->IsEmpty()){
          p_this->ThreadWait();
        }
        Task t;
        p_this->Get(t);
        p_this->ThreadUnlock();
        int res=0;
        res = t.Add();
        std::cout<<pthread_self()<<":"<<t._x<<"+"<<t._y<<"="<<res<<std::endl;

      }

    }
    void threadinit(){
      pthread_mutex_init(&_lock, nullptr);
      pthread_cond_init(&_cond, nullptr);
      pthread_t td[_numthread]; 
      size_t i=0;
      for(i=0; i<_numthread; i++){
        pthread_create(td+i, nullptr, handler, (void *)this);
      }
      for(i=0; i<_numthread; i++){
        pthread_detach(td[i]);
      }


    }
    void Get(Task& out){
     Task *t = _q.front();
     _q.pop();
     out=*t;
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

    }
  private:
    std::queue<Task*> _q;
    size_t _numthread;//线程个数
    pthread_mutex_t _lock;
    //只能消费者等，生产者不仅要放任务，还需要受理用户请求
    pthread_cond_t _cond;

};
