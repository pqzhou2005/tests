#include <stdio.h>  
#include <pthread.h>  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *sub_thread(void *argv);
void *sub_thread2(void *argv);

int i=1;

int main(int argc, char** argv)
{
	int ret;
	pthread_t thread_t,thread_t2;

	ret = pthread_create(&thread_t,NULL,sub_thread,NULL);
	if(ret<0){
		perror("pthread_create error");
		return 0;
	}

	ret = pthread_create(&thread_t2,NULL,sub_thread2,NULL);
	if(ret<0){
		perror("pthread_create error");
		return 0;
	}

	pthread_join(thread_t,NULL);
	pthread_join(thread_t2,NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond); 

	return 0;
}

void *sub_thread(void *argv)
{
	for(i=1;i<=6;i++)
	{
		pthread_mutex_lock(&mutex);
		printf("thread1: lock %d\n", __LINE__);
		if(i%3==0)
		{
			printf("thread1:signal 1 %d\n", __LINE__);
			pthread_cond_signal(&cond);
			printf("thread1:signal 2 %d\n", __LINE__);
			sleep(1);
		}
		pthread_mutex_unlock(&mutex);
		printf("thread1: unlock %d\n", __LINE__);
		sleep(1); 
	}
}

void *sub_thread2(void *argv)
{
	while(i<6)
	{
		pthread_mutex_lock(&mutex);
		printf("thread2: lock %d\n", __LINE__);
		if(i%3!=0)
		{
			printf("thread2: wait 1 %d\n", __LINE__);
			pthread_cond_wait(&cond,&mutex);
			printf("thread2: wait 2 %d\n", __LINE__);
		}

		pthread_mutex_unlock(&mutex);
		printf("thread2: unlock %d\n", __LINE__);
		sleep(1);
	}	 
}
