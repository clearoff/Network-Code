#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "sniffer.h"
#include "tools.h"


void SignalWhiteNow(int signum)
{
	printf("Use end,Bye!\n");
}

int exec_cmd(char* buf,int len)
{
	if(strncasecmp(buf,"quit",4) == 0)
		return 1;
	return 0;
}

int command_interpreter(int sd)
{
	int len;
	char buf[512];

	len=read(0,buf,512);
	if(len > 0){
		if(exec_cmd(buf,len)==1)
			return 1;
	}
	return 0;
}

void display_time_and_date()
{
	INITCOLOR(RED_COLOR);
	printf("[%s]",__DATE__);
	INITCOLOR(GREEN_COLOR);
	printf("[%s]",__TIME__);
	INITCOLOR(ZERO_COLOR);
}

void getting_started()
{
	CLEARSCREEN();
	display_time_and_date();
	printf("Getting start of Network sniffer\n\n");
}

void ProcessPacket(unsigned char* buf,int size,t_sniffer *sniffer)
{
	buf=buf+6+6+3;

	struct iphdr *iph=(struct iphdr *)buf;
	++sniffer->prot->total;

	switch(iph->protocol)
	{
		case 1:
			++sniffer->prot->icmp;
			print_icmp_packet(buf,size,sniffer);
			break;
		case 2:
			++sniffer->prot->igmp;
			break;
		case 6:
			++sniffer->prot->tcp;
			print_tcp_packet(buf,size,sniffer);
			break;
		case 17:
			++sniffer->prot->udp;
			print_udp_packet(buf,size,sniffer);
			break;
		default:
			++sniffer->prot->others;
			break;
	}
	display_time_and_date();

	printf("TCP:%d	UDP:%d	ICMP:%d\
			IGMP:%d	Others:%d	Total:%d\n",\
			sniffer->prot->tcp,sniffer->prot->udp,\
			sniffer->prot->icmp,sniffer->prot->igmp,\
			sniffer->prot->others,sniffer->prot->total);
}



int main()
{
	int sd;
	int res;
	int saddr_size;
	int data_size;
	struct sockaddr_in saddr;
	unsigned char *buf;	//保存数据包的数据
	t_sniffer sniffer; //保存数据包的类型和日志文件等信息
	fd_set fd_read;    //多路I/O复用，fd_set
	
	buf=(unsigned char*)malloc(sizeof(unsigned char* )* 65536);
	
	//下面以可写的方式打开当前文件夹中创建日志的文件
	sniffer.logfile=fopen("log.txt","wb");
	fprintf(sniffer.logfile,"***LOGFILE(%s - %s)***\n",__DATE__,\
			__TIME__);
	if(sniffer.logfile == NULL){
		perror("fopen()");
		exit(1);
	}

	sniffer.prot=malloc(sizeof(t_protocol*));
	//下面创建原生套接字
	//ETH_P_ALL表示侦听负载为IP数据报的以太网帧
	sd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	if( sd < 0){
		perror("socket()");
		exit(2);
	}
	getting_started();
	signal(SIGINT,SignalWhiteNow);
	signal(SIGQUIT,SignalWhiteNow);

	//下面循环通过select侦听以太网帧
	
	while(1)
	{
		FD_ZERO(&fd_read);
		FD_SET(0,&fd_read);
		FD_SET(sd,&fd_read);

		//多路复用检测可读的套接字和标准输入
		res=select(sd+1,&fd_read,NULL,NULL,NULL);
		if(res < 0){
			close(sd);
			if(errno != EINTR)
				perror("select()");
			return 1;
		}
		else{
			if(FD_ISSET(0, &fd_read))
			{
				if(command_interpreter(sd) ==1)
					break;
			}
			else if(FD_ISSET(sd,&fd_read))
			{
				// read mac 
				saddr_size = sizeof(saddr);
				data_size=recvfrom(sd,buf,65536,0,\
						(struct sockaddr *)&saddr,(socklen_t*)&saddr_size);
				if( data_size <=0 )
				{
					close(sd);
					perror("recvfrom()");
					return 1;
				}
				printf("Debug: 1\n");
				ProcessPacket(buf,data_size,&sniffer);
			}
		}
	}

	close(sd);
	return 0;
}
