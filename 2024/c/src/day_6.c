
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

typedef struct {
	size_t row;
	size_t col;
	bool is_position_valid;
} position_t;

typedef struct {
	int8_t drow;
	int8_t dcol;
} direction_t;

position_t find_initial_position(vector_str_t*);
direction_t get_direction(vector_str_t*, position_t);
void execute_path_and_mark_tiles(vector_str_t*, position_t, direction_t);
uint32_t count_marked_tiles(vector_str_t*);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_str_t matrix = split_string(data,"\n");
		position_t initial_position = find_initial_position(&matrix);
		if(initial_position.is_position_valid){
			direction_t initial_direction = get_direction(&matrix,initial_position);
			execute_path_and_mark_tiles(&matrix, initial_position, initial_direction);
			uint32_t count = count_marked_tiles(&matrix);
			printf("Result part 1: %u\n",count);
		}
		free_vector_str(&matrix);
		free(data);
	}
	return 0;
}

position_t find_initial_position(vector_str_t* matrix){
	for(size_t row = 0; row < matrix->length; ++row){
		for(size_t col = 0; matrix->data[row][col] != '\0'; ++col){
			char element = matrix->data[row][col];
			if(element == '>' || element == '<' || element == '^' || element == 'v'){
				position_t initial_position = {
					.row = row,
					.col = col,
					.is_position_valid = true
				};
				return initial_position;
			}
		}
	}
	position_t not_found_position = {
		.is_position_valid = false
	};
	return not_found_position;
}

direction_t get_direction(vector_str_t* matrix, position_t position){
	if(position.is_position_valid){
		char element = matrix->data[position.row][position.col];
		direction_t direction;
		switch (element) {
			case '>':
				direction.dcol = 1;
				direction.drow = 0;
				break;
			case '<':
				direction.dcol = -1;
				direction.drow = 0;
				break;
			case '^':
				direction.dcol = 0;
				direction.drow = -1;
				break;
			case 'v':
				direction.dcol = 0;
				direction.drow = 1;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
		return direction;
	} else {
		exit(EXIT_FAILURE);
	}
}

void execute_path_and_mark_tiles(vector_str_t* matrix, position_t initial_position, direction_t initial_direction){
	bool found_end = false;
	position_t current_position = initial_position;
	direction_t current_direction = initial_direction;
	while(!found_end){
		matrix->data[current_position.row][current_position.col] = 'X';
		if(current_position.row == 0 && current_direction.drow == -1){
			found_end = true;
		} else if(current_position.row == matrix->length-1 && current_direction.drow == 1){
			found_end = true;
		} else if(current_position.col == 0 && current_direction.dcol == -1){
			found_end = true;
		} else if(matrix->data[current_position.row][current_position.col+1] == '\0' && current_direction.dcol == 1){
			found_end = true;
		} else {
			position_t next_position;
			if(current_direction.dcol < 0){
				next_position.col = current_position.col - (-current_direction.dcol);
			} else{
				next_position.col = current_position.col + current_direction.dcol;
			}
			if(current_direction.drow < 0){
				next_position.row = current_position.row - (-current_direction.drow);
			} else{
				next_position.row = current_position.row + current_direction.drow;
			}
			char next_tile = matrix->data[next_position.row][next_position.col];
			if(next_tile == '#'){
				if(current_direction.drow != 0){
					current_direction.dcol = current_direction.drow * (-1);
					current_direction.drow = 0;
				} else{
					current_direction.drow = current_direction.dcol;
					current_direction.dcol = 0;
				}
			} else {
				current_position = next_position;
			}
		}
	}
}

uint32_t count_marked_tiles(vector_str_t* matrix){
	uint32_t count = 0;
	for(size_t row = 0; row < matrix->length; ++row){
		for(size_t col = 0; matrix->data[row][col] != '\0'; ++col){
			char element = matrix->data[row][col];
			if(element == 'X'){
				count++;
			}
		}
	}
	return count;
}
