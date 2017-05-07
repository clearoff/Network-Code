#include "DataType.h"
using namespace std;


DataType::DataType(const string _name,const string _school,const string _cmd,const string _msg):
	name(_name),
	school(_school),
	cmd(_cmd),
	msg(_msg)
{}

DataType::~DataType()
{}

void DataType::ValToString(const Json::Value val,std::string& out)
{
//	Json::Value val;
//	val["name"]=name;
//	val["school"]=school;
//	val["cmd"]=cmd;
//	val["msg"]=msg;
	JsonApi::Serialize(val,out);
}
void DataType::StrToValue(const std::string& str,Json::Value& out)
{
	out = JsonApi::deSerialize(str);
}


#ifdef _DATA_BUG_

int main()
{
	DataType v("fly pig","SUST","None","HelloWorld");
	std::string outstr;
	v.ValToString(outstr);
	cout<<outstr<<endl;
	Json::Value val;
	v.StrToValue(outstr,val);
	cout<<val["name"]<<endl;
	cout<<val["school"]<<endl;
	cout<<val["cmd"]<<endl;
	cout<<val["msg"]<<endl;
	return 0;
}
#endif
