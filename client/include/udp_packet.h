#ifndef __UDP_PACKET_H__
#define __UDP_PACKET_H__

#include "error_code.h"
#include "std_include.h"

#define PACKET_BUFFER_SIZE 256

#define ETH_SIZE sizeof(struct ethhdr)
#define IP_SIZE sizeof(struct iphdr)
#define UDP_SIZE sizeof(struct udphdr)

#define MAC_SIZE 6

typedef struct udp_packet {
    struct ethhdr *eth_header;
    struct iphdr *ip_header;
    struct udphdr *udp_header;
    uint8_t *udp_data;
    uint8_t packet_buffer[PACKET_BUFFER_SIZE];
} udp_packet_t;

ErrorCode udp_packet_init(udp_packet_t *packet);
ErrorCode udp_packet_set_eth(udp_packet_t *packet, uint8_t *src_mac, uint8_t *dest_mac);
ErrorCode udp_packet_set_ip(udp_packet_t *packet, uint8_t *src_ip, uint8_t *dest_ip);
ErrorCode udp_packet_set_udp(udp_packet_t *packet, size_t src_port, size_t dest_port);
ErrorCode udp_packet_set_data(udp_packet_t *packet, uint8_t *data, size_t data_size);
ErrorCode udp_packet_destroy(udp_packet_t *packet);

#endif