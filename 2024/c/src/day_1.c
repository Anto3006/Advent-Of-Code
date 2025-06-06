#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/utilities.h"
#include "../lib/vector.h"

void load_values(char*, vector_uint_t*, vector_uint_t*);
uint32_t distance(uint32_t first, uint32_t second);
uint32_t count_apparitions(vector_uint_t*, uint32_t);
uint32_t calculate_similarity(vector_uint_t*, vector_uint_t*);


int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);

		vector_uint_t first_vector;
		vector_uint_t second_vector;
		initialize_vector_uint(&first_vector);
		initialize_vector_uint(&second_vector);

		load_values(data, &first_vector, &second_vector);

		quicksort(&first_vector);
		quicksort(&second_vector);

		uintmax_t sum = 0;

		for(size_t index = 0; index < first_vector.length; ++index){
			sum += distance(first_vector.data[index], second_vector.data[index]);
		}

		uint32_t similarity = calculate_similarity(&first_vector, &second_vector);


		free_vector_uint(&second_vector);
		free_vector_uint(&first_vector);
		free(data);

		printf("Result first part: %lu\n",sum);
		printf("Result second part: %u\n",similarity);
	}
	return 0;
}

void load_values(char* file_content, vector_uint_t* first_vector, vector_uint_t* second_vector){
	vector_str_t lines = split_string(file_content,"\n");
	for(size_t pos = 0; pos < lines.length; ++pos){
		char* line = lines.data[pos];
		vector_str_t numbers = split_string(line, " ");
		uint32_t first_number = atoi(numbers.data[0]);
		uint32_t second_number = atoi(numbers.data[1]);
		append_vector_uint(first_vector,first_number);
		append_vector_uint(second_vector,second_number);
		free_vector_str(&numbers);
	}
	free_vector_str(&lines);
}

uint32_t distance(uint32_t first, uint32_t second){
	if(first >= second){
		return first - second;
	} else {
		return second - first;
	}
}

uint32_t count_apparitions(vector_uint_t* vector, uint32_t element){
	uint32_t apparitions = 0;
	for(size_t index = 0; index < vector->length; ++index){
		if(vector->data[index] == element){
			apparitions += 1;
		}
	}
	return apparitions;
}

uint32_t calculate_similarity(vector_uint_t* first_vector, vector_uint_t* second_vector){
	uint32_t similarity = 0;
	for(size_t index = 0; index < first_vector->length; ++index){
		uint32_t element = first_vector->data[index];
		uint32_t apparitions = count_apparitions(second_vector, element);
		similarity += element * apparitions;
	}
	return similarity;
}
