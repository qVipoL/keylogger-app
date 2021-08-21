#ifndef __SERVER_H__
#define __SERVER_H__

#include "../../udp_packet/include/error_code.h"
#include "std_include.h"

typedef struct server {
    int raw_sd;
    struct sockaddr_ll addr;
} server_t;

ErrorCode server_init(server_t *server, uint8_t interface_idx);
ErrorCode server_start(server_t *server, uint16_t port);
ErrorCode server_destroy(server_t *server);

#endif