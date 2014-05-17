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

#include "math/math.h"
#include "http_request_r.h"

#define SERVER_PORT 8000
#define BACK_LOG 128


int main(int argv,char **args)
{
	int ret;
	struct http_request_r *r,*p;
	ret = add_http_request(&r,1);
	r->content_length = 19;
	printf("%d\n",ret);

	ret = delete_http_request(&r,1);
	printf("%d\n",ret);

	ret = count_http_request(r);
	printf("%d\n",ret);

	p = find_http_request(r,1);

	add_http_request(&r,1);
	r->content_length = 68;	

	p = find_http_request(r,1);

	if(p!=NULL) printf("%d\n",p->content_length);
	
}



