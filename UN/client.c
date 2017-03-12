/*************************************************************************
	> File Name: client.c
	> Author: LZH
	> Mail: 597995302@qq.com 
	> Created Time: Sun 12 Mar 2017 06:48:18 AM PDT
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main(int argc,char* argv[])
{
	int sock = socket(AF_LOCAL,SOCK_STREAM,0);
	if(sock < 0){
		perror("socker...\n");
		return 1;
	}

	struct sockaddr_un peer;
	peer.sun_family=AF_LOCAL;
	strcpy(peer.sun_path,argv[1]);
	int ret = connect(sock,(struct sockaddr*)&peer,sizeof(peer));
	if(ret < 0){
		perror("connect...\n");
		return 2;
	}
	char buf[1024];
	while(1){
		memset(buf,0,sizeof(buf));
		ssize_t _s = read(0,buf,sizeof(buf));
		if(_s < 0){
			perror("read...\n");
			break;
		}
		else if(_s == 0){
			printf("server is quit ...\n");
			break;
		}
		ssize_t size = write(sock,buf,_s);
		printf("size:%d\n",size);
	}
	
	return 0;
}
