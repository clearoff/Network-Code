#include <stdio.h>
#include <assert.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

int StartUp(char* ip,char* port)
{
	assert(ip);
	assert(port);
	int sock=socket(AF_INET,SOCK_STREAM,0);
	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
	if(sock < 0){
		perror("socket...");
		exit(1);
	}
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0){
		perror("bind...\n");
		exit(2);
	}

	if(listen(sock,5) < 0){
		perror("listen...\n");
		exit(3);
	}
	return sock;
}

typedef struct log{
	int sock;
	int port;
	char* ip;
}log;

void* handleClient(void* arg)
{
	assert(arg);
	//int sock=(*(int*)arg);
	int sock=((log*)arg)->sock;
	char* ip=((log*)arg)->ip;
	int port=((log*)arg)->port;

	char buf[1024];
	while(1){
		ssize_t _s = read(sock,buf,sizeof(buf)-1);
		if(_s < 0){
			perror("read...\n");
			break;
		}
		else if(_s == 0){
			printf("client(%s:%d) is quit...\n",ip,port);
			break;
		}
		buf[_s]='\0';
		printf("client # %s\n",buf);
		write(sock,buf,_s);
	}

}

int main(int argc,char* argv[])
{
	if(argc!=3){
		printf("Usage: %s [localIp] [localPort]\n",argv[0]);
		return 3;
	}
	int listen_sock=StartUp(argv[1],argv[2]);

	while(1){
		struct sockaddr_in peer;
		socklen_t Len;
		int sock=accept(listen_sock,(struct sockaddr*)&peer,&Len);
		if(sock < 0){
			perror("accept...\n");
			continue;
		}
		printf("Get a client,address: %s:%d\n",inet_ntoa(peer.sin_addr),\
				ntohs(peer.sin_port));
		pthread_t tid;
		log arg={sock,ntohs(peer.sin_port),inet_ntoa(peer.sin_addr)};
		int ret=pthread_create(&tid,NULL,handleClient,&arg);
	}
	return 0;
}
