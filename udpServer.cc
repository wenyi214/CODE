#include"udpServer.hpp"

void  Direction(char *str){
  std::cout<<"Please enter"<<str<<" server_port"<<std::endl;
}

int main(int argc, char *argv[]){
  if(argc != 2){
    Direction(argv[0]);
    exit(1);
  }
  udpServer *s = new udpServer(atoi(argv[1]));
  s->InitServer();
  s->Start();

  return 0;
}
