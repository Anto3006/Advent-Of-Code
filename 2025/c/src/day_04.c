#include "solution.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t n_row;
	size_t n_col;
	size_t capacity;
	char *array;
} RollMatrix;

static const char* test_input = 
	"..@@.@@@@.\n"
	"@@@.@.@.@@\n"
	"@@@@@.@.@@\n"
	"@.@@@@..@.\n"
	"@@.@@@@.@@\n"
	".@@@@@@@.@\n"
	".@.@.@.@@@\n"
	"@.@@@.@@@@\n"
	".@@@@@@@@.\n"
	"@.@.@@@.@.\n";

static RollMatrix parse_roll_matrix(const char *input){
	size_t n_col = 0;
	size_t n_row = 0;
	size_t capacity = 1024;
	size_t pos = 0;
	char *array = calloc(capacity,sizeof(char));
	size_t str_pos = 0;
	size_t row_length = 0;
	bool is_first_row_read = false;
	while(input[str_pos] != '\0'){
		if(input[str_pos] == '\n'){
			if(is_first_row_read){
				if(row_length != n_col){
					printf("Error: found row with different length %ld\n",n_row);
					exit(EXIT_FAILURE);
				}
			} else {
				n_col = row_length;
				is_first_row_read = true;
			}
			n_row++;
			row_length = 0;
		} else{
			if(pos >= capacity){
				capacity = 2 * capacity;
				array = realloc(array, capacity);
			}
			array[pos] = input[str_pos];
			row_length++;
			pos++;
		}
		str_pos++;
	}
	RollMatrix roll_matrix;
	roll_matrix.n_row = n_row;
	roll_matrix.n_col = n_col;
	roll_matrix.capacity = capacity;
	roll_matrix.array = array;
	return roll_matrix;
}

static uint32_t count_neighbors(RollMatrix *roll_matrix, size_t row, size_t col){
	uint32_t initial_row = row;
	uint32_t initial_col = col;
	if(row > 0){
		initial_row = initial_row - 1;
	}
	if(col > 0){
		initial_col = initial_col - 1;
	}

	uint32_t final_row = row;
	uint32_t final_col = col;
	if(row < roll_matrix->n_row-1){
		final_row = final_row + 1;
	}
	if(col < roll_matrix->n_col-1){
		final_col = final_col + 1;
	}

	uint32_t count = 0;

	for(size_t neighbor_row = initial_row; neighbor_row <= final_row; ++neighbor_row){
		for(size_t neighbor_col = initial_col; neighbor_col <= final_col; ++neighbor_col){
			if(neighbor_row != row || neighbor_col != col){
				if(roll_matrix->array[neighbor_col + neighbor_row * roll_matrix->n_col] == '@'){
					count++;
				}
			}
		}
	}
	return count;
}

static void part_1(const char *input){
	RollMatrix roll_matrix = parse_roll_matrix(input);
	uint32_t accessible_count = 0;
	for(size_t row = 0; row < roll_matrix.n_row; ++row){
		for(size_t col = 0; col < roll_matrix.n_col; ++col){
			if(roll_matrix.array[col + row * roll_matrix.n_col] == '@'){
				uint32_t neighbor_count = count_neighbors(&roll_matrix, row, col);
				if(neighbor_count < 4){
					accessible_count++;
				}
			}
		}
	}
	free(roll_matrix.array);
	printf("Result: %d\n",accessible_count);
}

static void part_2(const char *input){
	RollMatrix roll_matrix = parse_roll_matrix(input);
	uint32_t accessible_count = 1;
	uint32_t sum = 0;
	while (accessible_count > 0){
		char *next_gen = calloc(roll_matrix.capacity, sizeof(char));
		accessible_count = 0;
		for(size_t row = 0; row < roll_matrix.n_row; ++row){
			for(size_t col = 0; col < roll_matrix.n_col; ++col){
				size_t pos = col + row * roll_matrix.n_col;
				if(roll_matrix.array[pos] == '@'){
					uint32_t neighbor_count = count_neighbors(&roll_matrix, row, col);
					if(neighbor_count < 4){
						accessible_count++;
						next_gen[pos] = '.';
					} else {
						next_gen[pos] = '@';
					}
				}else{
					next_gen[pos] = '.';
				}
			}
		}
		sum += accessible_count;
		free(roll_matrix.array);
		roll_matrix.array = next_gen;
	}
	free(roll_matrix.array);
	printf("Result: %d\n",sum);
}

Solution create_day_04_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

