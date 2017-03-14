#define _GNU_SOURCE 1
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>



static void Usege(char* proc)
{
	assert(proc);
	printf("usege: %s [peer_ip] [peer_port]\n",proc);
}

int main(int argc,char* argv[])
{
	if(argc!=3){
		Usege(argv[0]);
		return 1;
	}
	printf("Client lanuched...\n");
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if( sockfd < 0){
		perror("socket");
		return 2;
	}

	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(atoi(argv[2]));
	peer.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(sockfd,(struct sockaddr*)&peer,sizeof(peer)) < 0){
		perror("connect");
		return 3;
	}

	printf("Connect server success...\n");
	struct pollfd fds[2]; //fds[0]----> stdout fds[1]---->sockfd
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	fds[1].fd = sockfd;
	fds[1].events = POLLIN|POLLRDHUP;
	fds[1].revents = 0;

	char ReadBuf[1024];
	
	while(1){
		int ret = poll(fds, sizeof(fds)/sizeof(fds[0]), -1);
		if(ret < 0){
			perror("poll...");
			break;
		}

		if( fds[1].revents & POLLRDHUP){
			printf(" server is quit...\n");
			break;
		}
		else if( fds[1].revents & POLLIN){
			// data is enable to read 
			memset(ReadBuf,0,1024);
			ssize_t _s = read(fds[1].fd, ReadBuf, sizeof(ReadBuf)-1);
			printf("Debug:%d\n",_s);
			if(_s < 0){
				perror("read from server...");
				break;
			}
			else if(_s == 0 ){
				printf("server is quit...\n");
				break;
			}
			ReadBuf[_s]='\0';
			printf("Read from server #");
			fflush(stdout);
			printf("%s\n",ReadBuf);
		}
		
		if( fds[0].revents & POLLIN){
			memset( ReadBuf, 0, sizeof(ReadBuf));
			ssize_t _s = read( 0, ReadBuf, sizeof(ReadBuf)-1);
			if( _s < 0){
				perror("Input error...\n");
				break;
			}
			else{
				write( fds[1].fd, ReadBuf, _s);
			}
		}
	}
	close(sockfd);
	return 0;
}
