#include "solution.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define INIT_CAPACITY 100

static const char* test_input = 
	".......S.......\n"
	"...............\n"
	".......^.......\n"
	"...............\n"
	"......^.^......\n"
	"...............\n"
	".....^.^.^.....\n"
	"...............\n"
	"....^.^...^....\n"
	"...............\n"
	"...^.^...^.^...\n"
	"...............\n"
	"..^...^.....^..\n"
	"...............\n"
	".^.^.^.^.^...^.\n"
	"...............";

typedef struct {
	char* matrix;
	size_t n_rows;
	size_t n_cols;
	size_t capacity;
} CharMatrix;

static int read_char_matrix(CharMatrix* matrix, const char* input){
	if(matrix->matrix != 0){
		free(matrix->matrix);
	} else {
		matrix->matrix = calloc(INIT_CAPACITY, sizeof(char));
		matrix->capacity = INIT_CAPACITY;
		if(matrix->matrix == 0){
			printf("Error in allocation\n");
			return 1;
		}
	}
	matrix->n_cols = 0;
	matrix->n_rows = 0;
	size_t pos = 0;
	size_t m_pos = 0;
	size_t col_count = 0;
	while(input[pos] != 0){
		char c = input[pos];
		if(c == '\n'){
			matrix->n_rows += 1;
			if(matrix->n_cols == 0){
				matrix->n_cols = col_count;
			} else {
				if (matrix->n_cols != col_count){
					printf("Error: row with differing amount of columns found\n");
					return 1;
				}
			}
			col_count = 0;
		} else {
			if (m_pos >= matrix->capacity){
				matrix->capacity = 2*matrix->capacity;
				matrix->matrix = realloc(matrix->matrix, matrix->capacity * sizeof(char));
				if(matrix->matrix == 0){
					printf("Error in allocation\n");
					return 1;
				}
			}
			col_count += 1;
			matrix->matrix[m_pos] = c;
			m_pos += 1;
		}
		pos += 1;
	}
	return 0;
}

static char get_matrix_value(CharMatrix* matrix, size_t row, size_t col){
	if(row < matrix->n_rows && col < matrix->n_cols){
		size_t pos = row * matrix->n_cols + col;
		return matrix->matrix[pos];
	} else {
		return 0;
	}
}

static void set_matrix_value(CharMatrix* matrix, size_t row, size_t col, char new_value){
	if(row < matrix->n_rows && col < matrix->n_cols){
		size_t pos = row * matrix->n_cols + col;
		matrix->matrix[pos] = new_value;
	}
}

static int calculate_next_row(CharMatrix* matrix, size_t row){
	int split_count = 0;
	for(size_t col = 0; col < matrix->n_cols; ++col){
		char current = get_matrix_value(matrix, row, col);
		char next = get_matrix_value(matrix, row+1, col);
		if (current == 'S' || current == '|'){
			if(next == '.'){
				set_matrix_value(matrix, row+1, col, '|');
			} else if(next == '^'){
				set_matrix_value(matrix, row+1, col-1, '|');
				set_matrix_value(matrix, row+1, col+1, '|');
				split_count += 1;
			}
		}
	}
	return split_count;
}

static uint64_t* create_world_count_matrix(CharMatrix* matrix){
	uint64_t * world_count = calloc(matrix->n_cols*matrix->n_rows, sizeof(uint64_t));
	for(size_t pos = 0; pos < matrix->n_cols * matrix->n_rows; ++pos){
		if(matrix->matrix[pos] == 'S'){
			world_count[pos] = 1;
		} else {
			world_count[pos] = 0;
		}
	}
	return world_count;
}

static void calculate_next_row_many_worlds(CharMatrix* matrix, uint64_t* many_world_matrix, size_t row){
	for(size_t col = 0; col < matrix->n_cols; ++col){
		char current = get_matrix_value(matrix, row, col);
		char next = get_matrix_value(matrix, row+1, col);
		uint64_t current_worlds = many_world_matrix[row*matrix->n_cols + col];
		if (current == 'S' || current == '|'){
			if(next != '^'){
				set_matrix_value(matrix, row+1, col, '|');
				many_world_matrix[(row+1)*matrix->n_cols + col] += current_worlds;
			} else {
				set_matrix_value(matrix, row+1, col-1, '|');
				set_matrix_value(matrix, row+1, col+1, '|');
				many_world_matrix[(row+1)*matrix->n_cols + col+1] += current_worlds;
				many_world_matrix[(row+1)*matrix->n_cols + col-1] += current_worlds;
			}
		}
	}
}

static void part_1(const char *input){
	CharMatrix matrix;
	matrix.matrix = 0;
	read_char_matrix(&matrix, input);
	int total_splits = 0;
	for(size_t row = 0; row < matrix.n_rows-1; ++row){
		total_splits += calculate_next_row(&matrix,row);
	}
	printf("Result: %d\n",total_splits);
	free(matrix.matrix);
}

static void part_2(const char *input){
	CharMatrix matrix;
	matrix.matrix = 0;
	read_char_matrix(&matrix, input);
	uint64_t* world_count = create_world_count_matrix(&matrix);
	for(size_t row = 0; row < matrix.n_rows-1; ++row){
		calculate_next_row_many_worlds(&matrix,world_count,row);
	}
	size_t total_worlds = 0;
	for(size_t col = 0; col < matrix.n_cols; ++col){
		total_worlds += world_count[(matrix.n_rows-1)*matrix.n_cols + col];
	}
	printf("Result: %lu\n",total_worlds);
	free(matrix.matrix);
	free(world_count);
}

Solution create_day_07_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

