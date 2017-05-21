#include "sql_api.h"
using namespace std;

mysql::mysql()
{
	//create mysql handle
	if(NULL==mysql_init(&conn))
	{
		cout<<"<h3>Init failed</h3><br/>"<<endl;
	}
	cout<<"Init Success"<<endl;
}

int mysql::Connect()
{
	//mysql_real_connect
	MYSQL *tmp =mysql_real_connect(&conn,"0","root","960324","student_info",3306,\
			"NULL",0);
	if(NULL==tmp){
		cout<<"<h3>connect sql failed</h3><br/>"<<endl;
		return 0;
	}
	else{
		cout<<"<h3>connect sql success</h3><br/>"<<endl;
		return 1;
	}
}

mysql::~mysql()
{
	mysql_close(&conn);
	cout<<"<h3>sqldb link is closed</h3><br/>"<<endl;
}

int mysql:: Insert(const char* name,const char* sex,const char* major)
{
	// INSERT INTO TABLES VALUES(DATA)
	std::string data="INSERT INTO stu_info(name,sex,major,create_time) values";
	char buf[1024]={0};
	sprintf(buf,"('%s','%s','%s',%s)",name,sex,major,"NOW()");
	data+=buf;
	cout<<"Insert data:"<< data <<endl;
	int ret=mysql_query(&conn,data.c_str());
	if(ret==0){
		cout<<"<h1>Insert success</h1><br/>"<<endl;
	}
	else{
		cerr<<"<h1>Insert faild</h1><br/>"<<endl;
	}
	return ret;
}

int mysql:: Select()
{
	std::string data="select * from stu_info";
	if(0!=mysql_real_query(&conn,data.c_str(),strlen(data.c_str())))
	{
		cout<<"Select failed"<<endl;
		exit(1);
	}

	MYSQL_RES* res;
	MYSQL_ROW row;
	res=mysql_store_result(&conn);
	if(res==NULL){
		cerr<<"Mysql_store_result..."<<endl;
		mysql_free_result(res);
		exit(2);
	}

	unsigned int rownum=mysql_num_fields(res);
	printf("%2s\t%5s\t%s\t%6s\t%15s\n","id","name","sex","major","create_time");
	while(row=mysql_fetch_row(res))
	{
		int i=0;
		for(i=0;i<rownum;i++)
		{
			cout<<"<h3>"<<row[i]<<"\t"<<"</h3>";
		}
		cout<<endl<<"<br/>";
	}
	mysql_free_result(res);
}
//int mysql:: Delete();
//int mysql:: Update();

#ifdef _DEGUG_
int main()
{
	mysql sql;
	sql.Connect();
	sql.Insert("sql","women","ele");
	sql.Select();
	return 0;
}
#endif //_SQL_API_
