#pragma once 
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


class udpClient{
public:
	//初始化的是服务器的套接字
	udpClient(const char *ip = "127.0.0.1", int port = 8080)
		:_ip(ip)
		, _port(port)
	{}
	void InitClient(){
		//创建套接字
		_sock = socket(AF_INET, SOCK_DGRAM, 0);
		//不需要绑定，服务器是被动的，不会主动找客户端，当客户端打开时，服务器就能找到客户端了。
		//用struct sockaddr_in保存ipv4的套接字
		//struct sockaddr_in local;
		//local.sin_family = AF_INET;
		//local.sin_port = htons(_port);//端口号，字节序转网络字节序
		//local.sin_addr.s_addr = inet_addr(_ip.c_str());//string装C语言字符串，在装整数，在转字节序
		////绑定,注意强转,将创建的套接字和文件绑定
		//if(bind(_sock,(struct sockaddr *)&local, sizeof(local)) < 0){
		//  std::cerr<<"bind error"<<std::endl;     
		//  exit(1);
		//}
	}

	void Start(){
		struct sockaddr_in peer;
		peer.sin_family = AF_INET;
		peer.sin_port = htons(_port);
		peer.sin_addr.s_addr = inet_addr(_ip.c_str());
		while (1){
			//发送
			std::string msg;
			std::cout << "please enter #";
			getline(std::cin, msg);
			if (msg == "quit"){
				break;
			}
			msg += '\0';
			sendto(_sock, msg.c_str(), msg.size(), 0, (struct sockaddr *)&peer, sizeof(peer));
			char temp[128];
			recvfrom(_sock, temp, sizeof(temp), 0, nullptr, nullptr);
			std::cout << temp << std::endl;
		}

	}

	~udpClient(){

	}


private:
	std::string _ip;//IP地址
	int _port;//端口号
	int _sock;//创建套接字返回的文件描述符

};
