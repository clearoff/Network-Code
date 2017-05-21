#include "sql_api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void exec_add(char* arg)
{
	if(arg==NULL || *arg=='\0'){
		printf("<p>not cgi arg<br/>\n");
		exit(1);
	}
	int clength=atoi(getenv("CONTENT_LENGTH"));
	printf("<p>length=%d,arg=%s\n<br/>",clength,arg);
	char* data[10];
	int i=0;
	char* cur=arg;
	while(*cur){
		if(*cur=='='){
			data[i]=cur+1;
			i++;
		}
		else if(*cur=='&'){
			*cur=0;
		}
		cur++;
	}
	//printf("name=%s\n",data[0]);
	//printf("sex=%s\n",data[1]);
	//printf("major=%s\n",data[2]);
	mysql sql;
	sql.Connect();
	sql.Insert(data[0],data[1],data[2]);
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
		printf("QUERY_STRING:%s\n",post_data);
		exec_add(post_data);
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
		exec_add(post_data);
	}
	printf("</body><br/>\n");
	printf("</html><br/>\n");
	return 0;
}
