#include "map.h"
#include "string.h"

#include "stdlib.h"
#include "stdio.h"

const size_t BASE = 31;

static size_t polynomial_rolling_hash(const char *string){
	size_t hash = 0;
	size_t pos = 0;
	while(string[pos] != '\0'){
		hash = hash * BASE;
		hash = hash + string[pos];
		pos += 1;
	}
	return hash;
}

static int append_at_end(LinkedListPairElement *node, const char* key, const char* value){
	if (is_str_eq(node->key, key)){
		free(node->value);
		node->value = str_copy(value);
		return 0;
	} else if (node->next == NULL){
		LinkedListPairElement *new_element = malloc(sizeof(LinkedListPairElement));
		if (new_element == NULL){
			return 1;
		}
		new_element->key = str_copy(key);
		new_element->value = str_copy(value);
		new_element->next = NULL;
		node->next = new_element;
		return 0;
	} else {
		return append_at_end(node->next,key,value);
	}
}

static int append(LinkedListPair *list, const char *key, const char *value){
	if (list->head == NULL){
		LinkedListPairElement *new_element = malloc(sizeof(LinkedListPairElement));
		if (new_element == NULL){
			return 1;
		}
		new_element->key = str_copy(key);
		new_element->value = str_copy(value);
		new_element->next = NULL;
		list->head=new_element;
		return 0;
	} else {
		int error_code = append_at_end(list->head, key, value);
		return error_code;
	}
}


static char* get_value_linked_list(LinkedListPair *list, const char *key){
	LinkedListPairElement *current = list->head;
	bool key_found = false;
	while(current != NULL && !key_found){
		if (is_str_eq(key,current->key)){
			key_found = true;
			return current->value;
		}
		current = current->next;
	}
	return 0;
}

static bool is_key_in_linked_list(LinkedListPair *list, const char* key){
	LinkedListPairElement *current = list->head;
	bool key_found = false;
	while(current != NULL && !key_found){
		if (is_str_eq(key,current->key)){
			return true;
		}
		current = current->next;
	}
	return false;
}

static bool is_linked_list_empty(LinkedListPair *list){
	return list->head == NULL;
}

static void free_linked_list_element(LinkedListPairElement *node){
	free(node->key);
	free(node->value);
	if (node->next != NULL){
		free_linked_list_element(node->next);
	}
	free(node);
}

static void free_linked_list(LinkedListPair *list){
	if (list->head != NULL){
		free_linked_list_element(list->head);
	}
}

static void free_vector_linked_lists(VectorLinkedList *vector){
	for(size_t pos = 0; pos < vector->length; ++pos){
		free_linked_list(&vector->array[pos]);
	}
	free(vector->array);
}

int insert(MapStringString *map, const char *key, const char *value){
	int error_code = 0;
	size_t hash_value = map->hash_function(key) % map->hash_table.length;
	error_code = append(&map->hash_table.array[hash_value], key, value);
	return error_code;
}

char* get_value(MapStringString *map, const char *key){
	size_t hash_value = map->hash_function(key) % map->hash_table.length;
	char *value = get_value_linked_list(&map->hash_table.array[hash_value], key);
	return value;
}
bool contains_key(MapStringString *map, const char *key){
	size_t hash_value = map->hash_function(key) % map->hash_table.length;
	bool is_key_contained = is_key_in_linked_list(&map->hash_table.array[hash_value], key);
	return is_key_contained;
}
bool is_empty(MapStringString *map){
	for(size_t pos = 0; pos < map->hash_table.length; ++pos){
		if(!is_linked_list_empty(&map->hash_table.array[pos])){
			return false;
		}
	}
	return true;
}

MapStringString new_map(size_t hash_size){
	VectorLinkedList hash_table;
	hash_table.length = hash_size;
	hash_table.array = calloc(hash_size,sizeof(LinkedListPair));
	for(size_t pos = 0; pos < hash_size; ++pos){
		hash_table.array[pos].head =NULL;
	}
	MapStringString map;
	map.hash_function = &polynomial_rolling_hash;
	map.hash_table = hash_table;
	return map;
}

void free_map(MapStringString *map){
	free_vector_linked_lists(&map->hash_table);
}

void show_key_value_pairs(MapStringString *map){
	for(size_t pos = 0; pos < map->hash_table.length; ++pos){
		if(map->hash_table.array[pos].head != NULL){
			LinkedListPairElement *current = map->hash_table.array[pos].head;
			while (current != NULL){
				printf("Key: %s; Value: %s\n",current->key, current->value);
				current = current->next;
			}
		}
	}
}
