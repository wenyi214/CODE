#include"udpClient.hpp"

void Direction(char str[]){
  std::cout<<"Please Enter #"<<str<<" server_ip server_port"<<std::endl;
}
int main(int argc, char *argv[]){
//int main(){
  if(argc != 3){
    Direction(argv[0]);
    exit(1);
  }
  udpClient *c =new udpClient(argv[1], atoi(argv[2]));
  //udpClient *c =new udpClient();
  c->InitClient();
  c->Start();

  return 0;
}
