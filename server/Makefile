CC 			:= gcc
SHELL		:= /bin/bash
CFLAGS 		:= -g -Wall -Werror -Wextra
LDFLAGS 	:=

SRC_DIR 	:= src
INCLUDE_DIR	:= include
OUT_DIR		:= out
BIN_DIR		:= $(OUT_DIR)/bin
OBJ_DIR 	:= $(OUT_DIR)/obj
PROGRAM		:= keylogger-server
TARGET		:= $(BIN_DIR)/$(PROGRAM)

HEADERS		:= $(wildcard $(INCLUDE_DIR)/*.h)
SOURCES		:= $(wildcard $(SRC_DIR)/*c)
OBJECTS 	:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: all clean build_env

all: build_env $(TARGET)

build_env: $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	-rm -rf $(OUT_DIR)