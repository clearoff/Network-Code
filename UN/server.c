/*************************************************************************
	> File Name: server.c
	> Author: LZH
	> Mail: 597995302@qq.com 
	> Created Time: Sun 12 Mar 2017 06:00:10 AM PDT
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main(int argc,char* argv[])
{
	umask(0);
	if(argc!=2){
		printf("Usage: %s [PathName]\n",argv[0]);
		return 2;
	}

	int sockfd=socket(AF_LOCAL,SOCK_STREAM,0);
	if(sockfd < 0){
		perror("socket...\n");
		return 1;
	}
	unlink(argv[1]);
	struct sockaddr_un local;
	local.sun_family=AF_LOCAL;
	strcpy(local.sun_path,argv[1]);
	if(bind(sockfd,(struct sockaddr *)&local,SUN_LEN(&local)) < 0){
		perror("bind...\n");
		return 3;
	}
	printf("bound!...\n");
	if(listen(sockfd,5) < 0){
		perror("listen...\n");
		return 4; 
	}
	
	char buf[1024];
	struct sockaddr_un peer;
	socklen_t Len=sizeof(peer);
	int confd = accept(sockfd,(struct sockaddr*)&peer,&Len);
	if(confd < 0){
		perror("accept...\n");
		return 5;
	}
	while(1){
		ssize_t _s = read(confd,buf,sizeof(buf)-1);
		if(_s < 0){
			perror("read...\n");
			break;
		}
		else if(_s == 0){
			printf("client is quit...\n");
			break;
		}
		buf[_s-1]='\0';
		printf("client # %s\n",buf);
	}
	return 0;
}
