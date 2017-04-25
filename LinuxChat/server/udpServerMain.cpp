#include "udpServer.h"

using namespace std;

static void* productRun(void* arg)
{
	UdpServer* psvr=(UdpServer*)arg;
	std::string msg;
	while(1){
		psvr->RecvData(msg);
		cout<<"client:"<<msg<<endl;
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
	UdpServer svr(ip,port);
	cout<<"Debug 2"<<endl;
	svr.Init();
	pthread_t product;
	pthread_t consum;
	pthread_create(&product,NULL,productRun,(void*)&svr);
	pthread_create(&consum,NULL,consumRun,(void*)&svr);

	pthread_join(product,NULL);
	pthread_join(consum,NULL);
	return 0;
}
