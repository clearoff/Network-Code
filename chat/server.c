#define _GNU_SOURCE 1
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>



#define BUF_SIZE 1024
#define FD_LIMIT 10
#define USER_LIMIT 5

typedef struct client_data{
	struct sockaddr_in address;
	char* write_buf;
	char buf[1024];
} client_data;

int SetUnBlock( int fd)
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl( fd,F_SETFL,new_option);
	return old_option;
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
	if( argc!=3){
		printf("Usege: %s [local_ip] [local_port]\n",argv[0]);
		return 1;
	}
	printf("Server is start\n");
	int ret;
	int listen_sock = StartUp(argv[1] , argv[2]);
	char tmpbuf[1024];

	client_data* users = (client_data*)malloc(sizeof(client_data)*\
			USER_LIMIT);
	if(users==NULL){
		perror("malloc");
		exit(1);
	}
	struct pollfd fds[USER_LIMIT+1];
	int user_count = 0;

	// init fds
	int i=1;
	for(i=1; i<=USER_LIMIT; i++)
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		fds[i].revents = 0;
	}
	fds[0].fd = listen_sock;
	fds[0].events = POLLIN | POLLERR;
	fds[0].revents = 0;
	
	while(1){
		ret = poll( fds, user_count+1, -1);
		if(ret < 0){
			perror("poll...");
			break;
		}

		int i=0;
		for(i=0; i<user_count+1;i++){
			//
			if((fds[i].fd == listen_sock)&&( fds[i].revents & POLLIN )){
				struct sockaddr_in client_address;
				socklen_t client_addrlen = sizeof(client_address);

				int connfd = accept(listen_sock,(struct sockaddr*)\
						&client_address, &client_addrlen);
				if(connfd < 0){
					printf("errno : %d\n",errno);
					continue;
				}

				if( user_count >= USER_LIMIT){
					const char* info="clients is limit..\n";
					printf("Too many clients...\n");
					ssize_t size = write(connfd,info,sizeof(info)-1);
					if (size < 0){
						perror("write...");
					}
					close( connfd);
					continue;
				}

				user_count++;
				users[connfd].address = client_address;
				SetUnBlock(connfd);
				fds[user_count].fd = connfd;
				fds[user_count].events = POLLIN|POLLRDHUP|POLLERR;
				fds[user_count].revents = 0;
				printf("A new user coming,now have %d users\n",user_count);
			}
			//处理某个客户的出错情况，抛出错误码
			else if(fds[i].revents & POLLERR){
				printf("Get an error from %d user\n",fds[i].fd);
				char error[100];
				memset(error,0,100);
				socklen_t len=sizeof(error);
				if(getsockopt(fds[i].fd,SOL_SOCKET,SO_ERROR,&error,\
							&len) < 0){
					perror("getsockopt...\n");
					continue;
				}
			}
			//有客户关闭连接
			else if(fds[i].revents & POLLRDHUP){
				users[fds[i].fd]=users[fds[user_count].fd];
				close( fds[i].fd);
				fds[i] = fds[user_count];
				i--;
				user_count--;
				printf("A user is left...\n");
			}
			//处理客户的输入
			else if(fds[i].revents & POLLIN){
				int connfd = fds[i].fd;
				memset(users[connfd].buf,0,BUF_SIZE);
				ssize_t _s = read(connfd,users[connfd].buf,BUF_SIZE-1);
				if(_s < 0){
					perror("read...\n");
					users[fds[i].fd]=users[fds[user_count].fd];
					fds[i] = fds[user_count];
					i--;
					user_count--;
					continue;
				}
				else if(_s == 0){
					printf("User %d is quit...\n",connfd-2);
				}
				users[connfd].buf[_s-1]='\0';
				printf("User %d # %s\n",connfd,users[connfd].buf);
				int j=1;
				for(j=1;j<=user_count;j++){
					if(fds[j].fd==connfd){
						//服务器不向发出此数据的客户端发送数据
						continue;
					}
					fds[j].events |=~POLLIN;
					fds[j].events |=POLLOUT;  //OUT is enable
					//users[fds[j].fd].write_buf = "Users :";
					int k=0;
					for(k=0;k<sizeof("User :");k++){
						tmpbuf[k]="User :"[k];
						if( k == 4){
							tmpbuf[k] = fds[j].fd + '0';
						}
					}
					int t=0;
					printf("users buf:%s\n",users[connfd].buf);
					for(t=0;t < _s-1;t++,k++){
						tmpbuf[k-1] = users[connfd].buf[t];
					}
					users[fds[j].fd].write_buf = tmpbuf; 
						//users[connfd].buf;
				}
			}
			//服务器向对方客户端输入数据
			else if(fds[i].revents & POLLOUT){
				int connfd = fds[i].fd;
				if(!users[connfd].write_buf){
					continue;
				}
				//printf("write_buf:%s\n",users[fds[i].fd].write_buf);
				ret=write(connfd,users[connfd].write_buf,\
						strlen(users[connfd].write_buf));
				users[connfd].write_buf=NULL;
				fds[i].events |= ~POLLOUT;
				fds[i].events |= POLLIN;
			}
		}
	}
	//delete[] users;
	free(users);
	users=NULL;
	close(listen_sock);
	return 0;
}
