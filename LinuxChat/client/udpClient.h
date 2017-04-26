#ifndef _UDP_Client_H_
#define _UDP_Client_H_
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>

class UdpClient{
	private:
		int peer_sock;
		int peer_port;
		std::string peer_ip;
	public:
		UdpClient();
		UdpClient(const std::string& ip,const int& port);
		int RecvData(std::string&);
		int SendData(std::string&);
		void Init();
		~UdpClient();
};

#endif //_UDP_Client_H_
