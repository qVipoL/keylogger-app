#include "../include/client.h"

#include "../include/std_include.h"
#include "../include/udp_packet.h"

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

// static ErrorCode build_packet(uint8_t *buffer);

ErrorCode client_start() {
    ErrorCode error_code = ERROR_SUCCESS;
    struct sockaddr_ll sadr_ll;
    int raw_sd;
    udp_packet_t packet;
    uint8_t src_mac[] = {MY_MAC1, MY_MAC2, MY_MAC3, MY_MAC4, MY_MAC5, MY_MAC6};
    uint8_t dest_mac[] = {SERVER_MAC1, SERVER_MAC2, SERVER_MAC3, SERVER_MAC4, SERVER_MAC5, SERVER_MAC6};
    uint8_t src_ip[] = MY_IP, dest_ip[] = SERVER_IP;
    ssize_t data_size;

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

    udp_packet_init(&packet);
    udp_packet_set_eth(&packet, src_mac, dest_mac);
    udp_packet_set_ip(&packet, src_ip, dest_ip);
    udp_packet_set_udp(&packet, MY_PORT, SERVER_PORT);

    for (;;) {
        // memset(buffer, 0, BUFFER_SIZE);
        // build_packet(buffer);

        udp_packet_set_data(&packet, (uint8_t *)"abc", 3);

        if ((data_size = sendto(raw_sd, packet.packet_buffer, PACKET_BUFFER_SIZE, 0, (const struct sockaddr *)&sadr_ll, sizeof(struct sockaddr_ll))) < 0) {
            error_code = ERROR_SEND;
            goto cleanup;
        }
    }

cleanup:
    if (raw_sd != -1 && close(raw_sd) == -1)
        perror("close() fail.");

    return error_code;
}