#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<errno.h>
#include<sys/epoll.h>
#include<fcntl.h>

#include "http_request_r.h"
#include "http_request_parse.h"
#include "http_response_r.h"

#define SERVER_PORT 8000
#define BACK_LOG 128
#define LF     (u_char) 10
#define CR     (u_char) 13


int server_start();

int set_non_blocking(int fd);

int onData(int epfd,struct epoll_event ev,http_request_r **r);

int onAccept(int epfd,struct epoll_event ev,http_request_r **r);

int epoll_add(int epfd,int fd);


int main(int argv,char **args)
{
	int ret,epfd,nfds,server_socket_fd,i,index;
	struct epoll_event events[256];
	int count;	

	server_socket_fd = server_start();	
	if(server_socket_fd<0)
	{
		return 0;
	}
	set_non_blocking(server_socket_fd);

	epfd = epoll_create(1024);
	set_non_blocking(epfd);
	epoll_add(epfd,server_socket_fd);

	struct http_request_r *r=NULL,*p=NULL;

        while(1)
        {
                nfds=epoll_wait(epfd,events,20,500);
                for(i=0;i<nfds;i++)
                {
                        if(events[i].data.fd==server_socket_fd)
                        {
				onAccept(epfd,events[i],&r);
			}
                        else if(events[i].events&EPOLLIN)
                        {
				onData(epfd,events[i],&r);
                        }
                }
        }
}

int onData(int epfd,struct epoll_event ev,http_request_r **r)
{
	char buf[1024];
	void *msg;
	int ret;
	int j;

	memset(buf,0,1024);
        ret = recv(ev.data.fd,buf,1024,0);
        if(ret==-1)
        {
              if(errno != EINTR  && errno != EAGAIN){
                    close(ev.data.fd);
                    ev.data.fd = -1;
                    perror("error");
              }else{
                    perror("recive error");
              }
        }
        else if(ret==0)
	{
              close(ev.data.fd);
              ev.data.fd = -1;
        }
        else if(ret>0)
        {
		struct http_request_r *p;
		p = find_http_request(*r,ev.data.fd);
	      	http_request_parse(p,buf);

		if(p->is_end==1)
		{
			struct http_response_r *o;
			o = (struct http_response_r *)malloc(sizeof(struct http_response_r));			
			memset(o,0,sizeof(struct http_response_r));

			static_http_response_out(*r,o);
              		
			ret = send(ev.data.fd,o->buf,o->size,0);
              		if(ret<0)
              		{
                    		perror("send error!");
                    		return -1;
              		}
	
			ret = send(ev.data.fd,o->body,o->content_length,0);
              		if(ret<0)
              		{
                    		perror("send error!");
                    		return -1;
              		}
	
			delete_http_request(r,ev.data.fd);	
		
			close(ev.data.fd);
                	ev.data.fd=-1;		
		}		
        }
	return 0;
}

int onAccept(int epfd,struct epoll_event ev,struct http_request_r **r)
{
	int server_socket_fd,client_socket_fd,addrlen,ret;
	struct sockaddr_in client_addr;

	server_socket_fd = ev.data.fd;
	client_socket_fd = accept(server_socket_fd,(struct sockaddr *)&client_addr,&addrlen);
        if(client_socket_fd<0)
        {
              perror("accpet error");
              return -1;
        
	}

        add_http_request(r,client_socket_fd);
	struct http_request_r *q;
	q = *r;

	set_non_blocking(client_socket_fd);
	epoll_add(epfd,client_socket_fd);

	return 0;
}

int epoll_add(int epfd,int fd)
{
	struct epoll_event ev;
	
	ev.data.fd=fd;
        ev.events=EPOLLIN|EPOLLET;
        epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);

	return 1;
}

int set_non_blocking(int fd)
{
	int flags,ret;
	flags = fcntl(fd,F_GETFL);
        flags = flags|O_NONBLOCK;
        ret = fcntl(fd,F_SETFL,flags);
	return ret;
}

int server_start()
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
                perror("socket error");
                return -1;
        }

        addrlen = sizeof(struct sockaddr);
        ret = bind(server_socket_fd,(struct sockaddr *)&server_addr,addrlen);
        if(ret<0)
        {
                perror("bind error");
                return -1;
        }

        ret = listen(server_socket_fd,BACK_LOG);
        if(ret<0)
        {
                perror("listen error");
                return -1;
        }

	return server_socket_fd;
}
