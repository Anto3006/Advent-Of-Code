#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

typedef struct {
	size_t x;
	size_t y;
} position_t;

typedef struct {
	int8_t dx;
	int8_t dy;
} direction_t;

const char* XMAS = "XMAS";

uint32_t count_xmas_pos(vector_str_t*, position_t);
bool check_xmas_direction(vector_str_t*, position_t, direction_t);
uint32_t count_xmas(vector_str_t*);
bool check_x_mas_pos(vector_str_t*, position_t);
uint32_t count_x_mas(vector_str_t*);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_str_t matrix = split_string(data, "\n");
		uint32_t count = count_xmas(&matrix);
		uint32_t count_part_2 = count_x_mas(&matrix);
		free_vector_str(&matrix);
		free(data);
		printf("Result part 1: %u\n",count);
		printf("Result part 2: %u\n",count_part_2);
	}
}

uint32_t count_xmas(vector_str_t* matrix){
	uint32_t count = 0;
	for(size_t pos_y = 0; pos_y < matrix->length; ++pos_y){
		for(size_t pos_x = 0; matrix->data[pos_y][pos_x] != '\0'; pos_x++){
			position_t position = {
				.x = pos_x,
				.y = pos_y
			};
			count += count_xmas_pos(matrix, position);
		}
	}
	return count;
}

uint32_t count_xmas_pos(vector_str_t* matrix, position_t position){
	uint32_t count = 0;
	for(int8_t direction_x = -1; direction_x <= 1; direction_x++){
		for(int8_t direction_y = -1; direction_y <= 1; direction_y++){
			if(direction_x != 0 || direction_y != 0){
				direction_t direction = {
					.dx = direction_x,
					.dy = direction_y
				};
				if(check_xmas_direction(matrix, position, direction)){
					count++;
				}
			}
		}
	}
	return count;
}

bool check_xmas_direction(vector_str_t* matrix, position_t position, direction_t direction){
	position_t current_position = position;
	for(int step = 0; step < 4; step++){
		bool valid_row = current_position.y < matrix->length;
		bool valid_col = valid_row && matrix->data[current_position.y][current_position.x] != '\0';
		if(valid_row && valid_col){
			if(matrix->data[current_position.y][current_position.x] != XMAS[step]){
				return false;
			}
			if(direction.dx < 0){
				if(current_position.x == 0) return false;
				current_position.x -= -direction.dx;
			} else {
				current_position.x += direction.dx;
			}
			if(direction.dy < 0){
				if(current_position.y == 0) return false;
				current_position.y -= -direction.dy;
			} else {
				current_position.y += direction.dy;
			}
		} else {
			return false;
		}
	}
	return true;
}

bool check_x_mas_pos(vector_str_t* matrix, position_t position){
	if(position.x == 0 || position.y == 0){
		return false;
	} else if(position.y >= matrix->length-1 || matrix->data[position.y][position.x+1] == '\0'){
		return false;
	} else if(matrix->data[position.y][position.x] == 'A'){
		position_t upper_left_corner = {.x = position.x - 1, .y = position.y - 1};
		position_t upper_right_corner = {.x = position.x + 1, .y = position.y - 1};
		position_t lower_left_corner = {.x = position.x - 1, .y = position.y + 1};
		position_t lower_right_corner = {.x = position.x + 1, .y = position.y + 1};
		
		//First diagonal
		bool found_m_first_diag = false;
		bool found_s_first_diag = false;
		if(matrix->data[upper_left_corner.y][upper_left_corner.x] == 'M' || matrix->data[lower_right_corner.y][lower_right_corner.x] == 'M'){
			found_m_first_diag = true;
		}
		if(matrix->data[upper_left_corner.y][upper_left_corner.x] == 'S' || matrix->data[lower_right_corner.y][lower_right_corner.x] == 'S'){
			found_s_first_diag = true;
		}

		//Second diagonal
		bool found_m_second_diag = false;
		bool found_s_second_diag = false;
		if(matrix->data[upper_right_corner.y][upper_right_corner.x] == 'M' || matrix->data[lower_left_corner.y][lower_left_corner.x] == 'M'){
			found_m_second_diag = true;
		}
		if(matrix->data[upper_right_corner.y][upper_right_corner.x] == 'S' || matrix->data[lower_left_corner.y][lower_left_corner.x] == 'S'){
			found_s_second_diag = true;
		}

		if(found_m_first_diag && found_s_first_diag && found_s_second_diag && found_m_second_diag){
			return true;
		} else {
			return false;
		}

	} else{
		return false;
	}
}
uint32_t count_x_mas(vector_str_t* matrix){
	uint32_t count = 0;
	for(size_t pos_y = 0; pos_y < matrix->length; ++pos_y){
		for(size_t pos_x = 0; matrix->data[pos_y][pos_x] != '\0'; pos_x++){
			position_t position = {
				.x = pos_x,
				.y = pos_y
			};
			if(check_x_mas_pos(matrix, position)){
				count++;
			}
		}
	}
	return count;
}
