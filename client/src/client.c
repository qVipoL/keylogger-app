#include "../include/client.h"

#include "../include/std_include.h"

#define ETH_HDRLEN 14  // Ethernet header length
#define IP4_HDRLEN 20  // IPv4 header length
#define UDP_HDRLEN 8   // UDP header length, excludes data
#define MTU_SZ 1       // Unit is byte.
#define BUFFER_SIZE 256

#define NET_INTERFACE_IDX 2

#define MY_IP "10.0.2.15"
#define MY_PORT 4040
#define MY_MAC1 0x08
#define MY_MAC2 0x00
#define MY_MAC3 0x27
#define MY_MAC4 0xc6
#define MY_MAC5 0xda
#define MY_MAC6 0x8a

#define SERVER_IP "10.0.2.15"
#define SERVER_PORT 3040
#define SERVER_MAC1 0x08
#define SERVER_MAC2 0x00
#define SERVER_MAC3 0x27
#define SERVER_MAC4 0xc6
#define SERVER_MAC5 0xda
#define SERVER_MAC6 0x8a

static ErrorCode build_packet(uint8_t *buffer);

ErrorCode client_start() {
    ErrorCode error_code = ERROR_SUCCESS;
    struct sockaddr_ll sadr_ll;
    int raw_sd;
    ssize_t data_size;
    uint8_t buffer[BUFFER_SIZE];

    if ((raw_sd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        error_code = ERROR_SOCKET;
        goto cleanup;
    }

    sadr_ll.sll_ifindex = NET_INTERFACE_IDX;
    sadr_ll.sll_halen = ETH_ALEN;
    sadr_ll.sll_addr[0] = SERVER_MAC1;
    sadr_ll.sll_addr[1] = SERVER_MAC2;
    sadr_ll.sll_addr[2] = SERVER_MAC3;
    sadr_ll.sll_addr[3] = SERVER_MAC4;
    sadr_ll.sll_addr[4] = SERVER_MAC5;
    sadr_ll.sll_addr[5] = SERVER_MAC6;

    printf("sending data...\n");

    for (;;) {
        memset(buffer, 0, BUFFER_SIZE);
        build_packet(buffer);

        if ((data_size = sendto(raw_sd, buffer, BUFFER_SIZE, 0, (const struct sockaddr *)&sadr_ll, sizeof(struct sockaddr_ll))) < 0) {
            error_code = ERROR_SEND;
            goto cleanup;
        }

        break;
    }

cleanup:
    if (raw_sd != -1 && close(raw_sd) == -1)
        perror("close() fail.");

    return error_code;
}

static ErrorCode build_packet(uint8_t *buffer) {
    ErrorCode error_code = ERROR_SUCCESS;
    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    size_t total_size = 0;

    eth_header->h_source[0] = MY_MAC1;
    eth_header->h_source[1] = MY_MAC2;
    eth_header->h_source[2] = MY_MAC3;
    eth_header->h_source[3] = MY_MAC4;
    eth_header->h_source[4] = MY_MAC5;
    eth_header->h_source[5] = MY_MAC6;

    eth_header->h_dest[0] = SERVER_MAC1;
    eth_header->h_dest[1] = SERVER_MAC2;
    eth_header->h_dest[2] = SERVER_MAC3;
    eth_header->h_dest[3] = SERVER_MAC4;
    eth_header->h_dest[4] = SERVER_MAC5;
    eth_header->h_dest[5] = SERVER_MAC6;

    eth_header->h_proto = htons(ETH_P_IP);

    total_size += sizeof(struct ethhdr);

    struct iphdr *ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 16;
    ip_header->id = htons(54321);
    ip_header->ttl = 64;
    ip_header->protocol = 17;
    ip_header->saddr = inet_addr(MY_IP);
    ip_header->daddr = inet_addr(SERVER_IP);

    total_size += sizeof(struct iphdr);

    struct udphdr *udp_header = (struct udphdr *)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));

    udp_header->source = htons(MY_PORT);
    udp_header->dest = htons(SERVER_PORT);
    udp_header->check = 0;

    total_size += sizeof(struct udphdr);

    buffer[total_size++] = 'a';
    buffer[total_size++] = 'b';
    buffer[total_size++] = 'c';

    udp_header->len = htons(total_size - sizeof(struct iphdr) - sizeof(struct ethhdr));

    ip_header->tot_len = htons(total_size - sizeof(struct ethhdr));

    printf("%s\n", (buffer + total_size - 3));
    return error_code;
}