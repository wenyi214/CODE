#include "PollServer.hpp"
void Notice(){
  cout << "Notice:\n\t"<<"please port"<<endl;
}

int main(int argc, char* argv[]){
  if(argc != 2){
    Notice();
    exit(6);
  }
  PollServer *ps = new PollServer(atoi(argv[1]));
  ps->InitServer();
  ps->Start();
  delete ps;

  return 0;
}
