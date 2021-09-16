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
  //不创建共享内存，只是为了得到shmid
  int shmid = shmget(k, SIZE, 0);
  if(shmid == -1){
    perror("shmget error");
    return 1;
  }
  //与进程地址空间产生关联
  char *str = (char *)shmat(shmid, NULL, 0);
  char c='a';
  for(;c<='z';c++){
    str[c-'a']=c;
    sleep(5);
  }
  //删除关联
  shmdt(str);
  //不用释放共享内存，服务器端释放

  return 0;
}
