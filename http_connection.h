typedef struct http_connection_t
{
	char *buf;
	int keep_alive;
	int fd;
	struct http_request_t *r;

}http_connection_t;
