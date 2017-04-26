#include "udpClient.h"
#include "../window/window.h"
#include "../Json/DataType.h"
using namespace std;


Window win;

static void* runHeader(void* arg)
{
	pthread_detach(pthread_self());
	std::string msg = "Welcome my chat!";
	int index=0;
	int x,y;
	win.createHeader();
	win.winRefresh(win.header);
	getmaxyx(win.header,y,x);
	y=y/2;
	while(1){
		win.createHeader();
		win.winRefresh(win.header);
		index=index%x;
		win.putstrTowin(win.header,y,index++,msg);
		win.winRefresh(win.header);
		sleep(1);
		win.clrWinLine(win.header,y,1);
	}

}
static void* runOutput(void* arg)
{
	pthread_detach(pthread_self());
	UdpClient* cli = (UdpClient*)arg;
	std::string msg;
	int step=1;
	win.createOutput();
	win.winRefresh(win.output);
	int x,y;
	getmaxyx(win.output,y,x);
	Json::Value val;
	std::string in;
	while(1){
		in="";
		cli->RecvData(msg);	
		DataType::StrToValue(msg,val);
		in=val["name"].asString()+":"+val["msg"].asString();
		win.putstrTowin(win.output,step++,1,in);
		win.winRefresh(win.output);
		step %=y-1;
		usleep(100000);
		if( step == y-1){
			win.clrWinLine(win.output,1,y-1);
			win.createOutput();
			win.winRefresh(win.output);
			step=1;
		}
	}
}

int main(int argc,char* argv[])
{
	if(argc!=3){
		cout<<"Usege:"<< argv[0]<<"[ peer_ip]"<<" [peer_port]"<<endl;
		exit(1);
	}

	UdpClient cli(argv[1],atoi(argv[2]));
	cli.Init();
	pthread_t header;
	pthread_t output;
	pthread_create(&header,NULL,runHeader,NULL);
	pthread_create(&output,NULL,runOutput,(void*)&cli);

	std::string out;
	Json::Value val;
	val["name"]="fly pig";
	val["school"]="SUST";
	val["cmd"]="None";
	while(1){
		out="";
		win.createInput(out);
		val["msg"]=out.c_str();
		std::string in;
		DataType::ValToString(val,in);
		win.winRefresh(win.input);
		cli.SendData(in);
		usleep(200000);
	}
	pthread_join(header,NULL);
	pthread_join(output,NULL);
	return 0;
}
//int main(int argc,char* argv[])
//{
//	if(argc!=3){
//		cout<<"Usege:"<<" "<<argv[0]<<" [peer_ip]"<<" [peer_port]"<<endl;
//		exit(1);
//	}
//	UdpClient cli(argv[1],atoi(argv[2]));
//	cli.Init();
//	string msg="Hello,World";
//	while(1){
//		cli.SendData(msg);
//		cout<<"Send done ..."<<endl;
//		sleep(1);
//		msg="";
//		cli.RecvData(msg);
//		cout<<"echo:"<<msg<<endl;
//	}
//	return 0;
//}
