#include "../include/client.h"
#include "../include/std_include.h"

#define NET_INTERFACE_IDX 2

#define SRC_IP "10.0.2.15"
#define SRC_PORT 4040
#define SRC_MAC1 0x08
#define SRC_MAC2 0x00
#define SRC_MAC3 0x27
#define SRC_MAC4 0xc6
#define SRC_MAC5 0xda
#define SRC_MAC6 0x8a

#define DEST_IP "10.0.2.15"
#define DEST_PORT 3040
#define DEST_MAC1 0x08
#define DEST_MAC2 0x00
#define DEST_MAC3 0x27
#define DEST_MAC4 0xc6
#define DEST_MAC5 0xda
#define DEST_MAC6 0x8a

int main() {
    ErrorCode error_code = ERROR_SUCCESS;
    logger_client_t client;

    uint8_t src_mac[] = {SRC_MAC1, SRC_MAC2, SRC_MAC3, SRC_MAC4, SRC_MAC5, SRC_MAC6};
    uint8_t dest_mac[] = {DEST_MAC1, DEST_MAC2, DEST_MAC3, DEST_MAC4, DEST_MAC5, DEST_MAC6};
    uint8_t src_ip[] = SRC_IP;
    uint8_t dest_ip[] = DEST_IP;
    size_t src_port = SRC_PORT;
    size_t dest_port = DEST_PORT;

    if ((error_code = client_init(&client, NET_INTERFACE_IDX, dest_mac)) != ERROR_SUCCESS)
        goto cleanup;

    if ((error_code = client_start(&client, src_mac, dest_mac, src_ip, dest_ip, src_port, dest_port)) != ERROR_SUCCESS)
        goto cleanup;

cleanup:
    if (error_code != ERROR_SUCCESS)
        fprintf(stderr, "error: code %d\n", error_code);

    client_destroy(&client);

    return error_code;
}