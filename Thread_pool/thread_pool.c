#include "thread_pool.h"
#include <time.h>

static void *thread_routine(void* arg)
{
	struct timespec abstime;
	int timeout;
	printf("thread %d is start\n",(int)pthread_self());
	threadpool_t *pool=(threadpool_t *)arg;
	while(1){
		timeout=0;
		//访问线程池前需要加锁
		condition_lock(&pool->status);	
		//空闲线程的数量加1
		pool->idle++;
		//等待队列有任务到来或者收到线程池销毁的通知
		while(pool->first==NULL && !pool->quit)
		{
			printf("thread %d is wait\n",(int)pthread_self());	
			//获取当前的时间
			clock_gettime(CLOCK_REALTIME,&abstime);
			//加上等待时间
			abstime.tv_sec += 2;
			int status;
			status = condition_timewait(&pool->status,&abstime);
			if(status == ETIMEDOUT)
			{
				printf("thread %d wait time out\n",(int)pthread_self());
				timeout=1;
				break;
			}
		}
		pool->idle--;
		if(pool->first != NULL)
		{
			task_t *task = pool->first;
			pool->first = task->next;
			condition_unlock(&pool->status);
			task->run(task->arg);
			free(task);
			condition_lock(&pool->status);
		}

		if(pool->quit && pool->first == NULL)
		{
			pool->count--;
			if(pool->count == 0)
			{
				condition_signal(&pool->status);
			}
			condition_unlock(&pool->status);
			break;
		}

		if(timeout == 1)
		{
			pool->count--;
			condition_unlock(&pool->status);
			break;
		}

		condition_unlock(&pool->status);
	}

	printf("thread %d is exiting \n",(int)pthread_self());
	return NULL;
}

void threadPool_init(threadpool_t *pool,int threads)
{
	condition_init(&pool->status);
	pool->first=NULL;
	pool->last=NULL;
	pool->count=0;
	pool->idle=0;	//描述空闲的线程的数量
	pool->maxThread=threads;
	pool->quit=0;	
}

//添加一个任务到线程池
void threadPool_addtask(threadpool_t* pool,void *(*run)(void* arg),void* arg)
{
	task_t *newtask = (task_t *)malloc(sizeof(task_t));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = NULL;

	//对线程池进行加锁
	condition_lock(&pool->status);

	if(pool->first==NULL)
	{
		pool->first=newtask;
	}
	else{
		pool->last->next=newtask;
	}
	pool->last=newtask;

	//如果线程池中有空闲的线程则进行唤醒
	if(pool->idle > 0){
		condition_signal(&pool->status);
	}
	//如果当前线程池中线程个数没有达到设定的最大值
	//并且没有空闲的线程，则创建一个新的线程
	else if(pool->count < pool->maxThread)
	{
		pthread_t tid;
		pthread_create(&tid,NULL,thread_routine,pool);
		pool->count++;
	}
	//结束对线程池的访问
	condition_unlock(&pool->status);
}

void threadPool_destroy(threadpool_t *pool)
{
	if(pool->quit)
	{
		return;
	}

	condition_lock(&pool->status);
	pool->quit=1;
	if(pool->count > 0)
	{
		if(pool->idle > 0)
		{
			//对于等待的线程，发送信号唤醒
			condition_broadcast(&pool->status);
		}
		//正在执行的线程，等待他们的任务结束
		while(pool->count)
		{
			condition_wait(&pool->status);
		}
	}
	condition_unlock(&pool->status);
	condition_destroy(&pool->status);
}


#define _DEBUG_
#ifdef _DEBUG_

void* mytask(void * arg)
{
	printf("thread %d is working on task %d\n",\
			(int)pthread_self(),*((int*)arg));
	sleep(1);
	free(arg);
	return NULL;
}

int main()
{
	threadpool_t pool;
	threadPool_init(&pool,3);
	printf("Debug 1\n");
	int i;
	for(i=0;i<10;i++)
	{
		int *arg=malloc(sizeof(int));
		*arg=1;
		threadPool_addtask(&pool,mytask,arg);
	}
	printf("Debug 2\n");
	threadPool_destroy(&pool);
	//threadPool_destroy(&pool);
	return 0;
}

#endif //_DEBUG_
