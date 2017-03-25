#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

MYSQL mysql;

void ConnectDB(const char* host,const char* user,const char* passwd,\
		const char* db)
{
	assert(host);
	assert(user);
	assert(passwd);
	assert(db);
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,host,user,passwd,db,3306,NULL,0)){
		perror("MySQL connect failed...\n");
		exit(1);
	}

	printf("MySQL connect success...\n");
}
void FindData(const char* msg)
{
	assert(msg);

	char buf[1024];
	strcpy(buf,"SELECT ");
	strcat(buf,msg);
	strcat(buf," from pet");
	printf("%s\n",buf);

	int len=strlen(buf);
	if(0!=mysql_real_query(&mysql,buf,len))
	{
		perror("find failed...\n");
		exit(2);
	}

	MYSQL_RES* res;
	MYSQL_ROW row;
	res=mysql_store_result(&mysql);
	if(res==NULL)
	{
		perror("mysql_store_result...");
		mysql_free_result(res);
		exit(3);
	}
	// find success
	unsigned int num=mysql_num_fields(res);
	printf("%5s\t%s\t,%15s\t\n","|name|","sex|","birth|");
	while((row=mysql_fetch_row(res)))
	{

		unsigned int i=0;
		for(i=0;i<num;i++)
		{
			printf("%s\t",row[i]);	
		}
		printf("\n");
	}
	mysql_free_result(res);
}
void CloseDB()
{
	mysql_close(&mysql);
}
int main(int argc,char* argv[])
{
	if(argc!=5){
		printf("Useage : %s [host] [user] [passwd] [db]\n",argv[0]);
		return 1;
	}
	ConnectDB(argv[1],argv[2],argv[3],argv[4]);
	FindData("*");
	CloseDB();
	return 0;
}
