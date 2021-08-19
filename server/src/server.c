#include "../include/server.h"

#include "../include/std_include.h"

#define NET_INTERFACE_IDX 2
#define PORT 3040
#define BUFFER_SIZE 256

#define MY_MAC1 0xff
#define MY_MAC2 0xff
#define MY_MAC3 0xff
#define MY_MAC4 0xff
#define MY_MAC5 0xff

static ErrorCode parse_ip_header(uint8_t *buffer, int port);
static ErrorCode parse_udp_header(uint8_t *buffer, int port, size_t data_size, int has_more_frag, int offset);
static void print_udp_data(uint8_t *buffer, size_t data_size);
static ErrorCode server_recieve_packet(int raw_sd, int port);

uint8_t out_buf[1024];
short seq_num;

ErrorCode server_start() {
    ErrorCode error_code = ERROR_SUCCESS;
    int raw_sd;
    struct sockaddr_ll sll;

    if ((raw_sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        error_code = ERROR_SOCKET;
        goto cleanup;
    }

    memset(&sll, 0, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = NET_INTERFACE_IDX;
    sll.sll_protocol = htons(ETH_P_ALL);

    if (bind(raw_sd, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
        error_code = ERROR_BIND;
        goto cleanup;
    }

    printf("server started.\n");
    while ((error_code = server_recieve_packet(raw_sd, PORT)) == ERROR_SUCCESS)
        ;

cleanup:
    if (raw_sd != -1 && close(raw_sd) == -1)
        perror("close() fail.");

    return error_code;
}

static ErrorCode server_recieve_packet(int raw_sd, int port) {
    ErrorCode error_code = ERROR_SUCCESS;
    ssize_t data_size;
    uint8_t buffer[BUFFER_SIZE];
    struct ethhdr eth_header;

    memset(buffer, 0, BUFFER_SIZE * sizeof(uint8_t));

    if ((data_size = read(raw_sd, buffer, BUFFER_SIZE)) < 0) {
        error_code = ERROR_READ;
        goto cleanup;
    }

    memcpy(&eth_header, buffer, sizeof(struct ethhdr));

    if (eth_header.h_proto == htons(ETH_P_IP))
        if ((error_code = parse_ip_header(buffer + sizeof(eth_header), port)) != ERROR_SUCCESS)
            goto cleanup;

cleanup:
    return error_code;
}

static ErrorCode parse_ip_header(uint8_t *buffer, int port) {
    ErrorCode error_code = ERROR_SUCCESS;
    struct ip ip_header;

    memcpy(&ip_header, buffer, sizeof(ip_header));

    if (ip_header.ip_p == IPPROTO_UDP) {
        unsigned short frag_field = ntohs(ip_header.ip_off);
        int has_more_frag = (frag_field << 2) >> 15;
        short offset = frag_field << 3;
        offset = offset >> 3;

        error_code = parse_udp_header(buffer + 20 * sizeof(uint8_t), port, ntohs(ip_header.ip_len) - sizeof(struct udphdr),
                                      has_more_frag, offset);

        if (error_code != ERROR_SUCCESS)
            goto cleanup;

        seq_num = offset;
    }

cleanup:
    return error_code;
}

static ErrorCode parse_udp_header(uint8_t *buffer, int port, size_t data_size, int has_more_frag, int offset) {
    ErrorCode error_code = ERROR_SUCCESS;
    struct udphdr udp_header;
    memcpy(&udp_header, buffer, sizeof(udp_header));

    if (ntohs(udp_header.uh_dport) == (uint16_t)port) {
        if (has_more_frag) {
            printf("2\n");
            memcpy(out_buf + offset * data_size, buffer + sizeof(struct udphdr), data_size);
        } else if (seq_num != offset) {
            printf("3\n");
            memcpy(out_buf + offset * data_size, buffer + sizeof(struct udphdr), data_size);
            print_udp_data(out_buf, (offset + 1) * data_size);
            seq_num = 0;
        }
    }

    return error_code;
}

static void print_udp_data(uint8_t *buffer, size_t data_size) {
    size_t i;

    for (i = 0; i < data_size; i++)
        printf("%c", buffer[i]);
}
