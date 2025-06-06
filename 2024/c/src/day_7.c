#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

typedef struct {
	uint64_t target;
	vector_uint_t operands;
} equation_t;

typedef enum{
	PLUS = 0,
	MULT = 1,
	CONCAT = 2
} operator_t;

typedef struct {
	size_t operator_count;
	size_t operator_capacity;
	operator_t* operator_seq;
} operators_seq_t;

uint64_t calculate_sum_valid_targets(vector_str_t*, bool);
equation_t parse_equation(char*);
bool is_equation_valid(equation_t*, bool);
void free_equation(equation_t*);
uint64_t pow_int(uint64_t base, uint64_t exponent);
uint64_t apply_operators(equation_t* equation, operators_seq_t* operator_sequence);
void initialize_operator_sequence(operators_seq_t* operator_sequence, size_t operator_count);
void free_operator_sequence(operators_seq_t* operator_sequence);
void go_next_sequence(operators_seq_t* operator_sequence, bool use_concat);
void reset_operator_sequence(operators_seq_t* operator_sequence, size_t operator_count);


int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_str_t lines = split_string(data,"\n");
		uint64_t sum_no_concat = calculate_sum_valid_targets(&lines, false);
		uint64_t sum_concat = calculate_sum_valid_targets(&lines, true);
		printf("Result part 1: %lu\n", sum_no_concat);
		printf("Result part 2: %lu\n", sum_concat);
		free_vector_str(&lines);
		free(data);
	}
	return 0;
}

uint64_t calculate_sum_valid_targets(vector_str_t* lines, bool use_concat){
	uint64_t sum = 0;
	for(size_t index = 0; index < lines->length; ++index){
		char* line = lines->data[index];
		equation_t equation = parse_equation(line);
		if(is_equation_valid(&equation, use_concat)){
			sum += equation.target;
		}
		free_equation(&equation);
	}
	return sum;
}
equation_t parse_equation(char* equation_line){
	vector_str_t equation_parts = split_string(equation_line,": ");
	char* stopstring;
	uint64_t target = strtoul(equation_parts.data[0], &stopstring, 10);
	vector_str_t operands_str = split_string(equation_parts.data[1]," ");
	vector_uint_t operands = parse_vector_uint(&operands_str);
	equation_t equation;
	free_vector_str(&operands_str);
	free_vector_str(&equation_parts);
	equation.target = target;
	equation.operands = operands;
	return equation;
}
bool is_equation_valid(equation_t* equation, bool use_concat){
	size_t operator_count = equation->operands.length-1;
	uint64_t operator_codification_limit = pow_int(3,operator_count);
	operators_seq_t operators;
	initialize_operator_sequence(&operators, operator_count);
	for(uint64_t operator_codification = 0; operator_codification < operator_codification_limit; ++operator_codification){
		if(apply_operators(equation, &operators) == equation->target){
			return true;
		}
		go_next_sequence(&operators, use_concat);
	}
	free_operator_sequence(&operators);
	return false;
}
void free_equation(equation_t* equation){
	free_vector_uint(&equation->operands);
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

uint64_t apply_operators(equation_t* equation, operators_seq_t* operator_sequence){
	uint64_t result = equation->operands.data[0];
	uint32_t operand;
	for(size_t operator_index = 0; operator_index < operator_sequence->operator_count; ++operator_index){
		operator_t operator = operator_sequence->operator_seq[operator_index];
		switch (operator) {
			case PLUS:
				result += equation->operands.data[operator_index+1];
				break;
			case MULT:
				result *= equation->operands.data[operator_index+1];
				break;
			case CONCAT:
				operand = equation->operands.data[operator_index+1];
				while(operand > 0){
					result *= 10;
					operand = operand / 10;
				}
				result += equation->operands.data[operator_index+1];
				break;
			default:
				printf("Error\n");
				break;
		};
	}
	return result;
}

void initialize_operator_sequence(operators_seq_t* operator_sequence, size_t operator_count){
	operator_sequence->operator_count = operator_count;
	operator_sequence->operator_capacity = operator_count;
	operator_sequence->operator_seq = calloc(operator_count, sizeof(operator_t));
}

void free_operator_sequence(operators_seq_t* operator_sequence){
	operator_sequence->operator_count = 0;
	operator_sequence->operator_capacity = 0;
	free(operator_sequence->operator_seq);
}

void go_next_sequence(operators_seq_t* operator_sequence, bool use_concat){
	bool has_remainder = false;
	size_t index = 1;
	if(use_concat && operator_sequence->operator_seq[0] == CONCAT){
		has_remainder = true;
		operator_sequence->operator_seq[0] = PLUS;
	} else if(!use_concat && operator_sequence->operator_seq[0] == MULT){
		has_remainder = true;
		operator_sequence->operator_seq[0] = PLUS;
	} else {
		operator_sequence->operator_seq[0] += 1;
	}
	while(has_remainder && index < operator_sequence->operator_count){
		if(use_concat && operator_sequence->operator_seq[index] == CONCAT){
			has_remainder = true;
			operator_sequence->operator_seq[index] = PLUS;
		} else if(!use_concat && operator_sequence->operator_seq[index] == MULT){
			operator_sequence->operator_seq[index] = PLUS;
			has_remainder = true;
		} else {
			operator_sequence->operator_seq[index] += 1;
			has_remainder = false;
		}
		index++;
	}
}

void reset_operator_sequence(operators_seq_t* operator_sequence, size_t operator_count){
	if(operator_count > operator_sequence->operator_capacity){
		operator_sequence->operator_seq = realloc(operator_sequence->operator_seq,operator_count*sizeof(operator_t));
		operator_sequence->operator_capacity = operator_count;
	}
	operator_sequence->operator_count = operator_count;
	for(size_t op_index = 0; op_index < operator_count; ++op_index){
		operator_sequence->operator_seq[op_index] = PLUS;
	}
}
