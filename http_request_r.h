typedef struct http_request_r
{
        char *method;
        char *http;
        char *request_uri;

        unsigned int content_length;

        char *content;

	char buf[1024];	

        int is_end;

        int fd;

	int pos;

        struct http_request_r *plast;
        struct http_request_r *pnext;

}http_request_r;

int add_http_request(struct http_request_r **r,int fd);

int delete_http_request(struct http_request_r *r,int fd);

http_request_r* find_http_request(struct http_request_r *r,int fd);

void print_http_request(struct http_request_r *r);

int count_http_request(struct http_request_r *r);


