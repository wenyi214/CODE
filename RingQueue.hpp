#pragma once 
#include<iostream>
#include<vector>
#include<semaphore.h>
#include<pthread.h>
#define NUM 5

class RingQueue{
  private:
    void P(sem_t& s){
      //信号量减减操作，如果为0等待
      sem_wait(&s);
    }
    void V(sem_t& s){
      //信号量加加操作
      sem_post(&s);
    }
  public:
    RingQueue(size_t cap = NUM)
      :_v(cap)
      ,_cap(cap)
      ,_cindex(0)
      ,_pindex(0)
      {
        sem_init(&_space_sem, 0, cap);
        sem_init(&_data_sem, 0, 0);
      }

    void Put(const int& in){
      //生产者关注格子数
      P(_space_sem);
      _v[_pindex]=in;
      _pindex++;
      _pindex %= _cap;
      V(_data_sem);

    }
    void Get(int& out){
      //消费者关注数据
      P(_data_sem);
      out = _v[_cindex];
      _cindex++;
      _cindex %= _cap;
      V(_space_sem);

    }

    ~RingQueue(){
      sem_destroy(&_space_sem);
      sem_destroy(&_data_sem);
      _cindex = 0;
      _pindex = 0;
    }

  private:
    std::vector<int> _v;//队列
    size_t _cap;//队列容量
    sem_t _space_sem;//格子信号量
    sem_t _data_sem;//数据信号量
    
    int _cindex;//消费者位置
    int _pindex;//生产者位置


};
