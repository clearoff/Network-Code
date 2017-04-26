#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include <iostream>
#include "JsonApi.h"
#include <string>

class DataType{
	public:
		DataType()
		{};
		DataType(const std::string _name,const std::string _school,const std::string _cmd,const std::string _msg);
		~DataType();
		static void ValToString(const Json::Value val,std::string& out);
		static void StrToValue(const std::string&,Json::Value& out);
	private:
		std::string name;
		std::string school;
		std::string cmd;
		std::string msg;
};
#endif //_DATA_TYPE_H_
