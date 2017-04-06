#ifndef	__SNIFFER_H__
#define	__SNIFFER_H__

#define ETH_P_IP 0x0800
//描述数据报类型的结构体
typedef struct	s_protocol
{
		int	tcp;
		int	udp;
		int	icmp;
		int	igmp;
		int	others;
		int	total;
} t_protocol;

//配置文件与数据包类型的结构体
typedef struct	s_sniffer
{
		FILE* logfile;
		t_protocol *prot;
} t_sniffer;

//数据报处理函数
void ProcessPacket(unsigned char*, int, t_sniffer *);
void print_ip_header(unsigned char* , int, t_sniffer *);
void print_tcp_packet(unsigned char* , int, t_sniffer *);
void print_udp_packet(unsigned char * , int, t_sniffer *);
void print_icmp_packet(unsigned char* , int, t_sniffer *);
void PrintData (unsigned char* , int, t_sniffer *);
void display_time_and_date();
void getting_started();

#endif
