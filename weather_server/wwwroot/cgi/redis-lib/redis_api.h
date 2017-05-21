#ifndef _REDIS_API_H_
#define _REDIS_API_H_

#include "./hiredis/hiredis.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <vector>

class RedisApi{
private:
	redisContext* rds;
	redisReply* reply;
public:
	RedisApi();
	~RedisApi();
	int InsertDb(const std::string& key,const std::string& value);
	int GetData(const std::string& key,std::string& out);
};
#endif //_REDIS_API_H_
