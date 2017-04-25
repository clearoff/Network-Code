#include "pool.h"



Pool::Pool()
	:_cap(1024)
	,pool(_cap)
	,_size(0)
	,productStep(0)
	,consumStep(0)
{
	sem_init(&_blank,0,_cap);
	sem_init(&_data,0,0);
}

void Pool::GetData(string& outmsg)
{
	sem_wait(&_data);
	outmsg = pool[consumStep];
	consumStep++;
	consumStep %= _cap;
	_size--;
	sem_post(&_blank);
}

void Pool::PutData(const string& inmsg)
{
	//product
	sem_wait(&_blank);
	pool[productStep]=inmsg;
	productStep++;
	productStep %= _cap;
	_size++;
	sem_post(&_data);
}

Pool::~Pool()
{
	sem_destroy(&_blank);
	sem_destroy(&_data);
	_cap=0;
	_size=0;
}

#ifdef _DEBUG_
int main()
{
	Pool pool;
	return 0;
}
#endif
