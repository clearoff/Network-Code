#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>

//int socket(int domain, int type, int protocol);
//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
void SigpipeRun(int signo)
{
	printf("this is in client SigpipeRun..,signo:%d\n",signo);
	sleep(2);
}

int main(int argc,char* argv[])
{
	signal(SIGPIPE,SigpipeRun);
	if(argc!=3){
		printf("Usegs: %s [ip] [port]",argv[1]);
		return 3;
	}
	printf("Hello client\n");
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("socket..");
		exit(1);
	}
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(atoi(argv[2]));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0){
		perror("connect...");
		return 2;
	}
	printf("connect success...\n");
	char buf[1024];
	int count=10;
	while(1){
		ssize_t _s=read(0,buf,1024);
		if(_s<0){
			perror("read...\n");
			exit(1);
		}
		buf[_s-1]='\0';
		printf("client #");
		fflush(stdout);
		printf("%s\n",buf);
		//write(sock,buf,_s);
		if(send(sock,buf,_s,MSG_OOB) < 0){
			perror("send");
			continue;
		}
	}
	close(sock);
	return 0;
}
