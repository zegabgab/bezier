CC = gcc
CFLAGS = -Wall
SRC = src
TARGET = target
OBJ = $(TARGET)/obj
BIN = $(TARGET)/bin
MAIN = $(BIN)/bezier
SRCS = $(shell find $(SRC)/ -type f -name '*.c')
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: main

.PHONY: clean
clean:
	rm -rf $(TARGET)

main: $(MAIN)

$(MAIN): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $?
