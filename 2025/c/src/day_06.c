#include "solution.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include <stdint.h>

typedef struct {
	uint64_t numbers[4];
	char op;
} Calculation;

typedef struct {
	char* numbers[4];
	char op;
	size_t width;
} CalculationPart2;

static const char* test_input = 
	"123 328  51 64 \n"
	" 45 64  387 23 \n"
	"  6 98  215 314\n"
	"*   +   *   +  ";

static bool is_column_all_spaces(const char *text, size_t text_length, size_t str_pos, size_t line_length){
	size_t line_pos = str_pos % line_length;
	for(size_t pos = line_pos; pos < text_length; pos += line_length){
		if(text[pos] != ' '){
			return false;
		}
	}
	return true;
}

static void part_1(const char *input){
	printf("Begin day 6 part 1\n");
	size_t str_pos = 0;
	Calculation calculations[1024];
	size_t line = 0;
	size_t calculation_count = 0;
	bool is_number_line = false;
	while(input[str_pos] != '\0'){
		size_t calculation_pos = 0;
		while(input[str_pos] == ' '){
			str_pos++;
		}
		if('0' <= input[str_pos] && input[str_pos] <= '9'){
			is_number_line = true;
		} else {
			is_number_line = false;
		}
		if(is_number_line){
			while(input[str_pos] != '\n' && input[str_pos] != '\0'){
				uint64_t number = 0;
				while('0' <= input[str_pos] && input[str_pos] <= '9'){
					number = number * 10;
					number += (input[str_pos] - '0');
					str_pos++;
				}
				calculations[calculation_pos].numbers[line] = number;
				calculation_pos++;
				while(input[str_pos] == ' '){
					str_pos++;
				}
			}
			if(input[str_pos] != '\0'){
				str_pos++;
			}
		} else {
			while(input[str_pos] != '\n' && input[str_pos] != '\0'){
				while(input[str_pos] == ' '){
					str_pos++;
				}
				if(input[str_pos] != '\0'){
					calculations[calculation_pos].op = input[str_pos];
					str_pos++;
					calculation_pos++;
				}
			}
		}
		line++;
		if(calculation_count == 0){
			calculation_count = calculation_pos;
		} else if(calculation_count != calculation_pos) {
			printf("Different calculation count found\n");
		}
	}
	uint64_t sum = 0;
	for(size_t calculation_pos = 0; calculation_pos < calculation_count; ++calculation_pos){
		Calculation calc = calculations[calculation_pos];
		uint64_t result;
		if(calc.op == '+'){
			result = 0;
			for (size_t n_pos = 0; n_pos < line-1; ++n_pos){
				result += calc.numbers[n_pos];
			}
		} else if(calc.op == '*'){
			result = 1;
			for (size_t n_pos = 0; n_pos < line-1; ++n_pos){
				result *= calc.numbers[n_pos];
			}
		}
		sum += result;
	}
	printf("Result: %ld\n",sum);
}

static void part_2(const char *input){
	printf("Begin day 6 part 2\n");
	size_t str_pos = 0;

	CalculationPart2 calculations[1024];
	for(size_t calculation_pos = 0; calculation_pos < 1024; ++calculation_pos){
		calculations[calculation_pos].width = 0;
	}

	size_t line = 0;
	bool is_number_line = false;
	size_t calculation_count = 0;
	size_t line_length = 0;

	size_t text_length = 0;
	while(input[str_pos] != '\0'){
		while(input[str_pos] != '\n' && input[str_pos] != '\0'){
			str_pos++;
		}
		if(input[str_pos] == '\n'){
			str_pos++;
			if(line_length == 0){
				line_length = str_pos;
			}
		}
	}
	text_length = str_pos;
	

	str_pos = 0;

	while(input[str_pos] != '\0'){
		if(('0' <= input[str_pos] && input[str_pos] <= '9') || input[str_pos] == ' '){
			is_number_line = true;
		} else {
			is_number_line = false;
		}
		if(is_number_line){
			size_t calculation_pos = 0;
			while(input[str_pos] != '\n' && input[str_pos] != '\0'){
				char* number = calloc(10, sizeof(char));
				size_t width = 0;
				size_t number_pos = 0;
				while(input[str_pos] == ' '){
					number[number_pos] = '.';
					number_pos++;
					str_pos++;
					width++;
				}
				while('0' <= input[str_pos] && input[str_pos] <= '9'){
					number[number_pos] = input[str_pos];
					number_pos++;
					str_pos++;
					width++;
				}
				if(input[str_pos] != '\0' && input[str_pos] != '\n'){
					while(input[str_pos] != '\n' && input[str_pos] != '\0' && !is_column_all_spaces(input, text_length, str_pos, line_length)){
						number[number_pos] = '.';
						number_pos++;
						str_pos++;
						width++;
					}
				}
				number[number_pos] = '\0';
				calculations[calculation_pos].numbers[line] = number;
				if(calculations[calculation_pos].width == 0){
					calculations[calculation_pos].width = width;
				}
				if(width != calculations[calculation_pos].width){
					printf("Error: Different width found: %s %s\n",calculations[calculation_pos].numbers[0],calculations[calculation_pos].numbers[line]);
				}

				if(input[str_pos] == ' '){
					str_pos++;
				}
				calculation_pos++;
			}
			if(input[str_pos] == '\n'){
				str_pos++;
			} else if(input[str_pos] != '\0'){
				printf("Error: Ended line at char %c\n",input[str_pos]);
			}
			if(calculation_count == 0){
				calculation_count = calculation_pos;
			} else if(calculation_pos != calculation_count){
				printf("Error: Different amount of calculations found\n");
			}
			line++;
		} else {
			size_t calculation_pos = 0;
			while(input[str_pos] != '\n' && input[str_pos] != '\0'){
				while(input[str_pos] == ' '){
					str_pos++;
				}
				if(input[str_pos] != '\0'){
					calculations[calculation_pos].op = input[str_pos];
					str_pos++;
					calculation_pos++;
				}
			}
		}
	}
	uint64_t sum = 0;
	for(size_t calculation_pos = 0; calculation_pos < calculation_count; ++calculation_pos){
		uint64_t result;
		if(calculations[calculation_pos].op == '+'){
			result = 0;
		} else {
			result = 1;
		}
		for(size_t vertical_num_pos = 0; vertical_num_pos < calculations[calculation_pos].width; ++vertical_num_pos){
			uint64_t vertical_number = 0;
			for(size_t num_pos = 0; num_pos < line; ++num_pos){
				char digit = calculations[calculation_pos].numbers[num_pos][vertical_num_pos];
				if(digit != '.'){
					vertical_number = vertical_number * 10;
					vertical_number = vertical_number + (digit - '0');
				}
			}
			if(calculations[calculation_pos].op == '+'){
				result += vertical_number;
			} else {
				result *= vertical_number;
			}
		}
		sum += result;
	}
	for(size_t calculation_pos = 0; calculation_pos < calculation_count; ++calculation_pos){
		for(size_t num_pos = 0; num_pos < line; ++num_pos){
			free(calculations[calculation_pos].numbers[num_pos]);
		}
	}
	printf("Result: %ld\n",sum);
}

Solution create_day_06_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

