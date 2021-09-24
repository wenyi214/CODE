#include"BlockQueue.hpp"
#include<unistd.h>

#define PRO 2
#define CON 2
using namespace std;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
void *Productor(void *arg){
  sleep(1);
  blockqueue *q = (blockqueue *)arg;
  while(true){
    sleep(1);
    int x=rand()%9+1;
    int y=rand()%20+1;
    Task t(x,y);
    pthread_mutex_lock(&mutex2);
    q->Put(t);
    cout<<pthread_self()<<":"<<x<<"+"<<y<<"="<<"?"<<endl;
    pthread_mutex_unlock(&mutex2);
  }
}
void *Consumer(void *arg){
  blockqueue *q = (blockqueue *)arg;
  while(true){
    sleep(1);
    Task t;
    pthread_mutex_lock(&mutex1);
    q->Get(t);
    cout<<pthread_self()<<":"<<t._x<<"+"<<t._y<<"="<<t.Add()<<endl;
    pthread_mutex_unlock(&mutex1);
  }

}

int main(){
  pthread_mutex_init(&mutex2,nullptr);
  pthread_mutex_init(&mutex1,nullptr);
  blockqueue *bq = new blockqueue();
  pthread_t td1[PRO];
  int i=0;
  for(; i<PRO; i++){
    pthread_create(td1+i, nullptr, Productor, (void *)bq);
  }
  pthread_t td2[CON];
  for(i=0; i<CON; i++){
    pthread_create(td2+i, nullptr, Consumer, (void *)bq);
  } 
  
  for(i=0; i<PRO; i++){
    pthread_join(td1[i], nullptr);
  }
  for(i=0; i<CON; i++){
    pthread_join(td2[i], nullptr);
  }
  pthread_mutex_destroy(&mutex2);
  pthread_mutex_destroy(&mutex1);
  return 0;
}
