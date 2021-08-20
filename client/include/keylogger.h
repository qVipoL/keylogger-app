#ifndef __KEYLOGGER_H__
#define __KEYLOGGER_H__

#include "../include/std_include.h"
#include "error_code.h"

typedef struct keylogger {
    int fd;
} keylogger_t;

ErrorCode keylogger_init(keylogger_t *keylogger);
ErrorCode keylogger_read_key_event(keylogger_t *keylogger, struct input_event *event);
char *keylogger_parse_key_event(struct input_event *event);
ErrorCode keylogger_destroy(keylogger_t *keylogger);

#endif