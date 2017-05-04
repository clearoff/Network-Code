#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "condition.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task
{
	void *(*run)(void *args);  //function pointers
	void *arg;				   //线程函数的参数
	struct task *next;
}task_t;

typedef struct threadpool
{
	condition_t status;	//describe thread status
	task_t *first;
	task_t *last;
	int count;
	int maxThread;
	int quit;
	int idle;	//???
}threadpool_t;

void threadPool_init(threadpool_t* pool,int threads);
void threadPool_addtask(threadpool_t* pool,void *(*run)(void* arg),void* arg);
void threadPool_destroy(threadpool_t* pool);


#endif //_THREAD_POOL_H_
