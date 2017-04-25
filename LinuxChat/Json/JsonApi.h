#ifndef _JSON_H__
#define _JSON_H__

#include <iostream>
#include <json/json.h>
#include <string>


class JsonApi{
	public:
		static void Serialize(const Json::Value root,std::string&);
		static Json::Value deSerialize(const std::string);
};
#endif
