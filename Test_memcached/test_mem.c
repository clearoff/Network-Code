#include <sys/time.h>
#include <libmemcached/memcached.h>
#include <stdio.h>



int main()
{
	memcached_st *mem;
	memcached_return rc;
	memcached_server_st *servers;

	char value[8191];
	mem=memcached_create(NULL);
	servers=memcached_server_list_append(NULL,"localhost",11211,&rc);
	rc=memcached_server_push(mem,servers);
	memcached_server_free(servers);
	
	size_t i;
	char* keys[]={"key1","key2","key3"};
	size_t key_length[]={4,4,4};
	char* values[]={
		"This is first value",
		"This is second value",
		"This is third value"
	};
	
	size_t val_length[]={
		19,20,19
	};
	
	uint32_t flags;
	int k = 0;
	char* result;
	for(k=0;k<3;k++)
	{
		rc=memcached_set(mem,keys[k],key_length[k],values[k],\
				val_length[k],(time_t)180,(uint32_t)0);

		if(rc==MEMCACHED_SUCCESS)
		{
			//cout<<"save keys"<<keys[i]<<",value:"<<values[i]<<endl;
			printf("save keys:%s,value:%s\n",keys[k],values[k]);
		}
		char* key_return;
		// test set success
		result=memcached_get(mem,keys[k],key_length[k],\
				&val_length[k],&flags,&rc);
		if(rc==MEMCACHED_SUCCESS)
		{
			//cout<<"get value :"<<result<<endl;
			printf("Get value:%s\n",result);
		}

	}

	memcached_free(mem);
	return 0;
}
