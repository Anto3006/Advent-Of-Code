#include "vector.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


void initialize_vector_uint(vector_uint_t* vector){
	vector->capacity = 1;
	vector->length = 0;
	vector->data = calloc(1, sizeof(uint32_t));
}

void free_vector_uint(vector_uint_t* vector){
	vector->capacity = 0;
	vector->length = 0;
	free(vector->data);
}
void resize_vector_uint(vector_uint_t* vector){
	vector->capacity = GROWTH_RATE * vector->capacity;
	vector->data = realloc(vector->data, vector->capacity * sizeof(uint32_t));
}

void append_vector_uint(vector_uint_t* vector, const uint32_t new_element){
	if(vector->capacity == vector->length){
		resize_vector_uint(vector);
	}
	vector->data[vector->length] = new_element;
	vector->length += 1;
}

void quicksort(vector_uint_t* vector){
	_quicksort_aux(vector->data,0,vector->length-1);
}

void _quicksort_aux(uint32_t *array, size_t lo, size_t hi){
	if (lo >= hi){
		return;
	} else {
		size_t partition_index = _quicksort_partition_aux(array, lo, hi);
		if(partition_index == 0){
			_quicksort_aux(array, lo, 0);
		} else{
			_quicksort_aux(array, lo, partition_index-1);
		}
		_quicksort_aux(array, partition_index+1, hi);
	}
}

size_t _quicksort_partition_aux(uint32_t *array, size_t lo, size_t hi){
	uint32_t pivot = array[hi];

	size_t pivot_index = lo;

	uint32_t swap_aux = 0;

	for(size_t index = lo; index < hi; ++index){
		if(array[index] <= pivot){
			swap_aux = array[index];
			array[index] = array[pivot_index];
			array[pivot_index] = swap_aux;
			pivot_index++;
		}
	}
	swap_aux = array[hi];
	array[hi] = array[pivot_index];
	array[pivot_index] = swap_aux;
	return pivot_index;
}

bool is_in_vector_uint(vector_uint_t* vector, uint32_t element){
	for(size_t index = 0; index < vector->length; ++index){
		if(vector->data[index] == element){
			return true;
		}
	}
	return false;
}

void initialize_vector_str(vector_str_t* vector){
	vector->capacity = 1;
	vector->length = 0;
	vector->data = calloc(1, sizeof(char*));
}

void free_vector_str(vector_str_t* vector){
	vector->capacity = 0;
	for(size_t pos = 0; pos < vector->length; ++pos){
		free(vector->data[pos]);
	}
	vector->length = 0;
	free(vector->data);
}

void resize_vector_str(vector_str_t* vector){
	vector->capacity = GROWTH_RATE * vector->capacity;
	vector->data = realloc(vector->data, vector->capacity * sizeof(char*));
}

void append_vector_str(vector_str_t* vector, char* new_element){
	if(vector->capacity == vector->length){
		resize_vector_str(vector);
	}
	vector->data[vector->length] = new_element;
	vector->length += 1;
}

vector_uint_t parse_vector_uint(vector_str_t* vector){
	vector_uint_t parsed_vector;
	initialize_vector_uint(&parsed_vector);
	for(size_t index = 0; index < vector->length; ++index){
		uint32_t element = atoi(vector->data[index]);
		append_vector_uint(&parsed_vector, element);
	}
	return parsed_vector;
}
