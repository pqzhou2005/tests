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

#define SERVER_PORT 8000
#define BACK_LOG 128


int main(int argv,char **args)
{
	char *p = "15";
	
	int m;
	m = chtoint(p);

	printf("%d",m);
}

