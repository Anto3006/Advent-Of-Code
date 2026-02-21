CC = gcc
CFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow -Wwrite-strings -std=c2x -g
BUILD_DIR := ./build
EXEC_DIR := ./bin

all: \
	map \
	argument_reader \
	string \
	utilities \
	day_01 \
	day_02 \
	day_03 \
	day_04 \
	solution \
	main

map: lib/map.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

argument_reader: lib/argument_reader.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

string: lib/string.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

utilities: lib/utilities.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

day_01: src/day_01.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

day_02: src/day_02.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

day_03: src/day_03.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

day_04: src/day_04.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

solution: src/solution.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o

main: src/main.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/$@.o $(BUILD_DIR)/map.o $(BUILD_DIR)/argument_reader.o $(BUILD_DIR)/string.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/day_01.o $(BUILD_DIR)/day_02.o $(BUILD_DIR)/day_03.o $(BUILD_DIR)/day_04.o $(BUILD_DIR)/solution.o -o $(EXEC_DIR)/$@
