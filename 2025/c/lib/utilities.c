#include "utilities.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char* read_file(const char* file_path, const uint32_t initial_size, const uint8_t growth_rate){
	char* file_content = (char*) calloc(initial_size, sizeof(char));

	char buffer[CHUNK_SIZE];

	uint32_t current_size = initial_size;

	if (file_content == NULL){
		printf("Error in initial memory allocation\n");
		exit(EXIT_FAILURE);
	}

	FILE* file_stream = fopen(file_path, "r");
	
	if (file_stream == NULL){
		printf("Error in opening of file\n");
		exit(EXIT_FAILURE);
	}

	bool found_end_file = false;
	
	size_t buffer_pos = 0;
	size_t current_pos = 0;
	
	while(!found_end_file){
		char ch = fgetc(file_stream);
		if (ch == (char)EOF){
			found_end_file = true;
		} else {
			buffer[buffer_pos] = ch;
		}
		buffer_pos += 1;
		if(buffer_pos == CHUNK_SIZE || found_end_file){
			if(buffer_pos + current_pos >= current_size){
				while (buffer_pos + current_pos >= current_size){
					current_size *= growth_rate;
				}
				file_content = (char*)realloc(file_content, current_size * sizeof(char));
				if (file_content == NULL){
					printf("Error in initial memory allocation\n");
					exit(EXIT_FAILURE);
				}
			}
			for (size_t pos = 0; pos < buffer_pos; pos++){
				file_content[current_pos + pos] = buffer[pos];
			}
			current_pos += buffer_pos;
			buffer_pos = 0;
		}
	}
	file_content[current_pos-1] = '\0';
	fclose(file_stream);
	return file_content;
}

char* get_string_slice(const char* str, const size_t pos, const size_t length){
	char* str_slice = calloc(length+1,sizeof(char));
	for(size_t i = 0; i < length; i++){
		str_slice[i] = str[pos+i];
	}
	str_slice[length] = '\0';
	return str_slice;
}

