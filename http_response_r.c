#include <string.h>
#include<stdio.h>
#include<time.h>

#include "str/str.h"
#include "date/date.h"
#include "http_request_r.h"
#include "content_type.h"
#include "file/file.h"

char *static_http_response_out(struct http_request_r *r)
{
	int i,j,n,k,m,period_pos,query_pos,slash_pos,ext_end_pos,ext_length,status_code;
	char *dir;
	char *file_name;
	char *ext=NULL;
	char ch;
	char *out;
	char *const_ext[7]={"png","jpg","gif","swf","zip","html","htm"};
	
	struct http_response_r *o;
	o = (struct http_response_r *)malloc(sizeof(struct http_response_r));
	memset(o,0,sizeof(struct http_response_r));

	init_http_response(r,o);

	dir = (char *)malloc(100);
	memset(dir,0,100);
	file_name = (char *)malloc(50);
	memset(file_name,0,50);
	ext = (char *)malloc(10);
	memset(ext,0,10);

	n = strlen(r->request_uri);
	
	for(i=0;i<n;i++)
	{
		ch = r->request_uri[i];
		if(ch=='/')
		{
			slash_pos=i;
		}
		
		if(ch=='.')
		{
			period_pos=i;
		}
		
		if(ch=='?')
		{
			query_pos=i;
			break;
		}
	}
	
	strcpy(dir,"/data/tests/web/");
	if(slash_pos>0)
	{
		cstrcpy(dir,r->request_uri,0,slash_pos);
	}

	printf("dir:%s\n",dir);

	//ext check
	if(period_pos>0)
	{
		if(query_pos>0)
		{
			ext_end_pos = query_pos-1;
		}
		else
		{
			ext_end_pos = n;
		}
		
		
		for(j=0;j<7;j++)
		{
			m=0;
			i=period_pos-1;
			for(k=0;k<strlen(const_ext[j]);k++)
			{
				if(const_ext[j][k]==r->request_uri[i])
				{
					m++;	
				}
				i++;
			}
			if(m==strlen(const_ext[j]) && i<ext_end_pos)
			{
				strcpy(ext,const_ext[j]);
				break;
			}
		}

		if(strlen(ext)>0)
		{
			cstrcpy(file_name,r->request_uri,slash_pos+1,ext_end_pos-slash_pos-1);	
		}
		else
		{
			status_code = 404;	
		}
	
	}
	else
	{
		strcpy(ext,"html");
		strcpy(file_name,"index.html");
	}

	if(status_code==200){

		strcat(dir,file_name);
		readfile(dir,&out);
		
		
	
	}else
	{
		
	}
		
	return out;	
}


int init_http_resonse(struct http_request_r *r,struct http_response_r *o)
{
	o->status_code = 200;

        o->http_version = (char *)malloc(10);
        memset(o->http_version,0,10);
	strcpy(o->http_version,"HTTP/1.1");

        o->server = (char *)malloc(12);
        memset(o->server,0,12);
        strcpy(o->server,"HTTP SERVER");

        o->reason_phrase = (char *)malloc(50);
	memset(o->reson_phrase,0,50);
	strcpy(o->reason_phrase,"OK");

	o->date = (char *)malloc(100);
	memset(o->date,0,100);
	time_z t = time((time_z *)NULL);
	o->date = gdate(&t);	

	o->content_type = (char *)malloc(50);
	memset(o->content_type,0,50);
	o->content_type = "";
						
	o->connection = (char *)malloc(20);
	memset(o->connection,0,20);
	o->connection = "keep-alive";

	o->last_modified = (char *)malloc(50);
	memset(o->last_modified,0,50);
	o->last_modified = "";

	o->accept_ranges = (char *)malloc(50);
	memset(o->accept_ranges,0,50);
	o->accept_ranges = "bytes";

	return 1;			
}

int gen_http_response_buf(struct http_response_r *p)
{

}                                                                                                                                                                                                                                                                       	
