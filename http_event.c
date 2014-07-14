#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "http_event.h"

http_event_loop_t *http_event_create(int event_max)
{
	struct http_event_loop_t *event_loop;
	event_loop = (http_event_loop_t *)malloc(sizeof(http_event_loop_t));
	memset(event_loop,0,sizeof(struct http_event_loop_t));
	event_loop->epfd = epoll_create(event_max);	
	event_loop->http_events = (struct http_event_t *)malloc(sizeof(struct http_event_t)*event_max);
	
	int i;
	for(i=0;i<event_max;i++)
	{
		event_loop->http_events[i].mask = EVENT_NONE;
	}

	return event_loop;
}

int http_event_add(http_event_loop_t *event_loop,int fd,int mask,event_handler *read_handler,event_handler *write_handler)
{
	http_event_t *http_ev;
	http_ev = &(event_loop->http_events[fd]);
	http_ev->fd = fd;
	http_ev->event_loop = event_loop;

	if(read_handler!=NULL) http_ev->read_handler = read_handler;
        if(write_handler!=NULL) http_ev->write_handler = write_handler;

	int op = http_ev->mask == EVENT_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

	http_ev->mask |= mask;
        
	struct epoll_event ev;
        ev.data.ptr = http_ev;

	if(http_ev->mask & EVENT_READABLE) ev.events |= EPOLLIN;
        if(http_ev->mask & EVENT_WRITEABLE) ev.events |= EPOLLOUT;

        epoll_ctl(event_loop->epfd,op,fd,&ev);
}

int http_event_del(http_event_loop_t *event_loop,int fd,int mask)
{
	http_event_t *http_ev;
	http_ev = &(event_loop->http_events[fd]);
	http_ev->fd = fd;
	http_ev->event_loop = event_loop;
	
	http_ev->mask &= (~mask);
	
	struct epoll_event ev;
        ev.data.ptr = http_ev;

        if(http_ev->mask & EVENT_READABLE) ev.events |= EPOLLIN;
        if(http_ev->mask & EVENT_WRITEABLE) ev.events |= EPOLLOUT;
	
	int op = http_ev->mask == EVENT_NONE ? EPOLL_CTL_DEL : EPOLL_CTL_MOD;

	epoll_ctl(event_loop->epfd,op,fd,&ev);
}

int http_event_main(http_event_loop_t *event_loop)
{
	int nfds,i;
	event_handler *read_handler,*write_handler;
	http_event_t *http_ev;
	struct epoll_event ep_events[1024];
	while(1)
        {
                nfds=epoll_wait(event_loop->epfd,ep_events,1024,500);
                for(i=0;i<nfds;i++)
                {
			http_ev = ep_events[i].data.ptr;
                        if(ep_events[i].events & EPOLLIN)
			{
				http_ev->read_handler(ep_events[i].data.ptr);
			}
			if(ep_events[i].events & EPOLLOUT)
			{
				if(http_ev->mask & EVENT_WRITEABLE)
				{
					http_ev->write_handler(ep_events[i].data.ptr);
				}
			}
                }
        }
}
