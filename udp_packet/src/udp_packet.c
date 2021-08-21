#include "../include/udp_packet.h"

ErrorCode udp_packet_init(udp_packet_t *packet) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (packet == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    memset(packet->packet_buffer, 0, PACKET_BUFFER_SIZE);
    packet->eth_header = (struct ethhdr *)(packet->packet_buffer);
    packet->ip_header = (struct iphdr *)(packet->packet_buffer + ETH_SIZE);
    packet->udp_header = (struct udphdr *)(packet->packet_buffer + ETH_SIZE + IP_SIZE);
    packet->udp_data = (packet->packet_buffer + ETH_SIZE + IP_SIZE + UDP_SIZE);

cleanup:
    return error_code;
}

ErrorCode udp_packet_set_eth(udp_packet_t *packet, uint8_t *src_mac, uint8_t *dest_mac) {
    ErrorCode error_code = ERROR_SUCCESS;
    int i;

    if (packet == NULL || src_mac == NULL || dest_mac == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    for (i = 0; i < MAC_SIZE; i++) {
        packet->eth_header->h_source[i] = src_mac[i];
        packet->eth_header->h_source[i] = dest_mac[i];
    }

    packet->eth_header->h_proto = htons(ETH_P_IP);

cleanup:
    return error_code;
}

ErrorCode udp_packet_set_ip(udp_packet_t *packet, uint8_t *src_ip, uint8_t *dest_ip) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (packet == NULL || src_ip == NULL || dest_ip == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    packet->ip_header->ihl = 5;
    packet->ip_header->version = 4;
    packet->ip_header->tos = 16;
    packet->ip_header->id = htons(54321);
    packet->ip_header->ttl = 64;
    packet->ip_header->protocol = 17;
    packet->ip_header->saddr = inet_addr((char *)src_ip);
    packet->ip_header->daddr = inet_addr((char *)dest_ip);

cleanup:
    return error_code;
}

ErrorCode udp_packet_set_udp(udp_packet_t *packet, size_t src_port, size_t dest_port) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (packet == NULL || src_port == 0 || dest_port == 0) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    packet->udp_header->source = htons(src_port);
    packet->udp_header->dest = htons(dest_port);
    packet->udp_header->check = 0;

cleanup:
    return error_code;
}

ErrorCode udp_packet_set_data(udp_packet_t *packet, uint8_t *data, size_t data_size) {
    ErrorCode error_code = ERROR_SUCCESS;
    size_t total_size;

    if (packet == NULL || data == NULL || data_size <= 0 || data_size > PACKET_BUFFER_SIZE - ETH_SIZE - IP_SIZE - UDP_SIZE) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    strncpy((char *)packet->udp_data, (char *)data, data_size);
    total_size = ETH_SIZE + IP_SIZE + UDP_SIZE + data_size;
    packet->ip_header->tot_len = htons(total_size - ETH_SIZE);
    packet->udp_header->len = htons(total_size - ETH_SIZE - IP_SIZE);

cleanup:
    return error_code;
}

ErrorCode udp_packet_destroy(udp_packet_t *packet) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (packet == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

cleanup:
    return error_code;
}

ErrorCode udp_packet_init_from_buffer(udp_packet_t *packet, uint8_t *packet_buffer, size_t buffer_size) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (packet == NULL || packet_buffer == NULL || buffer_size == 0) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    memcpy(packet->packet_buffer, packet_buffer, buffer_size);
    packet->eth_header = (struct ethhdr *)(packet->packet_buffer);
    packet->ip_header = (struct iphdr *)(packet->packet_buffer + ETH_SIZE);
    packet->udp_header = (struct udphdr *)(packet->packet_buffer + ETH_SIZE + IP_SIZE);
    packet->udp_data = (packet->packet_buffer + ETH_SIZE + IP_SIZE + UDP_SIZE);

cleanup:
    return error_code;
}

bool udp_packet_is_mine(udp_packet_t *packet, uint16_t port) {
    bool is_mine = false;

    if (packet == NULL)
        return false;

    if (packet->eth_header->h_proto == htons(ETH_P_IP))
        if (packet->ip_header->protocol == IPPROTO_UDP)
            if (ntohs(packet->udp_header->uh_dport) == port)
                is_mine = true;

    return is_mine;
}

uint8_t *udp_packet_get_data(udp_packet_t *packet) {
    if (packet == NULL)
        return NULL;

    return packet->udp_data;
}
