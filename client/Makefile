CC :=gcc
FLAGS :=-g -pthread -Wall
SRC :=src
OBJ :=obj
SRCS :=$(wildcard $(SRC)/*.c)
OBJS :=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BIN_DIR := bin
BIN := $(BIN_DIR)/client

all: dirs $(BIN)

release: FLAGS := -Wall -O2
release: clean
release: dirs $(BIN)

dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ)

$(BIN): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@

# compile project objs
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(BIN_DIR)/* $(OBJ)/*