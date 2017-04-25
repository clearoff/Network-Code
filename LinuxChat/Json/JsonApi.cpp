#include "JsonApi.h"

void JsonApi::Serialize(const Json::Value root,std::string& outstr)
{
#ifndef _FAST_W__
	Json::FastWriter writer;
#else
	Json::FastWriter writer;
#endif
	outstr = writer.write(root);
}

Json::Value JsonApi::deSerialize(const std::string str)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(str,root,false);
	//std::string name = root["name"].asString();
	//int age = root["age"].asInt();
	//std::string school = root["school"].asString();
	return root;
}


//#ifdef _DEBUG_
//int main()
//{
//	Json::Value root;
//	root["name"]="leap";
//	root["age"]=21;
//	root["school"]="SUST";
//	std::string outstr;
//	JsonApi::Serialize(root,outstr);
//	cout<<outstr<<endl;
//	Json::Value tmp = JsonApi::deSerialize(outstr);
//	cout<<tmp["name"].asString()<<endl;
//	cout<<tmp["age"].asInt()<<endl;
//	cout<<tmp["school"].asString()<<endl;
//
//}
//#endif
