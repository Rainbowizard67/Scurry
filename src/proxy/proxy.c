#include "../include/proxy.h"

static void set_nonblocking(int sock) {
	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

void create_proxy_event() {


}

void main_event_loop(int epoll_fd, int server_soc) {

}
