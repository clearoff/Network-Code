#ifndef _SQL_API_
#define _SQL_API_

#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

class mysql
{
public:
	mysql();
	~mysql();
	int Connect();
int Insert(const char* name,const char* sex,const char* major);
	int Select();
	int Delete();
	int Update();
private:
	MYSQL conn;
};


#endif //_SQL_API_
