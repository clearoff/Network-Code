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

void InsertData(const char* msg)
{
	// INSERT INTO tablename VALUES();
	assert(msg);
	char buf[1024];
	strcpy(buf,"INSERT INTO pet VALUES");
	strcat(buf,msg);
	printf("%s\n",buf);
	int len=strlen(buf);
	if(mysql_real_query(&mysql,buf,len)){
		perror("Insert failed...\n");	
		exit(1);
	}
	printf("Insert Data into db success\n");
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

void DeleteData(const char* msg)
{
	assert(msg);
	//DELETE FORM pet WHERE **='**'
	char buf[1024];
	strcpy(buf,"DELETE FROM pet WHERE ");
	strcat(buf,msg);
	if(mysql_real_query(&mysql,buf,strlen(buf))){
		perror("Delete data failed");
		exit(1);
	}
	printf("Delete Success\n");
}

void ReplaceData(const char* msg)
{
	//UPDATE TABLENAME SET **=** WHERE **=**
	char buf[1024];
	strcpy(buf,"UPDATE pet SET ");
	strcat(buf,msg);
	if(mysql_real_query(&mysql,buf,strlen(buf))){
		perror("Replace error");
		exit(1);
	}
	printf("Replace data success\n");
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
	InsertData("('lzk','NULL','Y','1996-10-01')");
	FindData("*");
	ReplaceData("birth='2017-03-24' WHERE name='HEHE'");
	//DeleteData("name='name'");
	FindData("*");
	CloseDB();
	return 0;
}
