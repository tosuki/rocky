all: build/sophix

SRCS= src/main.c src/wm.c src/mouse.c src/window_node.c src/window.c src/keyboard.c src/logger.c

#output
BUILD_DIR := build
BIN := $(BUILD_DIR)/main

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra
LIBS := -lX11

# Create directory if it not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN): $(SRCS) | $(BUILD_DIR)
	$(CC) $(SRCS) -o $(BIN) $(CFLAGS) $(LIBS)

build: $(BIN)

clean:
	rm -rf $(BUILD_DIR)

