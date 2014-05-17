#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "http_request_r.h"

int add_http_request(struct http_request_r **r,int fd)
{
	struct http_request_r *p,*q;
	p = *r;
       	
	q = (struct http_request_r *)malloc(sizeof(struct http_request_r));
       	memset(q,0,sizeof(struct http_request_r));
	q->fd = fd;

	if(p!=NULL)
	{
		while(p->pnext!=NULL)
        	{
                	p = p->pnext;
        	}

        	p->pnext = q;
	}else{
		*r = q;
	}
	
	return 1;
}

int delete_http_request(struct http_request_r *r,int fd)
{
	struct http_request_r *q,*s;
	q = r;

	int i=0;
	while(q!=NULL)
	{
		if(q->fd==fd)
		{
			break;
		}
		q = q->pnext;
		i++;
	}

	q = r;
	
	int j=0;
	while(q!=NULL)
	{
		if(j==i-1)
		{
			s = q->pnext;
			q->pnext = q->pnext->pnext;		
			free(s);
			return 1;
		}
		q = q->pnext;
		j++;
	}
	return -1;
}


http_request_r* find_http_request(struct http_request_r *r,int fd)
{
	struct http_request_r *q,*s;
	q = r;
	int i=0;
	while(q!=NULL)
	{
		if(q->fd==fd)
		{
			return q;
		}
		i++;
		q = q->pnext;
	}

	return s;
}

void print_http_request(struct http_request_r *r)
{
	struct http_request_r *p;
	p = r;
	int i=0;

	while(p!=NULL)
	{
		p = p->pnext;
		i++;
	}
}

int count_http_request(struct http_request_r *r)
{
	struct http_request_r *q;
        q = r;
        int i=0;
        while(q!=NULL)
        {
                i++;
		q = q->pnext;
        }
	return i;
} 

