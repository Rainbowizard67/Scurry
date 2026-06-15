#ifndef PROXY_H
#define PROXY_H

#include "project_headers.h"
#include "scurry_utils.h"

#define BACKLOG 1024

typedef struct address_settings {
	int server_socket;
	struct sockaddr_storage address;
	socklen_t address_len;
	int address_family;
} addr_settings;

typedef struct ds_settings {
	struct addr_settings v4;
	struct addr_settings v6;
	bool supports_ds;
} ds_settings_t;

#endif
