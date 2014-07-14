#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include <stdlib.h>

#include "http_event.h"
#include "http_server.h"
#include "http_log.h"

int main(int args,char **argv)
{
	int server_socket_fd;
	struct http_event_loop_t *event_loop; 

	server_socket_fd = http_server_socket();

	event_loop = http_event_create(1024);	
	http_event_add(event_loop,server_socket_fd,EVENT_READABLE,onAccept,NULL);
	http_event_main(event_loop);
}

int http_server_socket()
{
	int server_socket_fd;
        int addrlen;
        int ret;
        struct sockaddr_in server_addr;

        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=INADDR_ANY;
        server_addr.sin_port=htons(SERVER_PORT);

        server_socket_fd = socket(PF_INET,SOCK_STREAM,0);
        if(server_socket_fd<0)
        {
                http_log("socket error");
                return -1;
        }

        addrlen = sizeof(struct sockaddr);
        ret = bind(server_socket_fd,(struct sockaddr *)&server_addr,addrlen);
        if(ret<0)
        {
                http_log("bind error");
                return -1;
        }

        ret = listen(server_socket_fd,BACK_LOG);
        if(ret<0)
        {
                http_log("listen error");
                return -1;
        }

	set_non_blocking(server_socket_fd);

        return server_socket_fd;				
}

int http_client_socket(int server_socket_fd)
{
	int client_socket_fd,addrlen,ret;
        struct sockaddr_in client_addr;

        client_socket_fd = accept(server_socket_fd,(struct sockaddr *)&client_addr,&addrlen);
        if(client_socket_fd<0)
        {
              http_log("accpet error");
              return -1;

        }
        set_non_blocking(client_socket_fd);

	return client_socket_fd;		
}

int http_recv(int fd,void *buf,int len)
{
	int ret;
	ret = recv(fd,buf,len,0);
	if(ret==-1)
	{
		if(errno == EAGAIN)
		{
			return 0;
		}
												
		return -1;
	}
	else if(ret==0)
	{
		return -1;
	}
	else
	{
		return ret;
	}
}

void onAccept(http_event_t *http_ev)
{
	int client_socket_fd = http_client_socket(http_ev->fd);

	http_event_add(http_ev->event_loop,client_socket_fd,EVENT_READABLE,onData,NULL);
}

void onData(http_event_t *http_ev)
{
	char *buf;
	int size,ret;
	size = 64;
	
	buf = (char *)malloc(size);	
	memset(buf,0,size);

	ret = http_recv(http_ev->fd,buf,size);
	if(ret>0)
	{
		http_log(buf);	
	}
	else if(ret==-1)
	{
		http_event_del(http_ev->event_loop,http_ev->fd,EVENT_READABLE);		
		http_event_del(http_ev->event_loop,http_ev->fd,EVENT_WRITEABLE);			
		close(http_ev->fd);
	}
	free(buf);
}

void onWrite(http_event_t *http_ev)
{
	
}

int set_non_blocking(int fd)
{
        int flags,ret;
        flags = fcntl(fd,F_GETFL);
        flags = flags|O_NONBLOCK;
        ret = fcntl(fd,F_SETFL,flags);
        return ret;
}

