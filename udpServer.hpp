#pragma once 
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


class udpServer{
public:
	//udpServer(/*const char *ip = "127.0.0.1", */int port = 8080)
	udpServer(int port = 8080)
		:/*_ip(ip)*/
		_port(port)
	{}
	void InitServer(){
		//创建套接字
		_sock = socket(AF_INET, SOCK_DGRAM, 0);
		//用struct sockaddr_in保存ipv4的套接字
		struct sockaddr_in local;
		local.sin_family = AF_INET;
		local.sin_port = htons(_port);//端口号，字节序转网络字节序
		//local.sin_addr.s_addr = inet_addr(_ip.c_str());//string装C语言字符串，在装整数，在转字节序
		local.sin_addr.s_addr = INADDR_ANY;//string装C语言字符串，在装整数，在转字节序,不需要绑定固定的IP了
		//绑定,注意强转,将创建的套接字和文件绑定
		if (bind(_sock, (struct sockaddr *)&local, sizeof(local)) < 0){
			std::cerr << "bind error" << std::endl;
			exit(1);
		}

		_dict.insert(std::make_pair("up", "上"));
		_dict.insert(std::make_pair("down", "下"));
		_dict.insert(std::make_pair("left", "左"));
		_dict.insert(std::make_pair("right", "右"));
	}

	void Start(){
		while (1){
			char buf[64];
			struct sockaddr_in peer;
			socklen_t len = sizeof(peer);
			//从网络读
			ssize_t n = recvfrom(_sock, buf, sizeof(buf), 0, (struct sockaddr *)&peer, &len);
			if (n > 0){
				//获得客户端IP和port
				std::string cli;
				cli += inet_ntoa(peer.sin_addr);//先转成主机序列，转字符串，点分十进制形式
				cli += ':';
				char tmp[16];
				sprintf(tmp, "%d", ntohs(peer.sin_port));//转成主机序列，再装字符串
				cli += tmp;
				buf[n] = '\0';
				std::cout << cli.c_str() << "#" << buf << std::endl;

				std::string s = "unkonw";
				if (_dict.find(buf) != _dict.end()){
					s = _dict[buf];
				}
				std::string str;
				//str += "Server send #";
				str += s;
				str += '\0';
				//发送
				sendto(_sock, str.c_str(), str.size(), 0, (struct sockaddr *)&peer, len);

			}

		}

	}

	~udpServer(){
		close(_sock);

	}


private:
	//std::string _ip;//IP地址
	int _port;//端口号
	int _sock;//创建套接字返回的文件描述符
	std::map<std::string, std::string> _dict;//字典

};
