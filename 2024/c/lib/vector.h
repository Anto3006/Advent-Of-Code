#ifndef VECTOR_LIB_H

#define VECTOR_LIB_H

#define GROWTH_RATE 2

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	size_t capacity;
	size_t length;
	uint32_t* data;
} vector_uint_t;

typedef struct {
	size_t capacity;
	size_t length;
	char** data;
} vector_str_t;

void initialize_vector_uint(vector_uint_t* vector);
void free_vector_uint(vector_uint_t* vector);
void resize_vector_uint(vector_uint_t* vector);
void append_vector_uint(vector_uint_t* vector, const uint32_t new_element);
void quicksort(vector_uint_t* vector);
void _quicksort_aux(uint32_t* array, size_t lo, size_t hi);
size_t _quicksort_partition_aux(uint32_t* array, size_t lo, size_t hi);
bool is_in_vector_uint(vector_uint_t* vector, uint32_t element);

void initialize_vector_str(vector_str_t* vector);
void free_vector_str(vector_str_t* vector);
void resize_vector_str(vector_str_t* vector);
void append_vector_str(vector_str_t* vector, char* new_element);
vector_uint_t parse_vector_uint(vector_str_t* vector);

#endif //!VECTOR_LIB_H
