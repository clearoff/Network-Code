#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <string.h>

#define USER_LIMIT 35
#define _SIZE_ 1024

typedef struct epbuf{
	int fd;
	char buf[_SIZE_];
} epbuf_t, *epbuf_p;

static epbuf_p Allocate(int NewSock)
{
	epbuf_p buf = (epbuf_p)malloc(sizeof(epbuf_t));
	if(buf==NULL){
		perror("malloc");
		exit(1);
	}
	buf->fd = NewSock;
	return buf;
}

static void DelBuf( epbuf_p delbuf)
{
	if(delbuf!=NULL){
		free(delbuf);
	}
	delbuf = NULL;
}

int StartUp(char* ip,char* port)
{
     //listen_sock
    int sock=socket(AF_INET,SOCK_STREAM,0);
	printf("sock:%d\n",sock);
	int opt=1;
	int ret=setsockopt(sock,SOL_SOCKET,SO_REUSEADDR\
			,&opt,sizeof(opt));
	if(ret < 0){
		perror("setsockopt...\n");
		return 4;
	}
	 if(sock < 0){
		 perror("socket...");
		 exit(-1);
	 }
	 struct sockaddr_in sockaddr;
	 sockaddr.sin_family=AF_INET;
	 sockaddr.sin_port=htons(atoi(port));   //16 bit port
	 sockaddr.sin_addr.s_addr=inet_addr(ip);
	 inet_aton(ip,&sockaddr.sin_addr);
	 if(bind(sock,(struct sockaddr*)&sockaddr,sizeof(sockaddr))<0){
		perror("bind..\n");
		exit(-2);
	 }
	 struct sockaddr_in addrtmp;
	 socklen_t Len;
	 getsockname(sock,(struct sockaddr*)&addrtmp,&Len);
	 printf("com:%d\n",ntohs(addrtmp.sin_port));

	 if(listen(sock,5)<0){
		perror("listen...\n");
		exit(-3);
	 }
		
	 return sock;
}


int main(int argc,char* argv[])
{
	if(argc != 3){
		printf("Useage: %s [peer_ip] [peer_local]");
		return 1;
	}
	
	int listen_sock = StartUp(argv[1],argv[2]);
	int exfd = epoll_create(256);
	if( exfd < 0){
		perror("epoll_create...");
		close(listen_sock);
		return 2;
	}
	//int MaxEvents;
	int timeout = -1;
	struct epoll_event evs[USER_LIMIT];
	struct epoll_event ev;
	//ev.data.fd = listen_sock;
	ev.events = EPOLLIN;
	ev.data.ptr = Allocate(listen_sock);
	int ret = epoll_ctl( exfd, EPOLL_CTL_ADD, listen_sock, &ev);

	if( ret < 0){
		perror("epoll_ctl...");
		return 3;
	}

	while(1){
		int num = 0;
		switch(num = epoll_wait( exfd, evs, USER_LIMIT,timeout)){
			case 0:
				printf(" time out...\n");
				break;
			case -1:
				perror("epoll_wait...");
				exit(0);
				break;
			default:
				{
					int i = 0;
					for(;i< num;i++){
						int fd = ((epbuf_p)(evs[i].data.ptr))->fd;
						//printf("debug 1 : fd=%d\n",fd);
						//printf("result:%d\n",evs[i].events & EPOLLIN);
						if((fd == listen_sock)&&(evs[i].events & EPOLLIN))
						{
							//printf(" debug 2\n");
							struct sockaddr_in peer;
							socklen_t len = sizeof(peer);
							int confd;
							if((confd = accept(listen_sock,(struct\
											sockaddr*)&peer,&len)) < 0)
							{
								perror("accept...\n");
								continue;
							}//if
							// accept success
							printf("Get a client,%s:%d\n",\
									inet_ntoa(peer.sin_addr),\
									ntohs(peer.sin_port));
							ev.data.ptr = Allocate(confd);
							ev.events = EPOLLIN | EPOLLET;
							if( epoll_ctl(exfd,EPOLL_CTL_ADD,confd,\
										&ev) < 0)
							{
								perror("epoll_ctl");
								exit(6);
							}
						}//if
						else if((fd!=listen_sock)&&(evs[i].events & EPOLLIN))
						{
							// read event come	
							//printf("fd=%d\n",fd);
							char* ReadBuf=((epbuf_p)(evs[i].data.ptr))->buf;
							int _s = read(fd, ReadBuf, _SIZE_-1);
							if( _s < 0){
								perror("read...");
								//DelBuf((epbuf_p)(evs[i].data.ptr));
								//close(fd);
								continue;
							}else if(_s ==0){
								printf("A client is quit...\n");
								DelBuf((epbuf_p)(evs[i].data.ptr));
								close(fd);
								continue;
							}else{
								//printf("_s %d\n",_s);
								((epbuf_p)(evs[i].data.ptr))->buf[_s-1] = 0;
								printf("client #%s\n",((epbuf_p)(evs[i].data.ptr))->buf);
								
								//evs[i].events = EPOLLOUT;
								((epbuf_p)(ev.data.ptr))->fd=fd;
								ev.events = evs[i].events|EPOLLOUT|EPOLLET;
								if(epoll_ctl( exfd,EPOLL_CTL_MOD, fd, &ev\
											) < 0){
									perror("epoll_ctl");
									exit(1);
								}
							}
						}
						else if((fd!=listen_sock)&&(evs[i].events & EPOLLOUT)){
								printf("Debug 1\n");
								const char* msg ="http/1.0 200 ok\r\n\r\n<html><h1>Hello World...</h1></html>";
								printf("%d\n",strlen(msg));
								ssize_t _s = write(fd,msg,strlen(msg));
								//DelBuf((epbuf_p)(evs[i].data.ptr));
								//close(fd);
						}
					}//for
				}//default
				break;
		}
	}
	return 0;
}
