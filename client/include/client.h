#ifndef __CLIENT_H__
#define __CLIENT_H__

typedef enum {
    ERROR_SUCCESS = 0,
    ERROR_SOCKET,
    ERROR_SEND,
    ERROR_INVALID_ARGS
} ErrorCode;

ErrorCode client_start();

#endif