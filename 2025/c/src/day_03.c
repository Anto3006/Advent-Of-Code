#include "solution.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"

static const char* test_input = 
	"987654321111111\n"
	"811111111111119\n"
	"234234234234278\n"
	"818181911112111\n";

static bool is_char_end_of_line(const char c){
	return c == '\n' || c == '\0';
}

static size_t find_biggest_digit_pos_in_line(const char *string, size_t initial_pos, size_t limit_pos){
	size_t pos = initial_pos;
	char biggest_digit = string[pos];
	size_t biggest_digit_pos = initial_pos;
	while(pos >= limit_pos){
		if (string[pos] >= biggest_digit){
			biggest_digit_pos = pos;
			biggest_digit = string[pos];
		}
		if(pos == 0){
			break;
		}
		pos--;
	}
	return biggest_digit_pos;
}

static uint64_t find_biggest_number_in_line(const char *string, size_t length, size_t pos_end_line){
	uint64_t biggest_number = 0;
	size_t limit_pos = 0;
	for(size_t digit = 0; digit < length; ++digit){
		size_t initial_pos = pos_end_line - length + 1 + digit;
		size_t pos_biggest_digit = find_biggest_digit_pos_in_line(string, initial_pos, limit_pos);
		char biggest_digit = string[pos_biggest_digit];
		biggest_number = biggest_number * 10 + (biggest_digit - '0');
		limit_pos = pos_biggest_digit + 1;
	}
	return biggest_number;
}

static void part_1(const char *input){
	size_t str_pos = 0;
	uint64_t sum = 0;
	size_t begin_line_pos = 0;
	while(input[str_pos] != '\0'){
		while(!is_char_end_of_line(input[str_pos])){
			str_pos++;
		}
		sum += find_biggest_number_in_line(&input[begin_line_pos],2,str_pos-1-begin_line_pos);
		if(input[str_pos] == '\n'){
			str_pos++;
			begin_line_pos = str_pos;
		}
	}
	printf("Result: %ld\n",sum);
}

static void part_2(const char *input){
	size_t str_pos = 0;
	uint64_t sum = 0;
	size_t begin_line_pos = 0;
	while(input[str_pos] != '\0'){
		while(!is_char_end_of_line(input[str_pos])){
			str_pos++;
		}
		sum += find_biggest_number_in_line(&input[begin_line_pos],12,str_pos-1-begin_line_pos);
		if(input[str_pos] == '\n'){
			str_pos++;
			begin_line_pos = str_pos;
		}
	}
	printf("Result: %ld\n",sum);
}

Solution create_day_03_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

