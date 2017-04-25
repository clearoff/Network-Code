#include "udpClient.h"
using namespace std;


int main(int argc,char* argv[])
{
	if(argc!=3){
		cout<<"Usege:"<<" "<<argv[0]<<" [peer_ip]"<<" [peer_port]"<<endl;
		exit(1);
	}
	UdpClient cli(argv[1],atoi(argv[2]));
	cli.Init();
	string msg="Hello,World";
	while(1){
		cli.SendData(msg);
		cout<<"Send done ..."<<endl;
		sleep(1);
		msg="";
		cli.RecvData(msg);
		cout<<"echo:"<<msg<<endl;
	}
	return 0;
}
