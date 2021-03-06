#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

typedef enum {
    ERROR_SUCCESS = 0,
    ERROR_SOCKET,
    ERROR_SEND,
    ERROR_OPEN,
    ERROR_BIND,
    ERROR_CLOSE,
    ERROR_READ,
    ERROR_INVALID_ARGS
} ErrorCode;

#endif