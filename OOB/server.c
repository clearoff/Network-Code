#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

int listen_sock,confd;


// socket(int domain, int type, int protocol);
//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
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

//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void SigURGRun(int arg)
{
	printf("receive a OOB msg\n");
	char buf[1024];
	ssize_t _s = recv(confd,buf,sizeof(buf)-1,MSG_OOB);
	buf[_s-1]=0;
	printf("read %s OOB bytes\n",buf);
}

int main(int argc,char* argv[])
{
	if(argc!=3){
		printf("Usg: %s [ip] [port]\n",argv[0]);
		return 3;
	}
	signal(SIGURG,SigURGRun);
	printf("hello servet..\n");
	listen_sock=StartUp(argv[1],argv[2]);
	printf("listen_sock:%d\n",listen_sock);
	int id=fork();
	if(id==0){
		printf("this is child...%d\n",getpid());
		//close(listen_sock);
		struct sockaddr_in addr;
		socklen_t Len=0;
		while(1){
			confd = accept(listen_sock,(struct sockaddr*)&addr,&Len);
			printf("It's %s address send message\n",\
					inet_ntoa(addr.sin_addr));
			if(confd < 0){
				perror("accept...\n");
				return 2;
			}
			else if(confd==0){
				printf("client is quit...\n");
			}
			//set own proc
			fcntl(confd,F_SETOWN,getpid());
			char buf[1024];
			while(1){
				//ssize_t _s=read(fd,buf,sizeof(buf));
				ssize_t _s = recv(confd,buf,sizeof(buf)-1,0);
				if(_s==0){
					printf("client is quit..\n");
					break;
				}
				else if(_s<0){
					perror("read...");
					exit(1);
				}
				buf[_s]='\0';
				printf("client #");
				fflush(stdout);
				printf("%s\n",buf);
			}
		}
	}
	else{
		//father proc
		wait(NULL);
	}

	return 0;
}
