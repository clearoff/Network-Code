#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_
#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <map>
#include "../pool/pool.h"

class UdpServer{
	private:
		int _sock;
		int _port;
		std::string _ip;
		map<in_addr_t,struct sockaddr_in> userlist;
		Pool pool;
	private:
		UdpServer(const UdpServer&);
	public:
		UdpServer(const std::string ip,const int port);
		void Init();
		bool RecvData(std::string&);
		bool broadCast(std::string&);
		int sendData(std::string&,struct sockaddr_in*,socklen_t);
		~UdpServer();
};

#endif //_UDP_SERVER_H_
