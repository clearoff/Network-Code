//使用libevent 实现server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <event.h>
using namespace std;

struct event_base* base;	//declare a event

void onRead(int CliFd, short Event, void *arg)
{
	int Len;
	char buf[1500];

	Len = recv(CliFd, buf, 1500, 0);
	if(Len<=0){
		cout<<"CLient Close" <<endl;

		//connection over ----> to release buffer
		struct event *pEvRead = (struct event*)arg;
		event_del(pEvRead);
		delete pEvRead;


		close(CliFd);
		return;
	}

	buf[Len] = 0;
	cout<<"CLient Info:"<< buf <<endl;

}


void onAccept(int SvrFd, short Event, void* arg)
{
	int CliFd;
	struct sockaddr_in CliAddr;

	socklen_t SinSize = sizeof(CliAddr);
	CliFd = accept(SvrFd, (struct sockaddr*)&CliAddr, &SinSize);
	if(CliFd < 0){
		cout<<"客户端连接失败"<<endl;
		exit(2);
	}
	cout<<"客户端链接成功"<<endl;

	//链接注册为新事件(EV_PERSIST为事件触发后不默认删除)
	struct event *pEvRead = new event;
	event_set(pEvRead, CliFd, EV_READ|EV_PERSIST, onRead, pEvRead);
	event_base_set(base, pEvRead);
	event_add(pEvRead, NULL);
}


int main()
{
	int SvrFd;
	struct sockaddr_in SvrAddr;

	cout<<"服务端启动!"<<endl;
	memset(&SvrAddr, 0, sizeof(SvrAddr));
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SvrAddr.sin_port = htons(8000);

	SvrFd = socket(AF_INET, SOCK_STREAM, 0);
	bind(SvrFd, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
	
	if(listen(SvrFd,10)!=0){
		cout<<"Server listen error!"<<endl;
		exit(1);
	}
	

	//创建base
	base = event_base_new();
	struct event evListen;
	event_set(&evListen, SvrFd, EV_READ | EV_PERSIST, onAccept, NULL);
	event_base_set(base, &evListen);
	event_add(&evListen, NULL);

	event_base_dispatch(base);
	return 0;
}
