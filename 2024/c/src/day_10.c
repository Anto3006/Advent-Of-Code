#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"


typedef struct{
	size_t row;
	size_t col;
} position_t;

typedef struct {
	position_t* positions;
	size_t capacity;
	size_t element_count;
} set_of_positions_t;

typedef struct {
	size_t row_count;
	size_t col_count;
	size_t element_count;
	uint8_t* altitude_map;
	set_of_positions_t* reachable_end_pos;
} topographic_map_t;

void initialize_topographic_map(char* data,topographic_map_t* map);
void initialize_set_of_positions(set_of_positions_t* set);
void find_path_to_end(topographic_map_t* map, size_t row, size_t col);
void add_to_set(set_of_positions_t* set, position_t position);
void copy_set(set_of_positions_t* source, set_of_positions_t* target);
void apply_union_set(set_of_positions_t* source, set_of_positions_t* target);
void find_all_paths(topographic_map_t* map);
void free_topographic_map(topographic_map_t* map);
uint32_t calculate_paths(topographic_map_t* map);
uint32_t* calculate_number_different_paths(topographic_map_t* map);
uint32_t add_number_of_different_paths(topographic_map_t* map, uint32_t* number_of_paths);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		topographic_map_t map;
		initialize_topographic_map(data, &map);
		find_all_paths(&map);
		uint32_t path_count = calculate_paths(&map);
		uint32_t* number_paths = calculate_number_different_paths(&map);
		uint32_t total_paths =  add_number_of_different_paths(&map, number_paths);
		printf("Result part 1: %u\n", path_count);
		printf("Result part 2: %u\n", total_paths);
		free(number_paths);
		free_topographic_map(&map);
		free(data);
	}
	return 0;
}

void initialize_topographic_map(char* data, topographic_map_t* map){
	map->row_count = 0;
	map->col_count = 0;
	map->element_count = 0;
	size_t data_index = 0;
	size_t capacity = 1000;
	size_t index = 0;
	size_t col = 0;
	map->altitude_map = calloc(capacity,sizeof(uint8_t));
	map->reachable_end_pos = calloc(capacity, sizeof(set_of_positions_t));
	while(data[data_index] != '\0'){
		if(data[data_index] == '\n'){
			col = 0;
			if(map->col_count == 0){
				map->col_count = data_index;
			}
			map->row_count++;
		} else {
			if(index == capacity){
				capacity *= 2;
				map->altitude_map = realloc(map->altitude_map, capacity*sizeof(uint8_t));
				map->reachable_end_pos = realloc(map->reachable_end_pos, capacity*sizeof(set_of_positions_t));
			}
			map->altitude_map[index] = data[data_index] - '0'; 
			initialize_set_of_positions(&map->reachable_end_pos[index]);
			if(map->altitude_map[index] == 9){
				position_t position = {
					.row = map->row_count,
					.col = col
				};
				add_to_set(&map->reachable_end_pos[index], position);
			}
			index++;
			map->element_count++;
			col++;
		}
		data_index++;
	}
	printf("Row count: %lu\n", map->row_count);
	printf("Col count: %lu\n", map->col_count);
	printf("Number elements: %lu\n", map->element_count);
}

void find_path_to_end(topographic_map_t* map, size_t row, size_t col){
	uint8_t value = map->altitude_map[row*map->col_count + col];
	if(value != 0){
		map->altitude_map[row*map->col_count + col] = 11;
		if(row > 0 && map->altitude_map[(row - 1)*map->col_count + col] == (value - 1)){
			apply_union_set(&map->reachable_end_pos[row*map->col_count + col], &map->reachable_end_pos[(row-1)*map->col_count + col]);
			find_path_to_end(map, row-1, col);
		} 
		if(row < map->row_count-1 && map->altitude_map[(row + 1)*map->col_count + col] == (value - 1)){
			apply_union_set(&map->reachable_end_pos[row*map->col_count + col], &map->reachable_end_pos[(row + 1)*map->col_count + col]);
			find_path_to_end(map, row + 1, col);
		} 
		if(col > 0 && map->altitude_map[row*map->col_count + col - 1] == (value - 1)){
			apply_union_set(&map->reachable_end_pos[row*map->col_count + col], &map->reachable_end_pos[row*map->col_count + col - 1]);
			find_path_to_end(map, row, col - 1);
		} 
		if(col < map->col_count-1 && map->altitude_map[row*map->col_count + col + 1] == (value - 1)){
			apply_union_set(&map->reachable_end_pos[row*map->col_count + col], &map->reachable_end_pos[row*map->col_count + col + 1]);
			find_path_to_end(map, row, col + 1);
		}
		map->altitude_map[row*map->col_count + col] = value;
	}
}
void initialize_set_of_positions(set_of_positions_t* set){
	set->capacity = 20;
	set->element_count = 0;
	set->positions = calloc(set->capacity,sizeof(position_t));
}

void add_to_set(set_of_positions_t* set, position_t position){
	bool found_element = false;
	for(size_t index = 0; index < set->element_count; ++index){
		if(set->positions[index].row == position.row && set->positions[index].col == position.col){
			found_element = true;
			break;
		}
	}
	if(!found_element){
		if(set->element_count == set->capacity){
			set->capacity *= 2;
			set->positions = realloc(set->positions, set->capacity*sizeof(position_t));
		}
		set->positions[set->element_count] = position;
		set->element_count++;
	}
}

void copy_set(set_of_positions_t* source, set_of_positions_t* target){
	if(target->capacity < source->capacity){
		target->capacity = source->capacity;
		target->positions = realloc(target->positions,target->element_count*sizeof(position_t));
	}
	target->element_count = source->element_count;
	for(size_t index = 0; index < target->element_count; ++index){
		target->positions[index] = source->positions[index];
	}
}

void apply_union_set(set_of_positions_t* source, set_of_positions_t* target){
	for(size_t source_index = 0; source_index < source->element_count; ++source_index){
		add_to_set(target, source->positions[source_index]);
	}
}

void find_all_paths(topographic_map_t* map){
	for(size_t row = 0; row < map->row_count; row++){
		for(size_t col = 0; col < map->col_count; col++){
			if(map->altitude_map[row*map->col_count + col] == 9){
				find_path_to_end(map, row, col);
			}
		}
	}
}

uint32_t calculate_paths(topographic_map_t* map){
	find_all_paths(map);
	uint32_t total_paths = 0;
	for(size_t row = 0; row < map->row_count; row++){
		for(size_t col = 0; col < map->col_count; col++){
			//set_of_positions_t set = map->reachable_end_pos[row*map->col_count + col];
			//for(size_t index = 0; index < set.element_count; ++index){
				//position_t pos = set.positions[index];
				//printf("(%lu, %lu)", pos.row, pos.col);
			//}
			//printf("\n");
			if(map->altitude_map[row*map->col_count + col] == 0){
				total_paths += map->reachable_end_pos[row*map->col_count + col].element_count;
			}
		}
		//printf("-----------------------------------------------------------------------\n");
	}
	return total_paths;
}

void free_topographic_map(topographic_map_t* map){
	free(map->altitude_map);
	for(size_t index = 0; index < map->element_count; index++){
		free(map->reachable_end_pos[index].positions);
	}
	free(map->reachable_end_pos);
}

uint32_t* calculate_number_different_paths(topographic_map_t* map){
	uint32_t* number_of_paths = calloc(map->element_count,sizeof(uint32_t));
	for(size_t index = 0; index < map->element_count; ++index){
		if(map->altitude_map[index] == 9){
			number_of_paths[index] = 1;
		} else {
			number_of_paths[index] = 0;
		}
	}
	for(uint8_t altitude = 9; altitude > 0; altitude--){
		for(size_t row = 0; row < map->row_count; row++){
			for(size_t col = 0; col < map->col_count; col++){
				if(map->altitude_map[row*map->col_count + col] == altitude){
					if(row > 0 && map->altitude_map[(row - 1)*map->col_count + col] == (altitude - 1)){
						number_of_paths[(row - 1)*map->col_count + col] += number_of_paths[row * map->col_count + col];
					} 
					if(row < map->row_count-1 && map->altitude_map[(row + 1)*map->col_count + col] == (altitude - 1)){
						number_of_paths[(row + 1)*map->col_count + col] += number_of_paths[row * map->col_count + col];
					} 
					if(col > 0 && map->altitude_map[row*map->col_count + col - 1] == (altitude - 1)){
						number_of_paths[row*map->col_count + col - 1] += number_of_paths[row * map->col_count + col];
					} 
					if(col < map->col_count-1 && map->altitude_map[row*map->col_count + col + 1] == (altitude - 1)){
						number_of_paths[row*map->col_count + col + 1] += number_of_paths[row * map->col_count + col];
					}
				}
			}
		}
	}
	return number_of_paths;
}

uint32_t add_number_of_different_paths(topographic_map_t* map, uint32_t* number_of_paths){
	uint32_t sum_paths = 0;
	for(size_t index = 0; index < map->element_count; ++index){
		if(map->altitude_map[index] == 0){
			sum_paths += number_of_paths[index];
		}
	}
	return sum_paths;
}
