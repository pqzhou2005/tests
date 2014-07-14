#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define SERVER_PORT 8000
#define BACK_LOG 128

struct test
{
	int fd;
}

struct test *test();

int main(int argv,char **args)
{
	struct test *t;
	t = test();
	//printf("%d\n",t.fd);
}

struct test *test()
{
	struct test t;
	t.fd = 1;

	return &t;
}
