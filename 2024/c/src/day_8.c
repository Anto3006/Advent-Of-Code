#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

typedef struct {
	size_t row;
	size_t col;
	bool is_valid;
} position_t;

typedef struct {
	char signal;
	position_t* antenna_position;
	size_t antenna_count;
	size_t antenna_capacity;
} antenna_pos_t;

typedef struct {
	int8_t drow;
	int8_t dcol;
} direction_vector_t;

antenna_pos_t* parse_antenna_info(vector_str_t*, size_t*);
void mark_signal_antinodes(vector_str_t*, antenna_pos_t*, size_t, size_t);
void mark_total_antinodes(vector_str_t*);
uint32_t calculate_total_antinode_count(vector_str_t*);
position_t calculate_next_position(position_t position, direction_vector_t direction, size_t max_row, size_t max_col);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_str_t lines = split_string(data,"\n");
		uint32_t total_antinode_count = calculate_total_antinode_count(&lines);
		printf("Result part 2: %u\n", total_antinode_count);
		free_vector_str(&lines);
		free(data);
	}
	return 0;
}

antenna_pos_t* parse_antenna_info(vector_str_t* lines, size_t* antenna_count){
	size_t capacity = 100;
	*antenna_count = 0;
	antenna_pos_t* antenna_infos = (antenna_pos_t*) calloc(capacity, sizeof(antenna_pos_t));
	for(size_t line_index = 0; line_index < lines->length; ++line_index){
		char* line = lines->data[line_index];
		size_t char_index = 0;
		while(line[char_index] != '\0'){
			char signal = line[char_index];
			if(signal != '.'){
				position_t position = {
					.row = line_index,
					.col = char_index,
					.is_valid = true
				};
				bool found_signal = false;
				size_t signal_index;
				for(size_t antenna_index = 0; antenna_index < *antenna_count; ++antenna_index){
					if(antenna_infos[antenna_index].signal == signal){
						found_signal = true;
						signal_index = antenna_index;
						break;
					}
				}
				if(found_signal){
					antenna_pos_t* antenna = &antenna_infos[signal_index];
					if(antenna->antenna_capacity == *antenna_count){
						antenna->antenna_capacity *= 2;
						antenna->antenna_position =realloc(antenna->antenna_position, antenna->antenna_capacity * sizeof(position_t));
					}
					antenna->antenna_position[antenna->antenna_count] = position;
					antenna->antenna_count++;
				} else {
					antenna_pos_t new_antenna;
					new_antenna.signal = signal;
					new_antenna.antenna_capacity = 20;
					new_antenna.antenna_count = 1;
					new_antenna.antenna_position = calloc(new_antenna.antenna_capacity, sizeof(position_t));
					new_antenna.antenna_position[0] = position;
					if(capacity == *antenna_count){
						capacity *= 2;
						antenna_infos =realloc(antenna_infos, capacity * sizeof(position_t));
					}
					antenna_infos[*antenna_count] = new_antenna;
					(*antenna_count)++;
				}
			}
			char_index++;
		}
	}
	return antenna_infos;
}

void mark_signal_antinodes(vector_str_t* lines, antenna_pos_t* antenna_pos, size_t max_row, size_t max_col){
	for(size_t tower_index = 0; tower_index < antenna_pos->antenna_count-1; ++tower_index){
		position_t first_position = antenna_pos->antenna_position[tower_index];
		for(size_t second_tower_index = tower_index+1; second_tower_index < antenna_pos->antenna_count; ++second_tower_index){
			direction_vector_t direction;
			position_t second_position = antenna_pos->antenna_position[second_tower_index];
			if(first_position.row >= second_position.row){
				direction.drow = first_position.row - second_position.row;
			} else {
				direction.drow = -(second_position.row - first_position.row);
			}
			if(first_position.col >= second_position.col){
				direction.dcol = first_position.col - second_position.col;
			} else {
				direction.dcol = -(second_position.col - first_position.col);
			}
			position_t next_position = second_position;
			while(next_position.is_valid){
				next_position = calculate_next_position(next_position, direction, max_row, max_col);
				if(next_position.is_valid){
					lines->data[next_position.row][next_position.col] = '#';
				}
			}
			direction.drow *= -1;
			direction.dcol *= -1;
			next_position = first_position;
			while(next_position.is_valid){
				next_position = calculate_next_position(next_position, direction, max_row, max_col);
				if(next_position.is_valid){
					lines->data[next_position.row][next_position.col] = '#';
				}
			}
		}
	}
}

uint32_t calculate_total_antinode_count(vector_str_t* lines){
	uint32_t total_count = 0;
	mark_total_antinodes(lines);
	for(size_t line_index = 0; line_index < lines->length; ++line_index){
		for(size_t line_pos = 0; lines->data[line_index][line_pos] != '\0'; ++line_pos){
			if(lines->data[line_index][line_pos] == '#'){
				total_count++;
			}
		}
	}
	return total_count;
}

void mark_total_antinodes(vector_str_t* lines){
	size_t antenna_count = 0;
	size_t max_row = lines->length-1;
	size_t max_col = 0;
	while(lines->data[0][max_col] != '\0'){
		++max_col;
	}
	max_col--;
	antenna_pos_t* antenna_infos = parse_antenna_info(lines, &antenna_count);
	for(size_t antenna_index = 0; antenna_index < antenna_count; ++antenna_index){
		mark_signal_antinodes(lines, &antenna_infos[antenna_index], max_row, max_col);
		free(antenna_infos[antenna_index].antenna_position);
	}
	free(antenna_infos);
}

position_t calculate_next_position(position_t position, direction_vector_t direction, size_t max_row, size_t max_col){
	position_t next_position;
	next_position.is_valid = true;
	if(direction.drow >= 0 && position.row + direction.drow <= max_row){
		next_position.row = position.row + direction.drow;
	} else if(direction.drow < 0 && position.row >= (uint8_t)-direction.drow){
		next_position.row = position.row - (-direction.drow);
	} else{
		next_position.is_valid = false;
	}
	if(direction.dcol >= 0 && position.col + direction.dcol <= max_col){
		next_position.col = position.col + direction.dcol;
	} else if(direction.dcol < 0 && position.col >=(uint8_t) -direction.dcol){
		next_position.col = position.col - (-direction.dcol);
	} else{
		next_position.is_valid = false;
	}
	return next_position;
}
