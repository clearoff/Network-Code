#include <algorithm>
#include <set>
#include "udpClient.h"
#include "../window/window.h"
#include "../Json/DataType.h"
using namespace std;


Window win;
UdpClient* g_cli;
std::string nick_name;
std::string school;
Json::Value g_val;

void SigInt_Fun(int)
{
	Json::Value val;
	val["name"]=nick_name;
	val["school"]=school;
	val["msg"]="";
	val["cmd"]="QUIT";
	std::string in="";
	DataType::ValToString(val,in);
	g_cli->SendData(in);
	exit(1);
}

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

//static void* runFriendList(void* arg)
//{
//	pthread_detach(pthread_self());
//	UdpClient* cli=(UdpClient*)arg;
//	int step=1;
//	win.createFriendlist();
//	win.winRefresh(win.friend_list);
//	int x,y;
//	getmaxyx(win.friend_list,y,x);
//	std::string info;
//	std::set<std::string>::iterator iter=(cli->users).begin();	
//	while(1){
//		if(iter!=(cli->users).end())
//		{
//			win.putstrTowin(win.friend_list,step++,1,*iter);
//			iter++;
//			win.winRefresh(win.friend_list);
//		}
//		else{
//			iter=(cli->users).begin();
//		}
//		step %=y-1;
//	}
//}
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
	std::string info;
	//friend_list
	//

	int step_f=1;
	win.createFriendlist();
	win.winRefresh(win.friend_list);
	int f_x,f_y;
	getmaxyx(win.friend_list,f_y,f_x);
	std::set<std::string>::iterator iter;	
	std::string cmd="";
	std::set<std::string>::iterator _find;
	while(1){
		in="";
		cmd="";
		cli->RecvData(msg);	
		DataType::StrToValue(msg,val);
		in=val["name"].asString()+":"+val["msg"].asString();
		info=val["name"].asString()+"-"+val["school"].asString();
		cmd=val["cmd"].asString();
		if(cmd=="QUIT")
		{
			_find=find((cli->users).begin(),\
				(cli->users).end(),info);
			if(_find!=(cli->users).end()){
				(cli->users).erase(_find);	
			}
		}
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

		_find=find((cli->users).begin(),(cli->users).end(),info);
		if(_find==(cli->users).end())
		{
			(cli->users).insert(info);
			win.putstrTowin(win.friend_list,step_f++,1,info);
			win.winRefresh(win.friend_list);
		}
		step_f %= f_y-1;
		if(step_f ==f_y-1)
		{
			win.clrWinLine(win.friend_list,1,f_y-1);
			win.createOutput();
			win.winRefresh(win.friend_list);
			step_f=1;
			for(iter=(cli->users).begin();iter!=(cli->users).end();iter++){
				win.putstrTowin(win.friend_list,step_f++,1,info);
				win.winRefresh(win.friend_list);
			}
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
	g_cli=&cli;
	signal(SIGINT,SigInt_Fun);
	cli.Init();
	pthread_t header;
	pthread_t output;
	pthread_create(&header,NULL,runHeader,NULL);
	pthread_create(&output,NULL,runOutput,(void*)&cli);

	std::string out;
	Json::Value val;
	nick_name="fly_pig";
	school="SUST";
	val["name"]=nick_name;
	val["school"]=school;
	val["cmd"]="None";
	g_val["name"]=val["name"];
	g_val["school"]=val["school"];

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
