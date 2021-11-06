#include"HttpServer.hpp"

void Usage(std::string arg){
  std::cout<<"Usage1: "<< arg <<std::endl;
  std::cout<<"Usage2: "<< arg <<"+ port"<<std::endl;
}

int main(int argc, char *argv[]){
  if(argc != 1 && argc != 2){
    std::cout<<argc<<std::endl;
    Usage(argv[0]);
    exit(0);
  }
  //daemon(1,0);
  httpserver *hp;
  if(argc == 1){
    hp = new httpserver();
  }
  else{
    hp = new httpserver(atoi(argv[1]));
  }
  hp->InitServer();
  hp->Start();
  delete hp;
  return 0;
}
