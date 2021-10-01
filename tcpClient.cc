#include "tcpClient.hpp"

void Notice(std::string str){
  std::cout<<"Notice"<<std::endl;
  std::cout<<"\t"<<str<<" sev_ip sev_port"<<std::endl;
}

int main(int argc, char *argv[]){
  if(argc !=3){
    Notice(argv[0]);
    exit(1);
  }
  tcpClient *c = new tcpClient(argv[1], atoi(argv[2]));
  c->initClient();
  c->start();
  delete c;

  return 0;
}
