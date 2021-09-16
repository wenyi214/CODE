#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include"com.h"

int main(){
  key_t k = ftok(PATHNAME,PROJ_ID);
  if(k==-1){
    perror("ftok error");
    return 1;
  }
  //服务器关不要加IPC_EXCL，已存在不要保存，直接返回
  int shmid = shmget(k,SIZE,IPC_CREAT | IPC_EXCL | 0666);
  if(shmid == -1){
    perror("shmget error");
    return 1;
  }
  //与进程地址空间产生关联
  char *str = (char *)shmat(shmid, NULL, 0);
  //读数据
  while(1){
    printf("%s\n",str);
    sleep(1);
  }
  //删除关联
  shmdt(str);
  int sh = shmctl(shmid,IPC_RMID,NULL);
  if(sh == -1){
    perror("shmctl");
    return 1;
  }
  return 0;
}
