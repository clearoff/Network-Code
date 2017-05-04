#include "condition.h"

//初始化互斥锁和条件变量
int condition_init(condition_t *cond)
{
	int status;
	if(0!=pthread_mutex_init(&(cond->pmutex),NULL))
		return 1;
	if(0!=pthread_cond_init(&(cond->pcond),NULL))
		return 2;
	return 0;
}

int condition_lock(condition_t *cond)
{
	return pthread_mutex_lock(&(cond->pmutex));
}

int condition_unlock(condition_t *cond)
{
	return pthread_mutex_unlock(&(cond->pmutex));
}

int condition_wait(condition_t *cond)
{
	return pthread_cond_wait(&(cond->pcond),&(cond->pmutex));
}

int condition_signal(condition_t *cond)
{
	return pthread_cond_signal(&cond->pcond);
}

int condition_timewait(condition_t *cond, const struct timespec *abstime)
{
	return pthread_cond_timedwait(&cond->pcond,&cond->pmutex,abstime);
}

int condition_broadcast(condition_t *cond)
{
	return pthread_cond_broadcast(&cond->pcond);
}

int condition_destroy(condition_t *cond)
{
	if(pthread_mutex_destroy(&cond->pmutex)!=0)
		return 1;

	if(pthread_cond_destroy(&cond->pcond)!=0)
		return 2;
	return 0;
}
