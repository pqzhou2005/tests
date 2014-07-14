#define SERVER_PORT 8000
#define BACK_LOG 128

int http_server_socket();

int http_client_socket(int fd);

void onAccept(struct http_event_t *ev);

void onData(struct http_event_t *ev);

void onWrite(struct http_event_t *ev);

int set_non_blocking(int fd);

