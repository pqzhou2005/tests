#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<errno.h>
#include<sys/epoll.h>
#include <fcntl.h>

#include "http_request_r.h"


#define SERVER_PORT 8000
#define BACK_LOG 128

void mod_http_request(struct http_request_r *r);
void mod_http_request2(struct http_request_r **r);
void mod_http_request3(struct http_request_r *r);

int main(int argv,char **args)
{
	struct http_request_r *r=NULL;
	
	//mod_http_request(r);	
	mod_http_request2(&r);	
	printf("%d\n",r->fd);
	
	mod_http_request3(r);	
	printf("%d\n",r->fd);
}

void mod_http_request(struct http_request_r *r)
{
	r = (struct http_request_r *)malloc(sizeof(struct http_request_r));
	memset(r,0,sizeof(struct http_request_r));
	r->fd = 1;	
}

void mod_http_request3(struct http_request_r *r)
{
	r->fd = 2;	
}

void mod_http_request2(struct http_request_r **r)
{
	struct http_request_r *q;
	q = (struct http_request_r *)malloc(sizeof(struct http_request_r));
	memset(q,0,sizeof(struct http_request_r));
	q->fd = 1;
	*r = q;	
}
