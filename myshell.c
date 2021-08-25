//一.实现的功能
//能够执行一般的指令，如ls - a - l，ps等。
//
//支持重定向(输出重定向，追加重定向和输入重定向的功能)。
//
//二.怎么实现
//2.1.怎么实现可以执行一般的指令
//首先要获取命令行
//通过fgets命令从标准输入，获得输入的命令。
//
//解析获得的命令行
//将命令行分解，以空格作为分隔符，分割命令行用到字符串函数strtok函数。
//
//执行命令
//我们都知道，命令实际上都是一些可执行程序。系统可以直接通过环境变量PATH直接找到他们，然后执行。
//
//执行命令时，我们通过fork一个子进程去执行命令。运用进程替换exec函数，让命令的生成的进程替换子进程。
//
//在替换时用的execvp函数，因为系统可以通过自己的环境变量找到可执行程序，并且，我将分解的命令，报错在数组中。
//
//父进程的作用只是获取命令行，等待子进程的结果。
//
//2.2 怎么实现重定向的功能
//输入重定向：学了基础IO后，我们知道，标准输出的文件描述符是1，而文件描述符的本质是一个结构体指针数组的下标，这个结构体指针数组的每一个元素指向的是一个个系统描述的文件。我们只需要将需要重定向的文件文件描述符位置的内容拷贝给标准输出的文件描述符1处即可。
//
//想了解重定向原理细节的小伙伴，可以看这里：https ://blog.csdn.net/weixin_57023347/article/details/119858572?spm=1001.2014.3001.5501
//
//标准输入，标准输出的文件描述符为0，1。
//
//输出重定向：实际是将输出到标准输出上的数据，输出到文件中。只需要将重定向文件的文件描述符的内容，拷贝到文件描述符1处。文件打开方式为O_WRONLY | O_APPEND | O_CREAT
//
//追加重定向：原理和上面的一样，只是文件打开方式不同，O_WRONLY | O_APPEND | O_CREAT。
//
//输入重定向：原理一样，将重定向文件，文件描述符出的内容拷贝到文件描述符0处。文件打开方式为：O_RDONLY。


#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

#define NUM 128

void redirect(char *buf){
  int fd=0;
  while(*buf){
    //记录>个数 redirect_count=1,输出重定向，2追加重定向
    int redirect_count=0;
    if(*buf=='>'){
      *buf++='\0';
      redirect_count++;
      if(*buf=='>'){
        *buf++='\0';
        redirect_count++;
      }
      //去前面的空格
      while(*buf!='\0'&&isspace(*buf)){
        buf++;
      }
      char *file=buf;
      //去后面的空格
      while(!isspace(*buf)){
        buf++;
      }
      *buf='\0';
      //打开文件
      if(redirect_count==1){
        fd=open(file,O_WRONLY | O_TRUNC | O_CREAT,0644);
      }
      //追加重定向
      else if(redirect_count==2){
        fd=open(file,O_WRONLY | O_APPEND | O_CREAT,0644);
      }
      else{
        
      }
      //重定向
      dup2(fd,1);
      close(fd);
    }
    //输入重定向
    else if(*buf=='<'){
      *buf++='\0';
      while(*buf!='\0'&&isspace(*buf)){
        buf++;
      }
      char *file=buf;
      while(!isspace(*buf)){
        buf++;
      }
      *buf='\0';
      fd=open(file,O_RDONLY);
      dup2(fd,0);
      close(fd);
    }
    else{

    }
    buf++;
  }

}
int main(){
  const char *str="[temp@root login-user]#";
  char buf[NUM];
  char *argn[NUM];
  while(1){
    //获得字符串
    printf("%s",str);
    fgets(buf,NUM,stdin);
    //去掉最后一个\n
    buf[strlen(buf)-1]='\0';
    
    pid_t id=fork();
    if(id<0){
      perror("fork fail");
      continue;
    }
    //子进程替换
    else if(id==0){
      redirect(buf);
      //分解字符串
      argn[0]=strtok(buf," ");
      int i=0;
      while(argn[i]){
        argn[++i]=strtok(NULL," ");
      }
      execvp(argn[0],argn);
      exit(1);
    }
    int status=0;
    pid_t res=waitpid(id,&status,0);
    if(res>0){
      int code=status & 0x7f;
      if(code==0){
        printf("quit code is :%d\n",(status>>8)&0xff);
      }
      else{
        printf("error code is :%d\n",code);
      }
    }
    else{
      perror("waitpid fail\n");
    }

  
  }


  return 0;
}
