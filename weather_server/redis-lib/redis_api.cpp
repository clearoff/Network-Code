#include "redis_api.h"

using namespace std;
RedisApi::RedisApi()
{
	rds = redisConnect("127.0.0.1",6379);
	if(rds==NULL || rds->err){
		if(rds){
			cout<<"Error:"<<rds->errstr<<endl;
		}
		else{
			cout<<"Can't allocate redis context"<<endl;		
		}
		exit(1);
	}
	cout<<"Redis connect success"<<endl;
}

RedisApi::~RedisApi()
{
	redisFree(rds);
	cout<<"Close link to redis"<<endl;
}

int RedisApi::InsertDb(const std::string& key,const std::string& value)
{	
	int ret = 0;
	reply = (redisReply *)redisCommand(rds,"select 1");
	assert(reply!=NULL);
	freeReplyObject(reply);

	cout<<"SET "<<key<<" "<<value<<endl;
	reply = (redisReply *)redisCommand(rds,"HSET user %s %s",\
			key.c_str(),value.c_str());
	if( reply==NULL){

		cout<<"Insert error"<<endl;
		ret = -1;
	}
	else{
		ret = 0;
		cout<<"Insert Success"<<endl;
	}
	//if(reply->type==REDIS_REPLY_INTEGER){
	//	
	//	cout<<"Insert Success"<<endl;
	//	ret = 0;
	//}
	//else{

	//	cout<<"Insert failed"<<endl;
	//	ret = 1;
	//}
	freeReplyObject(reply);
	return ret;
}

int RedisApi::GetData(const std::string& key,std::string& out)
{
	int ret = 0;
	reply =(redisReply *)redisCommand(rds, "select 1");
	assert(reply!=NULL);
	//redisGetReply(rds,(void**)&reply);
	freeReplyObject(reply);
	reply =(redisReply *)redisCommand(rds, "HGET user %s"\
			,key.c_str());
	//assert(reply!=NULL);
	//redisGetReply(rds,(void**)&reply);
	out = reply->str;
	cout<< reply->str<<endl;
	freeReplyObject(reply);
	return ret;
}

#ifdef _DEBUG_

//int main()
//{
//	RedisApi _redis;
//	_redis.InsertDb("message","ni hao a ");
//	string out;
//	_redis.GetData("message",out);
//	cout<<out<<endl;
//	return 0;
//}
#endif //_DEBUG_
