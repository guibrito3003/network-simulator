CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj
BIN     = netsim

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean run

all: $(OBJ_DIR) $(BIN)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $

run: all
	./$(BIN) tests/teste.txt

clean:
	rm -rf $(OBJ_DIR) $(BIN)