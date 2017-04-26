#include "udpServer.h"

UdpServer::UdpServer(const std::string ip,const int port):
	_port(port),
	_ip(ip),
	pool()
{
	cout<<"UdpServer is start"<<endl;
	cout<<"IP:"<<_ip<<endl;
	cout<<"port:"<<_port<<endl;
}

void UdpServer::Init()
{
	_sock = socket(AF_INET,SOCK_DGRAM,0);	
	if(_sock < 0){
		//PrintLog();
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port); 
	local.sin_addr.s_addr = inet_addr(_ip.c_str()); 
	if(bind(_sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		//PrintLog();	
		exit(2);
	}
	cout<<"socket is creat success"<<endl;
}

bool UdpServer::RecvData(std::string& outmsg)
{
	char buf[1024];
	struct sockaddr_in remote;
	socklen_t len=sizeof(remote);
	ssize_t _s = recvfrom(_sock,buf,sizeof(buf),0,(struct sockaddr*)\
			&remote, &len);
	if(_s <= 0){
		//PrintLog();
		cout<<"recvfrom error"<<endl;
		return false;
	}
	buf[_s]='\0';
	outmsg = buf;
	// insert client user into userlist
	userlist.insert(pair<in_addr_t,struct sockaddr_in>\
			(remote.sin_addr.s_addr,remote));
	pool.PutData(buf);
	return true;
}

bool UdpServer::broadCast(std::string& outmsg)
{
	map<in_addr_t,struct sockaddr_in>::iterator it;
	it = userlist.begin();
	ssize_t _s;
	while(it!=userlist.end())
	{
		_s = sendData(outmsg,&it->second,sizeof(it->second));
		if(_s < 0){
			//PrintLog();
			cout<<"sendData error"<<endl;
			return false;
			//exit(1);
		}
	}
	return true;
}

int UdpServer::sendData(std::string& outmsg,struct sockaddr_in* remote,socklen_t len)
{
	pool.GetData(outmsg);
	size_t _s;
	_s=sendto(_sock,outmsg.c_str(),outmsg.size(),0,\
		(struct sockaddr*)remote ,len);	
	return _s;
}

UdpServer::~UdpServer()
{
	cout<<"The server is quit"<<endl;
}
