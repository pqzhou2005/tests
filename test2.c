#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<sys/time.h>
#include<sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<errno.h>
#include<sys/epoll.h>
#include <fcntl.h>
#include <time.h>
#include<unistd.h>

#include "math/math.h"
#include "http_request_r.h"
#include "date/date.h"
#include "content_type.h"
#include "http_response_r.h"

#define SERVER_PORT 8000
#define BACK_LOG 128

int main(int argv,char **args)
{
	while(1){
		fork();
	}
}
