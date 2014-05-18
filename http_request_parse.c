#include <string.h>
#include <stdio.h>

#include "http_request_r.h"
#include "http_request_parse.h"
#include "math/math.h"

int http_request_parse(struct http_request_r *r,char p[1024])
{
	int n,i,header_length=0;
	char ch;

	strcat(r->buf,p);	

	n = strlen(r->buf);
	for(i=0;i<n;i++)
	{
		ch = r->buf[i];
		//printf("%02x ",ch);
		if(ch=='\r' && n>i+3)
		{
			if(r->buf[i+1]=='\n' && r->buf[i+2]=='\r' && r->buf[i+3]=='\n')
			{
				header_length = i+4;
				break;
			}
		}
	}

	if(header_length>0)
	{
		//parse header
		r->content_pos = header_length;
		http_request_header_parse(r);
		if(r->content_length==n-header_length)
		{
			r->is_end = 1;
		}
		if(r->is_end==1)
		{
			http_request_line_parse(r);
			
			http_request_header_fields_parse(r);
		}
	}

	return 0;
}

int http_request_header_parse(struct http_request_r *r)
{
	int i,j,n,m=0,l,k,content_len;
	char ch;	
	char *content_length = "Content-Length";
	char content_len_str[12];
	memset(content_len_str,0,12);

	n = strlen(r->buf);
	
	for(i=0;i<n;i++)
	{
		ch = r->buf[i];
		if(ch=='\r' && r->buf[i+1]=='\n')
		{
			m=0;
			for(j=0;j<strlen(content_length);j++)
			{
				if(r->buf[i+2+j]==content_length[j])
				{
					m++;
				}
			}
			
			if(m==strlen(content_length))
			{
				for(j=i+1+m;j<n;j++)
				{
					if(r->buf[j]=='\r' && r->buf[j+1]=='\n')
					{
						l = j;
						break;
					}
				}
				
				k=0;
				for(j=i+1+m;j<l;j++)
				{
					if(r->buf[j]>='0' && r->buf[j]<='9')
					{
						content_len_str[k]=r->buf[j];
						k++;
					}
				}
				break;
			}
			
		}		

	}

	content_len = chtoint(content_len_str);
	
	r->content_length = content_len;

	return 1;
}

int http_request_line_parse(struct http_request_r *r)
{
	int i,n,k,m;
	n = strlen(r->buf);	
	k = 0;
	int pos[3]={0};

	for(i=0;i<n;i++)
	{
		if(r->buf[i]==' ')
		{
			pos[k]=i;
			k++;
		}
		
		if(r->buf[i]=='\r' && r->buf[i+1]=='\n')
		{
			pos[k]=i;
			break;
		}
	}


	r->method = (char *)malloc(12);
	memset(r->method,0,12);
	cstrcpy(r->method,r->buf,0,pos[0]);
	

	r->request_uri = (char *)malloc(128);
	memset(r->request_uri,0,128);
	cstrcpy(r->request_uri,r->buf,pos[0]+1,pos[1]-pos[0]-1);

	
	r->protocol = (char *)malloc(12);
	memset(r->protocol,0,12);
	cstrcpy(r->protocol,r->buf,pos[1]+1,pos[2]-pos[1]-1);

	r->request_header_fields_pos = pos[2]+2;
}

int http_request_header_fields_parse(struct http_request_r *r)
{
	int i,m;
	int request_header_fields_length;
	int crlf_pos[100]={0};
	int colon_pos[100]={0};
	
	struct http_request_fields *f[100],*p;

	m=0;
	for(i=r->request_header_fields_pos;i<r->content_pos;i++)
	{
		if(r->buf[i]==':')
		{
			colon_pos[m]=i;
		}
		
		if(r->buf[i]=='\r' && r->buf[i+1]=='\n')
		{
			crlf_pos[m]=i;
			m++;		
		}
	
		if(r->buf[i]=='\r' && r->buf[i+1]=='\n' && r->buf[i+2]=='\r' && r->buf[i+3]=='\n')
		{
			break;
		}	
	}
	
	for(i=0;i<m;i++)
	{
		if(1)
		{
			f[i] = (struct http_request_fields *)malloc(sizeof(struct http_request_fields));
			memset(f[i],0,sizeof(struct http_request_fields));
			
			f[i]->key = (char *)malloc(128);
			memset(f[i]->key,0,128);

			f[i]->val = (char *)malloc(128);
			memset(f[i]->val,0,128);
		
			if(i==0){
				cstrcpy(f[i]->key,r->buf,r->request_header_fields_pos,colon_pos[i]-r->request_header_fields_pos);
			}else{
                        	cstrcpy(f[i]->key,r->buf,crlf_pos[i-1]+2,colon_pos[i]-(crlf_pos[i-1]+2));
			}
                       	cstrcpy(f[i]->val,r->buf,colon_pos[i]+1,crlf_pos[i]-colon_pos[i]);
			
		//	printf("%s:%s\n",f[i]->key,f[i]->val);
		}
		
	}	
	

}
