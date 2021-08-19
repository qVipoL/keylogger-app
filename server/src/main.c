#include "../include/server.h"
#include "../include/std_include.h"

int main() {
    ErrorCode error_code = ERROR_SUCCESS;

    if ((error_code = server_start()) != ERROR_SUCCESS)
        goto cleanup;

cleanup:
    if (error_code != ERROR_SUCCESS)
        fprintf(stderr, "error: code %d", error_code);

    return error_code;
}