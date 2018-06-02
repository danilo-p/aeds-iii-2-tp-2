SHELL:=/bin/bash -O globstar

BIN_NAME=tp2
BIN_DIR=.
OBJ_DIR=./obj
SRC_DIR=./src
SRC=$(wildcard $(SRC_DIR)/**/*.c)

CFLAGS += -lm -Wall -Wextra -Werror

.PHONY: all

all: setup main

main: main.o
	gcc $(SRC) $(OBJ_DIR)/main.o -o $(BIN_DIR)/$(BIN_NAME) $(CFLAGS)

main.o:
	gcc -c $(SRC_DIR)/main.c -g -o $(OBJ_DIR)/main.o $(CFLAGS)

setup:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

run:
	$(BIN_DIR)/$(BIN_NAME)

clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/$(BIN_NAME)

valgrind:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$(BIN_NAME)
