#include "../include/proxy.h"

static void set_nonblocking(int sock) {
	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

//TODO, once specific setting options are created we will need to add more functionality
static void set_socket_options(int server_socket) {
	int opt = 1;
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("void"); //TODO, error message
		exit(EXIT_FAILURE);
	}
}

static int create_socket_type(bool v4_type) {
	if(v4_type) {
		return socket(AF_INET, SOCK_STREAM, 0);
	}
	else {
		return socket(AF_INET6, SOCK_STREAM, 0);
}

static struct sockaddr_in create_v4_struct(unsigned short port, char* address) {
	struct sockaddr_in server_address;

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	if(inet_pton(AF_INET, address, &server_address.sin_addr) <= 0) {
		perror(""); //TODO, error message
		exit(EXIT_FAILURE);
	}

	return server_address;
}

static struct sockaddr_in6 create_v6_struct(unsigned short port, char* address) {
	struct sockaddr_in6 server_address;

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET6;
	server_address.sin_port = htons(port);

	if(inet_pton(AF_INET6, address, &server_address.sin_addr) <= 0) {
		perror("void"); //TODO, error message
		exit(EXIT_FAILURE);
	}

	return server_address;
}

static struct ds_settings_t socket_setup() {
	struct ds_settings_t socket_settings;

	socket_settings
}

void create_proxy_event() {



	if(server_socket < 0) {
		perror("void"); //TODO, error message
		exit(EXIT_FAILURE);
	}

	set_socket_options(&server_socket);

	struct sockaddr_in server_addr = create_address_struct();
}

void main_event_loop() {

}
