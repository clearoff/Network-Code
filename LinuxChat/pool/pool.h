#ifndef _POOL_H__
#define _POOL_H__

#include <iostream>
#include <vector>
#include <string>
#include <semaphore.h>
using namespace std;
#define _BLANK_ 1024

class Pool{
	public:
		Pool();
		void GetData(std::string& outmsg);
		void PutData(const std::string& inmsg);
		~Pool();
	private:
		Pool(const Pool&);
	private:
		vector<std::string> pool;
		int _cap;
		int _size;
		int productStep;
		int consumStep;
		sem_t _blank;
		sem_t _data;
};
#endif //_POOL_H__
