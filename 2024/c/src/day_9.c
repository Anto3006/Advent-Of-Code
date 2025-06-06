#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

vector_uint_t parse_memory_state(char* data);
void fragment_memory(vector_uint_t* memory_state);
uint64_t calculate_checksum(vector_uint_t* memory_state);
void rearrange_memory(vector_uint_t* memory_state);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_uint_t memory_state = parse_memory_state(data);
		//fragment_memory(&memory_state);
		rearrange_memory(&memory_state);
		uint64_t checksum = calculate_checksum(&memory_state);
		//printf("Result part 1: %lu\n", checksum);
		printf("Result part 2: %lu\n", checksum);
		free_vector_uint(&memory_state);
		free(data);
	}
	return 0;
}

vector_uint_t parse_memory_state(char* data){
	vector_uint_t memory_state;
	initialize_vector_uint(&memory_state);
	bool is_empty_space = false;
	uint32_t block_id = 1;
	uint32_t empty_block = 0;
	for(size_t char_index = 0; data[char_index] != '\0'; ++char_index){
		if(data[char_index] < '0' || data[char_index] > '9'){
			break;
		}
		uint8_t block_size = data[char_index] - '0';
		if(!is_empty_space){
			for(size_t i = 0; i < block_size; ++i){
				append_vector_uint(&memory_state, block_id);
			}
			block_id++;
		} else {
			for(size_t i = 0; i < block_size; ++i){
				append_vector_uint(&memory_state, empty_block);
			}
		}
		is_empty_space = !is_empty_space;
	}
	return memory_state;
}

void fragment_memory(vector_uint_t* memory_state){
	size_t first_empty_slot = 0;
	size_t last_memory_slot = memory_state->length;
	while(first_empty_slot < memory_state->length && memory_state->data[first_empty_slot] != 0){
		first_empty_slot++;
	}
	while(last_memory_slot > 0 && memory_state->data[last_memory_slot] == 0){
		last_memory_slot--;
	}
	while(first_empty_slot < last_memory_slot){
		memory_state->data[first_empty_slot] = memory_state->data[last_memory_slot];
		memory_state->data[last_memory_slot] = 0;
		while(first_empty_slot < memory_state->length && memory_state->data[first_empty_slot] != 0){
			first_empty_slot++;
		}
		while(last_memory_slot > 0 && memory_state->data[last_memory_slot] == 0){
			last_memory_slot--;
		}
	}
}

uint64_t calculate_checksum(vector_uint_t* memory_state){
	uint64_t checksum = 0;
	for(size_t memory_index = 0; memory_index < memory_state->length; ++memory_index){
		uint64_t memory = memory_state->data[memory_index];
		if(memory > 0){
			uint64_t added_value = (memory - 1) * memory_index;
			if(checksum > UINT64_MAX-added_value){
				printf("Overflow\n");
			}
			checksum += added_value;
		}
	}
	return checksum;
}

void rearrange_memory(vector_uint_t* memory_state){
	size_t memory_location = memory_state->length - 1;
	uint32_t memory_id = 0;
	while(memory_location > 0){
		size_t block_size = 0;
		while(memory_location > 0 && (memory_state->data[memory_location] == 0 || memory_state->data[memory_location] == memory_id)){
			memory_location--;
		}
		memory_id = memory_state->data[memory_location];
		while(memory_location > 0 && memory_state->data[memory_location] == memory_id){
			memory_location--;
			block_size++;
		}
		size_t free_slot_location = 0;
		size_t free_slot_size = 0;
		while(free_slot_location <= memory_location){
			if(memory_state->data[free_slot_location] == 0){
				free_slot_size++;
			} else {
				free_slot_size = 0;
			}
			if(free_slot_size == block_size){
				break;
			}
			free_slot_location++;
		}
		if(free_slot_size == block_size){
			for(size_t index = 0; index < block_size; ++index){
				memory_state->data[memory_location+1+index] = 0;
				memory_state->data[free_slot_location-index] = memory_id;
			}
		}
	}
}
