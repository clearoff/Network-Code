#include "udpServer.h"
#include "../Json/DataType.h"


//int UdpServer::GetFromRedis(std::string& out)
//{
//	redisReply* reply;
//	reply = (redisReply *)redisCommand(rds,"SELECT 1");
//	assert(reply!=NULL);
//	reply = (redisReply *)redisCommand(rds,"LRANGE session 0 -1");
//	assert(reply!=NULL);
//	return 0;
//}
//
//int UdpServer::SaveToRedis(std::string& msg)
//{
//	redisReply* reply;
//	printf("debug ...\n");
//	reply = (redisReply *)redisCommand(rds,"SELECT 1");
//	assert(reply!=NULL);	
//	freeReplyObject(reply);
//	printf("debug1...\n");
//	reply = (redisReply *)redisCommand(rds,"LPUSH session %s", msg.c_str());
//	assert(reply!=NULL);
//	printf("debug2...\n");
//	if(reply->type ==REDIS_REPLY_INTEGER){
//		freeReplyObject(reply);
//		return 0;
//	}
//	else{
//		freeReplyObject(reply);
//		cout<<reply->type<<":"<<reply->str<<endl;
//		return 1;
//	}
//	return 0;
//}

UdpServer::UdpServer(std::string ip,int port):
	_port(port),
	_ip(ip),
	pool(),
	SessionCount(0)
	//rds()
{
	cout<<"UdpServer is start"<<endl;
	cout<<"IP:"<<_ip<<endl;
	cout<<"port:"<<_port<<endl;
	//rds=redisConnect("127.0.0.1",6379);
	//if( rds == NULL || rds->err){
	//	if( rds){
	//		printf("Connect redis-server error:%s\n",rds->errstr);
	//	}
	//	else{
	//		printf("Can't allocate redis context.\n");
	//	}
	//	exit(1);
	//}
	//printf("Connect redis-server success.\n");
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

//bool UdpServer::RecvData(std::string& outmsg)
//{
//	Json::Value val;
//	char buf[1024];
//	struct sockaddr_in remote;
//	socklen_t len=sizeof(remote);
//	ssize_t _s = recvfrom(_sock,buf,sizeof(buf),0,(struct sockaddr*)\
//			&remote, &len);
//	printf("_s:%d\n",_s);
//	if(_s <= 0){
//		//PrintLog();
//		cout<<"recvfrom error"<<endl;
//		return false;
//	}
//	buf[_s]='\0';
//	std::string tmp=buf;
//	DataType::StrToValue(tmp,val);
//	std::string cmd=val["cmd"].asString();
//	outmsg = buf;
//	if(cmd=="QUIT")
//	{
//		userlist.erase(remote.sin_addr.s_addr);
//	}
//	else
//	{
//		userlist.insert(pair<in_addr_t,struct sockaddr_in>\
//			(remote.sin_addr.s_addr,remote));
//	}
//	pool.PutData(outmsg.c_str());
//	//printf("Redis ...\n");
//	//SaveToRedis(outmsg);
//	return true;
//}

bool UdpServer::RecvData(std::string& outmsg)
{
	Json::Value val;
	char buf[1024];
	buf[0]='\0';
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
	std::string tmp=buf;
	DataType::StrToValue(tmp,val);
	std::string cmd=val["cmd"].asString();
	outmsg = buf;
	if(cmd=="QUIT")
	{
		userlist.erase(remote.sin_addr.s_addr);
	}
	else
	{
		userlist.insert(pair<in_addr_t,struct sockaddr_in>\
			(remote.sin_addr.s_addr,remote));
	}
	pool.PutData(outmsg.c_str());
	//SaveToRedis(outmsg);
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
	cout<<outmsg<<endl;
	size_t _s;
	_s=sendto(_sock,outmsg.c_str(),outmsg.size(),0,\
		(struct sockaddr*)remote ,len);	
	return _s;
}

UdpServer::~UdpServer()
{
	//if(rds){
	//	redisFree(rds);
	//	rds==NULL;
	//}
	cout<<"The redis-server is quit"<<endl;
	cout<<"The server is quit"<<endl;
}
