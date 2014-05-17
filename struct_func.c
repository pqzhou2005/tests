#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct event_handle
{
	void (*recv_handle)(int);
}event_handle;

void recv_handle(int value);

int main(int argc, char **argv)
{
	printf("%d\n",argc);
	event_handle *event_t;
	event_t = (event_handle *)malloc(sizeof(event_handle));
	memset(event_t,0,sizeof(event_handle));
	event_t->recv_handle=&recv_handle;

	event_t->recv_handle(3);
	return 0;
}

void recv_handle(int value)
{
	printf("%d\n",value);
}
