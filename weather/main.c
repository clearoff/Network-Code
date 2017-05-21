#include "httpd.h"

static void Usege(const char* arg)
{
	printf("Usege: %s [ip] [port]\n",arg);
}

static void* AcceptRun(void* arg)
{
	int sock=(int)arg;
	//printf("sock:%d\n",sock);
	pthread_detach(pthread_self());
	return (void*)HandleSock(sock);
}

int main(int argc,char* argv[])
{
	//openlog("httpd", LOG_CONS｜LOG_PID,LOG_USER);
	if(argc!=3){
		closelog();
		Usege(argv[0]);
	}	
	int listen_sock = StartUp(argv[1],atoi(argv[2]));
//	printf("listen_sock:%d\n",listen_sock);
	while(1){
		struct sockaddr_in addr;
		socklen_t len=sizeof(addr);
		int sock;
		if((sock = accept(listen_sock,(struct sockaddr*)&addr,&len)) < 0){
			PrintLog("Server con't accept client",WARNING);
			continue;
		}
		printf("%s:%d is link server\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
		//printf("sock%d\n",sock);
		// accept success
		pthread_t pid;
		if(pthread_create(&pid,NULL,AcceptRun,(void*)sock) != 0){
			PrintLog("server can't create pthread",WARNING);
			closelog();
			close(sock);
			continue;
		}
	}//while 1
	//closelog();
	return 0;
}
