#include "server.hpp"

int main(int argc, char *argv[]){
  if(argc != 2){
    std::cout<<"please enter argv[0] server_port"<<std::endl;
    exit(1);
  }
  Server *s = new Server(atoi(argv[1]));
  s->initServer();
  s->Start();

  return 0;
}
