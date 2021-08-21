#include "../include/server.h"

#include "../../udp_packet/include/udp_packet.h"
#include "../include/std_include.h"

ErrorCode server_init(server_t *server, uint8_t interface_idx) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (server == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if ((server->raw_sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        error_code = ERROR_SOCKET;
        goto cleanup;
    }

    memset(&(server->addr), 0, sizeof(struct sockaddr_ll));
    server->addr.sll_family = AF_PACKET;
    server->addr.sll_ifindex = interface_idx;
    server->addr.sll_protocol = htons(ETH_P_ALL);

    if (bind(server->raw_sd, (struct sockaddr *)&(server->addr), sizeof(struct sockaddr_ll)) < 0) {
        error_code = ERROR_BIND;
        goto cleanup;
    }

cleanup:
    if (error_code != ERROR_SUCCESS && server != NULL && server->raw_sd != -1 && close(server->raw_sd) < 0)
        perror("close()");

    return error_code;
}

ErrorCode server_start(server_t *server, uint16_t port) {
    ErrorCode error_code = ERROR_SUCCESS;
    ssize_t bytes;
    uint8_t packet_buffer[PACKET_BUFFER_SIZE];
    udp_packet_t packet;

    printf("server started...\n");

    for (;;) {
        memset(packet_buffer, 0, PACKET_BUFFER_SIZE);

        if ((bytes = read(server->raw_sd, packet_buffer, PACKET_BUFFER_SIZE)) < 0) {
            error_code = ERROR_READ;
            goto cleanup;
        }

        if ((error_code = udp_packet_init_from_buffer(&packet, packet_buffer, PACKET_BUFFER_SIZE)) != ERROR_SUCCESS)
            goto cleanup;

        if (udp_packet_is_mine(&packet, port))
            printf("%s\n", (char *)udp_packet_get_data(&packet));
    }

    if ((error_code = udp_packet_destroy(&packet)) != ERROR_SUCCESS)
        goto cleanup;

cleanup:
    return error_code;
}

ErrorCode server_destroy(server_t *server) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (server == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if (server->raw_sd == -1 && close(server->raw_sd) < 0) {
        error_code = ERROR_CLOSE;
        goto cleanup;
    }

cleanup:
    return error_code;
}
