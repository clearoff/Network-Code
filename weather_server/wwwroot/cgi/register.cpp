#include <iostream>
using namespace std;

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "redis-lib/redis_api.h"



static std::string _itoa(int num)
{
	char buf[8];
	sprintf(buf,"%d",num);
	return buf;
}

void exec_calc(char* arg){
	if(arg==NULL || *arg=='\0'){
		printf("<p>not cgi arg<br/>\n");
		exit(1);
	}
	int clength = atoi(getenv("CONTENT_LENGTH"));
	char* data[10];
	char* cur=arg;
	int i=0;
	while(*cur){
		if((*cur)=='='){
			data[i]=cur+1;
			i++;
		}
		else if((*cur)=='&'){
			*cur=0;
		}
		cur++;
	}

	RedisApi _redis;
	string out="";
	_redis.GetData("UserCount",out);
	int count=atoi(out.c_str());
	string user="user"+_itoa(count);
	count++;
	_redis.InsertDb("UserCount",_itoa(count).c_str());
	string email=data[1];
	_redis.InsertDb(user.c_str(),email.c_str());
	_redis.GetData("user0",out);
	cout<<out<<endl;

}
int main()
{
	char method[256]={0};
	char post_data[1024]={0};
	int clength=0;
	char* tmp=NULL;

	printf("<html><br/>\n");
	printf("<body background=\"./pika.jpg\"><br/>\n");
	if((tmp=getenv("REQUEST_METHOD"))==NULL){
		printf("<p>method error<br/>\n");
		return 1;
	}
	strcpy(method,tmp);

	if(strcasecmp("GET",method)==0){
		strcpy(post_data,getenv("QUERY_STRING"));
		exec_calc(post_data);
	}else
	{
		char ch;
		int i=0;
		clength=atoi(getenv("CONTENT_LENGTH"));
		for(i=0; i<clength; i++)
		{
			//recv(0,&ch,1,0);
			read(0,&ch,1);
			post_data[i]=ch;
		}
		post_data[i]='\0';
//		int fd=open("sock.txt",O_WRONLY|O_CREAT);
//		char buf[1024]={0};
//		strcpy(buf,post_data);
//		write(fd,buf,strlen(post_data));
//
		exec_calc(post_data);
	}
	printf("</body><br/>\n");
	printf("</html><br/>\n");
	return 0;
}
