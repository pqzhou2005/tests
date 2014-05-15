#include <stdio.h>  
#include <pthread.h>  

void *sub_thread(void *argv);

int main(int argc, char** argv)
{
	int ret;
	pthread_t thread_t;
	void* status;	

	ret = pthread_create(&thread_t,NULL,sub_thread,NULL);
	if(ret<0){
		perror("pthread_create error");
		return 0;
	}

	printf("thread id is %lu\n",(unsigned long int)pthread_self());
	printf("process id is %d\n",getpid());

	pthread_join(thread_t,&status);

}

void *sub_thread(void *argv)
{
	printf("thread id is %lu\n",(unsigned long int)pthread_self());
	printf("process id is %d\n",getpid());
}

void *sub_thread2(void *argv)
{
	printf("thread id is %lu\n",(unsigned long int)pthread_self());
        printf("process id is %d\n",getpid());
}
