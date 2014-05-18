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
	char *path = "/data/tests/web/1.gif";

	FILE *fp;
	int size;
	char str[1]={0}; 

	fp = fopen(path,"r");
        fseek(fp,0,SEEK_END);
        size = ftell(fp)+1;
	fread(str,1,1,fp);
	printf("%02x ",str[0]);
	fclose(fp);
	
	printf("size:%d\n",size);

	struct stat stat_buf;
        stat(path,&stat_buf);	
	printf("stsize:%d\n",stat_buf.st_size);
	
}
