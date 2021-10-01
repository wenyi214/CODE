#include "tcpServer.hpp"

void Notice(std::string str){
  std::cout<<str<<std::endl;
  std::cout<<"\t"<<str<<" server_port"<<std::endl;

}
int main(int argc, char *argv[]){
  if(argc != 2){
    Notice(argv[0]);
    exit(1);
  }
  tcpServer *s = new tcpServer(atoi(argv[1]));
  s->initServer();
  s->start();

  delete s;

  return 0;
}
