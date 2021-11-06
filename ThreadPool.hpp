#pragma once 

#include <iostream>
#include <queue>
#include <pthread.h>
#include "Protocol.hpp"

#define NUM 4

typedef void(*hander)(int);

class Task{
  private:
    int sock;
    hander hd;
  public:
    Task(){}
    Task(int _sock)
      :sock(_sock)
      ,hd(Entry::handlerhttp)//初始化为处理协议函数
    {}
    void Run(){
      //运行http服务
      hd(sock);
    }
    ~Task()
    {
      close(sock);
    }


};

class ThreadPool{
  private:
    std::queue<Task *> tq;
    int num;//线程数
    pthread_mutex_t lk;
    pthread_cond_t cond;
    //单例模式
    static ThreadPool *tp;
    static pthread_mutex_t mt;

  private:
    bool IsEmpty(){
      return tq.size() == 0;
    }
    void ThreadLock(){
      pthread_mutex_lock(&lk);
    }
    void ThreadUnlock(){
      pthread_mutex_unlock(&lk);
    }
    void ThreadWait(){
      pthread_cond_wait(&cond, &lk);
    }
    void ThreadWakeUp(){
      pthread_cond_signal(&cond);
    }
    //单例模式，构造函数私有
    ThreadPool(int n = NUM)
    :num(n)
    {}

  public:
    static ThreadPool* GetInstance(){
      if(nullptr == tp){
        pthread_mutex_lock(&mt);
        if(nullptr == tp){
          tp = new ThreadPool();
        }
        pthread_mutex_unlock(&mt);
      }
      return tp;
      
    }
    static void* Routine(void *args){
      ThreadPool* this_p = (ThreadPool *)args;
      while(true){
        this_p->ThreadLock();
        while(this_p->IsEmpty()){
          std::cout<<pthread_self()<<"################"<<std::endl;
          this_p->ThreadWait();
          std::cout<<pthread_self()<<"****************"<<std::endl;
        }
        Task *tk =  this_p->PopTask();
        this_p->ThreadUnlock();
        tk->Run();
        std::cout<<pthread_self()<<"^^^^^^^^^^^^^^^^^^^^"<<std::endl;
        delete tk;
      }
    }

    void InitThreadPool(){
      pthread_mutex_init(&lk, nullptr);
      pthread_cond_init(&cond, nullptr);
      
      pthread_t tid;
      for(int i=0; i < num; i++){
        pthread_create(&tid, nullptr, Routine, this);
        //线程分离
        pthread_detach(tid);
      }

    }
    void PushTask(Task* in){
      ThreadLock();
      std::cout<<"Put Task Succes"<<std::endl;
      tq.push(in);
      ThreadUnlock();
      ThreadWakeUp();
    }
    Task* PopTask(){
      Task *tk = tq.front();
      tq.pop();
      return tk;
      
    }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&lk);
      pthread_cond_destroy(&cond);
    }

};
ThreadPool* ThreadPool::tp = nullptr;
pthread_mutex_t ThreadPool::mt = PTHREAD_MUTEX_INITIALIZER;
