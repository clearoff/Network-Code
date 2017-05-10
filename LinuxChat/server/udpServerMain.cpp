#include "udpServer.h"
#include "../Json/DataType.h"
#include "../redis-lib/redis_api.h"

using namespace std;

#define KEY_MAX 512

static std::string _itoa(int num)
{
	char buf[8];
	sprintf(buf,"%d",num);
	return buf;
}

static void* productRun(void* arg)
{
	UdpServer* psvr=(UdpServer*)arg;
	std::string msg;
	RedisApi _redis;
	string Info="key";
	string tmp;
	string value;
	Json::Value val;
	while(1){
		tmp = "";
		psvr->RecvData(msg);
		cout<<"client:"<<msg<<endl;
		DataType::StrToValue(msg,val);

		if("LOAD"==val["msg"].asString()){
			vector<string>::iterator iter=(psvr->session).begin();
			string out;
			while(iter!=(psvr->session).end()){
				_redis.GetData(*iter,out);	
				(psvr->pool).PutData(out);
				iter++;
			}
		}else{
			tmp = Info + _itoa(psvr->SessionCount);  
			psvr->SessionCount++;
			(psvr->SessionCount) %= KEY_MAX;
			(psvr->session).push_back(tmp);
			_redis.InsertDb(tmp,msg);	
		}
		sleep(1);
	}

}

static void* consumRun(void* arg)
{
	UdpServer* psvr=(UdpServer*)arg;
	string msg;
	while(1){
		msg="";
		psvr->broadCast(msg);
		sleep(1);
	}
}

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		cout<<"Useage:"<<argv[0]<<" [local_ip]"<<" [local_port]"<<endl;
		exit(1);
	}
	cout<<"Reay Start"<<endl;
	cout<<"port"<<atoi(argv[2])<<endl;
	std::string ip=argv[1];
	int port = atoi(argv[2]);
	cout<<"Begin::init"<<endl;
	UdpServer svr(ip,port);
	svr.Init();
	pthread_t product;
	pthread_t consum;
	pthread_create(&product,NULL,productRun,(void*)&svr);
	pthread_create(&consum,NULL,consumRun,(void*)&svr);

	pthread_join(product,NULL);
	pthread_join(consum,NULL);
	return 0;
}
