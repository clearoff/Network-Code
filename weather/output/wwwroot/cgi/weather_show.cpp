#include<iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

static int ShowWeather(const char* arg)
{
	if(arg==NULL && *arg=='\0')
	{
		cout<<"<h1> arg is error</h1><br/>"<<endl;
	}
	cout<<"<h1>"<<arg<<"</h1><br/>"<<endl;
	pid_t id = vfork();
	if(id == 0){
		cout<<"Begin execl"<<endl;
		execl("./ShowWeather.py","./ShowWeather.py",NULL);
		cout<<"execl error"<<endl;
		exit(1);
	}
	if(id==waitpid(id,NULL,0))
	{
		cout<<"weather already push"<<endl;
		return 0;
	}
	cout<<"exec error"<<endl;
	return 1;
}

int main()
{
	char method[256]={0};
	char post_data[1024]={0};
	int clength=0;
	char* tmp;

	printf("<html><br/>\n");
	printf("<body background=\"./pika.jpg\"><br/>\n");
	if((tmp=getenv("REQUEST_METHOD"))==NULL){
		printf("<p>method error<br/>\n");
		return 1;
	}
	strcpy(method,tmp);

	printf("method=%s\n",method);
	if(strcasecmp("GET",method)==0){
		strcpy(post_data,getenv("QUERY_STRING"));
		ShowWeather(post_data);
	}else
	{
		char ch;
		int i=0;
		clength=atoi(getenv("CONTENT_LENGTH"));
		cout<<"Content-Length:"<<clength<<endl;
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
		ShowWeather(post_data);
	}
	printf("</body><br/>\n");
	printf("</html><br/>\n");
	return 0;
}
