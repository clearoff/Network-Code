#include "sql_api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

static void exec_show(const char* arg)
{
	if( arg==NULL && *arg=='\0'){
		cout<<"<h1>arg is error</h1><br/>"<<endl;
	}
	cout<<"<h1>"<<arg<<"</h1><br/>"<<endl;
	cout<<"<h1>Show DataBase</h1><br/>"<<endl;
	mysql sql;
	sql.Connect();
	sql.Select();
}

int main()
{
	char method[256]={0};
	char post_data[1024]={0};
	int clength=0;
	char* tmp=NULL;

	printf("<html/><br/>\n");
	printf("<body/><br/>");
	if((tmp=getenv("REQUEST_METHOD"))==NULL){
		printf("<p>method error<br/>\n");
		return 1;
	}
	strcpy(method,tmp);

	if(strcasecmp("GET",method)==0){
		strcpy(post_data,getenv("QUERY_STRING"));
		exec_show(post_data);
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
		exec_show(post_data);
	}
	printf("</body><br/>\n");
	printf("</html><br/>\n");
	return 0;
}
