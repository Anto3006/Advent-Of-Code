#ifndef MAP_H
#define MAP_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct LinkedListPairElement LinkedListPairElement;

struct LinkedListPairElement{
	char *key;
	char *value;
	LinkedListPairElement *next;
};

typedef struct {
	LinkedListPairElement *head;
}LinkedListPair;

typedef struct {
	size_t length;
	LinkedListPair *array;
} VectorLinkedList;

typedef struct {
	size_t (*hash_function)(const char*);
	VectorLinkedList hash_table;
} MapStringString;

int insert(MapStringString *map, const char *key, const char *value);
char* get_value(MapStringString *map, const char *key);
bool contains_key(MapStringString *map, const char *key);
bool is_empty(MapStringString *map);
MapStringString new_map(size_t hash_size);
void free_map(MapStringString *map);
void show_key_value_pairs(MapStringString *map);

#endif
