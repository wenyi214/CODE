#pragma once 

#include<iostream>
#include<queue>
#include<pthread.h>
#define NUM 5
struct Task{
  Task(){};
  Task(int x,int y)
    :_x(x)
    ,_y(y)
  {}
  int _x;
  int _y;
  int Add(){
    return _x + _y;
  }
};

class blockqueue{
private:
  //加锁
  void MakeLock(){
    pthread_mutex_lock(&_lock);
  }
  //取消锁
  void CancelLock(){
    pthread_mutex_unlock(&_lock);
  }
  //唤醒消费者
  void WakeUpConsumer(){
    std::cout<<"Consumer wake up"<<std::endl;
    pthread_cond_signal(&_empty);

  }
  //唤醒生产者
  void WakeUpProductor(){
    std::cout<<"Productor wake up"<<std::endl;
    pthread_cond_signal(&_full);

  }
  //生产者等待
  void SleepProductor(){
    std::cout<<"Productor sleep"<<std::endl;
    pthread_cond_wait(&_full, &_lock);

  }
  //消费者等待
  void SleepConsumer(){
    std::cout<<"Consumer sleep"<<std::endl;
    pthread_cond_wait(&_empty, &_lock);

  }

public:
  blockqueue(size_t cap = NUM)
    :_cap(cap)
  {
    pthread_mutex_init(&_lock, nullptr);
    pthread_cond_init(&_full, nullptr);
    pthread_cond_init(&_empty, nullptr);
  }
  ~blockqueue(){
    pthread_cond_destroy(&_empty);
    pthread_cond_destroy(&_full);
    pthread_mutex_destroy(&_lock);

  }
  //放数据
  void Put(Task in){
    //q队列使临界资源，需要加锁
    MakeLock();
    //需要使用循环
    while(_q.size()>=_cap){
      WakeUpConsumer();
      SleepProductor();
    }
    _q.push(in);
    CancelLock();

  }
  //获取数据
  void Get(Task& out){
    MakeLock();
    while(_q.empty()){
      WakeUpProductor();
      SleepConsumer();
    }
    out=_q.front();
    _q.pop();
    CancelLock();

  }

private:
  std::queue<Task> _q;
  size_t _cap;
  pthread_mutex_t _lock;
  pthread_cond_t _empty;//消费者在此等待
  pthread_cond_t _full;//生产者在此等待

};
