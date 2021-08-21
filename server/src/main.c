#include "../include/server.h"
#include "../include/std_include.h"

#define PORT 3040
#define NET_INTERFACE_IDX 2

int main() {
    ErrorCode error_code = ERROR_SUCCESS;
    server_t server;

    if ((error_code = server_init(&server, NET_INTERFACE_IDX)) != ERROR_SUCCESS)
        goto cleanup;

    if ((error_code = server_start(&server, PORT)) != ERROR_SUCCESS)
        goto cleanup;

cleanup:
    if (error_code != ERROR_SUCCESS)
        fprintf(stderr, "error: code %d\n", error_code);

    server_destroy(&server);

    return error_code;
}