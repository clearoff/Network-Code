#include "udpClient.h"

using namespace std;
UdpClient::UdpClient(const std::string& ip,const int& port)
	:peer_ip(ip)
	,peer_port(port)
{}

void UdpClient::Init()
{
	peer_sock = socket(AF_INET,SOCK_DGRAM,0);	
	if(peer_sock < 0){
		//PrintLog();
		exit(1);
	}

//	struct sockaddr_in local;
//	local.sin_family = AF_INET;
//	local.sin_port = htons(peer_port); 
//	local.sin_addr.s_addr = inet_addr(peer_ip.c_str()); 
//	if(bind(_sock,(struct sockaddr*)&local,sizeof(local)) < 0){
//		//PrintLog();	
//		exit(2);
//	}
	cout<<"socket is creat success"<<endl;
}

int UdpClient::RecvData(std::string& outmsg)
{
	char buf[1024];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	ssize_t _s=recvfrom(peer_sock, buf, sizeof(buf)-1, 0,\
		(struct sockaddr*)&peer, &len);
	if(_s < 0){
		return -1;
	}
	buf[_s] = '\0';
	outmsg = buf;
	return _s;
}

int UdpClient::SendData(std::string& inmsg)
{	
	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(peer_port);
	peer.sin_addr.s_addr = inet_addr(peer_ip.c_str());
	//Json::Value val;
	//DataType::StrToValue(inmsg,val);
	//DataType people("fly pig","SUST","None",val["cmd"].asString());
	//std::string msg;
	//people.ValToString(msg);
	ssize_t _s=sendto(peer_sock, inmsg.c_str(),inmsg.size(), 0,\
			(struct sockaddr*)&peer, sizeof(peer));
	if(_s < 0){
		cout<<"Sendto error"<<endl;
		return -1;
	}
	return 0;

}

UdpClient::~UdpClient()
{
	cout<<"The Client is quit"<<endl;
}
