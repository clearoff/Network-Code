#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_
#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstdlib>
#include <map>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include "../pool/pool.h"

class UdpServer{
	private:
		int _sock;
		int _port;
		std::string _ip;
		map<in_addr_t,struct sockaddr_in> userlist;
	private:
		UdpServer(const UdpServer&);
	public:
		Pool pool;
		int SessionCount;
		std::vector<std::string> session;
		UdpServer(std::string ip,int port);
		void Init();
		bool RecvData(std::string&);
		bool broadCast(std::string&);
		int sendData(std::string&,struct sockaddr_in*,socklen_t);
		int SaveToRedis(std::string& msg);
		int GetFromRedis(std::string& out);
		~UdpServer();
};

#endif //_UDP_SERVER_H_
