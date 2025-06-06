#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"
#include <math.h>
#include <strings.h>

typedef struct node_t node_t;

struct node_t{
	uint64_t value;
	size_t neighbor_count;
	uint8_t max_remaining_blinks;
	node_t* neighbors[2];
};

typedef struct{
	node_t*** nodes_hash_map;
	size_t* hash_list_lengths;
	size_t* hash_list_capacities;
	size_t hash_map_length;
} state_transitions_t;

uint32_t calculate_number_stones_after_blink(uint64_t starting_stone_number, uint8_t number_blinks);
uint64_t pow_int(uint64_t base, uint64_t exponent);
node_t* add_node(state_transitions_t* state_transitions, uint64_t value, uint8_t number_blinks);
bool is_value_in_state(state_transitions_t* state_transitions, uint64_t value);
uint64_t count_stones(state_transitions_t* state_transitions, uint64_t starting_value, uint8_t number_blinks);
void initialize_state_transitions(state_transitions_t* state_transitions);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_str_t numbers_str = split_string(data," ");
		vector_uint_t numbers = parse_vector_uint(&numbers_str);
		uint64_t total_stones = 0;
		uint8_t number_blinks = 25;
		for(size_t number_index = 0; number_index < numbers.length; ++number_index){
			total_stones += calculate_number_stones_after_blink(numbers.data[number_index], number_blinks);
		}
		printf("Result part 1: %lu\n", total_stones);
		free_vector_uint(&numbers);
		free_vector_str(&numbers_str);
		free(data);
	}
	return 0;
}

uint32_t calculate_number_stones_after_blink(uint64_t starting_stone_number, uint8_t number_blinks){
	if(number_blinks == 0){
		return 1;
	} else {
		if(starting_stone_number == 0){
			return calculate_number_stones_after_blink(1, number_blinks - 1);
		} else {
			uint8_t number_digits= log10(starting_stone_number)+1;
			if(number_digits % 2 == 0){
				uint64_t power_ten = pow_int(10, number_digits / 2);
				uint64_t right_half = starting_stone_number % power_ten;
				uint64_t left_half = starting_stone_number / power_ten;
				return calculate_number_stones_after_blink(left_half, number_blinks - 1) + calculate_number_stones_after_blink(right_half, number_blinks - 1);
			} else {
				return calculate_number_stones_after_blink(starting_stone_number * 2024, number_blinks-1);
			}
		}
	}
}

uint64_t pow_int(uint64_t base, uint64_t exponent){
	if(exponent == 0){
		return 1;
	} else if(exponent == 1){
		return base;
	} else if((exponent & 1) == 0){
		return pow_int(base*base,exponent >> 1);
	} else {
		return pow_int(base*base,(exponent-1) >> 1) * base;
	}
}

node_t* add_node(state_transitions_t* state_transitions, uint64_t value, uint8_t number_blinks){
	size_t hash_value = value % state_transitions->hash_map_length;
	size_t pos;
	node_t* node = 0;
	if(!is_value_in_state(state_transitions, value)){
		size_t length = state_transitions->hash_list_lengths[hash_value];
		size_t capacity = state_transitions->hash_list_capacities[hash_value];
		pos = length;
		if(length == capacity){
			capacity *= 2;
			state_transitions->hash_list_capacities[hash_value] = capacity;
			state_transitions->nodes_hash_map[hash_value] = realloc(state_transitions->nodes_hash_map[hash_value],capacity*sizeof(node_t*));
		}
		state_transitions->nodes_hash_map[hash_value][pos] = malloc(sizeof(node_t));
		state_transitions->nodes_hash_map[hash_value][pos]->value = value;
		state_transitions->nodes_hash_map[hash_value][pos]->neighbor_count = 0;
		state_transitions->nodes_hash_map[hash_value][pos]->max_remaining_blinks = 0;
		state_transitions->hash_list_lengths[hash_value]++;
		node = state_transitions->nodes_hash_map[hash_value][pos];
	} else {
		for(size_t index = 0; index < state_transitions->hash_list_lengths[hash_value]; ++index){
			if(state_transitions->nodes_hash_map[hash_value][index]->value == value){
				node = state_transitions->nodes_hash_map[hash_value][index];
				pos = index;
				break;
			}
		}
	}
	if(node != 0 && number_blinks > node->max_remaining_blinks){
		node->max_remaining_blinks = number_blinks;
		if(value == 0){
			node_t* new_node = add_node(state_transitions, 1, number_blinks-1);
			if(new_node != 0){
				node->neighbor_count = 1;
				node->neighbors[0] = new_node;
			}
		} else {
			uint8_t number_digits= log10(value)+1;
			if(number_digits % 2 == 0){
				uint64_t power_ten = pow_int(10, number_digits / 2);
				uint64_t right_half = value % power_ten;
				uint64_t left_half = value / power_ten;
				node_t* new_left_node = add_node(state_transitions, left_half, number_blinks-1);
				node_t* new_right_node = add_node(state_transitions, right_half, number_blinks-1);
				if(new_left_node != 0 && new_right_node != 0){ 
					node->neighbor_count = 2;
					node->neighbors[0] = new_left_node;
					node->neighbors[1] = new_right_node;
				}
			} else {
				node_t* new_node = add_node(state_transitions, value * 2024, number_blinks-1);
				if(new_node != 0){
					node->neighbor_count = 1;
					node->neighbors[0] = new_node;
				}
			}
		}
	}
	return node;
	
}

bool is_value_in_state(state_transitions_t* state_transitions, uint64_t value){
	size_t hash_value = value % state_transitions->hash_map_length;
	bool found_value = false;
	for(size_t index = 0; index < state_transitions->hash_list_lengths[hash_value]; ++index){
		if(state_transitions->nodes_hash_map[hash_value][index]->value == value){
			found_value = true;
			break;
		}
	}
	return found_value;
}


uint64_t count_stones(state_transitions_t* state_transitions, uint64_t starting_value, uint8_t number_blinks){
	if(number_blinks == 0){
		return 1;
	} else {
		uint64_t stones = 0;
		size_t hash_value = starting_value % state_transitions->hash_map_length;
		node_t* node = 0;
		for(size_t index = 0; index < state_transitions->hash_list_lengths[hash_value]; ++index){
			if(state_transitions->nodes_hash_map[hash_value][index]->value == starting_value){
				node = state_transitions->nodes_hash_map[hash_value][index];
			}
		}
		if(node != 0){
			if(node->neighbor_count == 0){
				printf("Error in node: %lu with remaining blinks: %u\n", starting_value, number_blinks);
			}
			if(node->neighbor_count > 0){
				stones += count_stones(state_transitions, node->neighbors[0]->value, number_blinks-1);
			}
			if(node->neighbor_count > 1){
				stones += count_stones(state_transitions, node->neighbors[1]->value, number_blinks-1);
			}
			return stones;
		} else {
			printf("Error\n");
			return 1;
		}
	}
}

void initialize_state_transitions(state_transitions_t* state_transitions){
	size_t HASH_MAP_SIZE = 100000;
	size_t INITIAL_HASH_LIST_CAPACITY = 20;
	state_transitions->hash_map_length = HASH_MAP_SIZE;
	state_transitions->hash_list_lengths = calloc(HASH_MAP_SIZE, sizeof(size_t));
	state_transitions->hash_list_capacities = calloc(HASH_MAP_SIZE, sizeof(size_t));
	state_transitions->nodes_hash_map = calloc(HASH_MAP_SIZE, sizeof(node_t**));
	for(size_t index = 0; index < HASH_MAP_SIZE; ++index){
		state_transitions->hash_list_lengths[index] = 0;
		state_transitions->hash_list_capacities[index] = INITIAL_HASH_LIST_CAPACITY;
		state_transitions->nodes_hash_map[index] = calloc(INITIAL_HASH_LIST_CAPACITY, sizeof(node_t*));
	}
}
