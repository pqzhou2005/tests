typedef struct http_response_r
{
	char *http_version;
	
	int status_code;
	
	char *reason_phrase;

	char *server;

	char *date;

	char *content_type;

	int content_length;
	
	char *last_modified;

	char *connection;

	char *accept_ranges;

	char *body;

	char *buf;

	int size;

} http_response_r;

int static_http_response_out(struct http_request_r *r,struct http_response_r *o);

int init_http_resonse(struct http_request_r *r, struct http_response_r *o);

int gen_http_response_buf(struct http_response_r *o);

int readfile(char *path, struct http_response_r *o);
