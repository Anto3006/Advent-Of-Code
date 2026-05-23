CC = gcc
CFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow -Wwrite-strings -std=c2x -g
BUILD_DIR := build
EXEC_DIR := bin
SRC_DIR := src
LIB_DIR := lib

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
SRC_OBJ := $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
LIB_FILES := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ := $(LIB_FILES:$(LIB_DIR)/%.c=$(BUILD_DIR)/%.o)


all: $(EXEC_DIR) $(BUILD_DIR) $(LIB_OBJ) $(SRC_OBJ)
	$(CC) $(CFLAGS) $(LIB_OBJ) $(SRC_OBJ) -o $(EXEC_DIR)/main

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC_DIR):
	mkdir $(EXEC_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -r $(EXEC_DIR)
	rm -r $(BUILD_DIR)
