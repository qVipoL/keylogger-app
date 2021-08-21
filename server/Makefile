CC 				:= gcc
SHELL			:= /bin/bash
CFLAGS 			:= -g -Wall -Werror -Wextra
LDFLAGS 		:=

# general
SRC_DIR 		:= src
INCLUDE_DIR		:= include
OUT_DIR			:= out

# project
BIN_DIR			:= $(OUT_DIR)/bin
OBJ_DIR 		:= $(OUT_DIR)/obj
PROGRAM			:= keylogger-server
TARGET			:= $(BIN_DIR)/$(PROGRAM)

HEADERS			:= $(wildcard $(INCLUDE_DIR)/*.h)
SOURCES			:= $(wildcard $(SRC_DIR)/*c)
OBJECTS 		:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# udp_packet
UDP_SRC_DIR 	:= ../udp_packet/$(SRC_DIR)
UDP_INCLUDE_DIR := ../udp_packet/$(INCLUDE_DIR)

UDP_HEADERS		:= $(wildcard $(UDP_INCLUDE_DIR)/.*h)
UDP_SOURCES		:= $(wildcard $(UDP_SRC_DIR)/*.c)
UDP_OBJECTS		:= $(patsubst $(UDP_SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(UDP_SOURCES))

.PHONY: all clean build_env

all: build_env $(TARGET)

build_env: $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(UDP_SRC_DIR)/%.c $(UDP_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS) $(UDP_OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	-rm -rf $(OUT_DIR)