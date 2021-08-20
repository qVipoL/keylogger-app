#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "error_code.h"
#include "std_include.h"

typedef struct logger_client {
    int raw_sd;
    struct sockaddr_ll server_address;
} logger_client_t;

ErrorCode client_init(logger_client_t *client, uint8_t interface_idx, uint8_t *dest_mac);
ErrorCode client_start(logger_client_t *client, uint8_t *src_mac, uint8_t *dest_mac, uint8_t *src_ip, uint8_t *dest_ip,
                       size_t src_port,
                       size_t dest_port);
ErrorCode client_destroy(logger_client_t *client);

#endif