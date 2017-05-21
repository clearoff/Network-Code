#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>



void exec_calc(char* arg){
	if(arg==NULL || *arg=='\0'){
		printf("<p>not cgi arg<br/>\n");
		exit(1);
	}
	char* v1=arg;
	char* v2;
	int clength=atoi(getenv("CONTENT_LENGTH"));
	int i=0;
	int num1;
	int num2;
	for(i=0;i<clength;i++)
	{
		if(arg[i]=='&'){
			arg[i++]='\0';
			v2=arg+i;
			break;
		}
	}
	printf("%s<br/>\n",v1);
	printf("%s<br/>\n",v2);
	num1=atoi(v1+3);
	num2=atoi(v2+3);
	printf("<p>v1+v2=%d<br/>\n",num1+num2);
	//printf("%s\n",v1);
	//printf("%s\n",v2);
	//sscanf(arg,"v1=%d&v2=%d",&v1,&v2);
	//printf("<p>v1+v2=%d<br/>\n",v1+v2);
}
int main()
{
	char method[256]={0};
	char post_data[1024]={0};
	int clength=0;
	char* tmp=NULL;

	printf("<html><br/>\n");
	printf("<body background=\"./pika.jpg\"><br/>");
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
