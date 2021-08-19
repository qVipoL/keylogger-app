#ifndef __SERVER_H__
#define __SERVER_H__

typedef enum {
    ERROR_SUCCESS = 0,
    ERROR_SOCKET,
    ERROR_BIND,
    ERROR_READ,
    ERROR_INVALID_ARGS
} ErrorCode;

ErrorCode server_start();

#endif