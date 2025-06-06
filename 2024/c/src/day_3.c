#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/utilities.h"

enum search_state {
	Scouting,
	Flags,
	Mult,
	First_Number,
	Second_Number
};

uint32_t calculate_sum(char*, bool);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		uint32_t sum = calculate_sum(data,false);
		uint32_t sum_with_flags = calculate_sum(data,true);
		printf("Result part 1: %u\n",sum);
		printf("Result part 2: %u\n",sum_with_flags);
		free(data);
	}
}

uint32_t calculate_sum(char* file_data, bool is_using_flags){
	uint32_t sum = 0;
	uint32_t first_number = 0;
	uint32_t second_number = 0;

	enum search_state state = Scouting;
	bool adding_flag = true;
	bool possible_do_flag = false;
	bool possible_dont_flag = false;

	char number_buffer[4];
	size_t number_buffer_pos = 0;

	const char* mult = "mul(";
	const char* do_flag = "do()";
	size_t do_flag_length = 4;
	const char* dont_flag = "don\'t()";
	size_t dont_flag_length = 7;

	size_t initial_search_pos = 0;

	size_t index = 0;
	char current_char = file_data[index];
	bool keep_index = false;
	while(current_char != '\0'){
		keep_index = false;
		if(state == Scouting){
			if(adding_flag && current_char == mult[0]){
				state = Mult;
				initial_search_pos = 1;
			} else if(is_using_flags && current_char == do_flag[0]){
				state = Flags;
				initial_search_pos = 1;
				possible_do_flag = true;
				possible_dont_flag = true;
			} 
		} else if(state == Mult){
			if(current_char == mult[initial_search_pos]){
				initial_search_pos += 1;
				if(mult[initial_search_pos] == '\0'){
					state = First_Number;
					initial_search_pos = 0;
				}
			} else {
				if(initial_search_pos > 1){
					keep_index = true;
				}
				initial_search_pos = 0;
				state = Scouting;
			}
		} else if(state == First_Number){
			if(current_char >= '0' && current_char <= '9'){
				if(number_buffer_pos <= 3){
					number_buffer[number_buffer_pos] = current_char;
					number_buffer_pos++;
				} else {
					state = Scouting;
					number_buffer_pos = 0;
				}
			} else if(current_char == ','){
				if(number_buffer_pos == 0){
					state = Scouting;
				} else {
					number_buffer[number_buffer_pos] = '\0';
					first_number = atoi(number_buffer);
					state = Second_Number;
					number_buffer_pos = 0;
				}
			} else {
				keep_index = true;
				state = Scouting;
				number_buffer_pos = 0;
			}
		} else if(state == Second_Number){
			if(current_char >= '0' && current_char <= '9'){
				if(number_buffer_pos <= 3){
					number_buffer[number_buffer_pos] = current_char;
					number_buffer_pos++;
				} else {
					state = Scouting;
					number_buffer_pos = 0;
				}
			} else if(current_char == ')'){
				if(number_buffer_pos == 0){
					state = Scouting;
				} else {
					number_buffer[number_buffer_pos] = '\0';
					second_number = atoi(number_buffer);
					state = Scouting;
					sum += first_number * second_number;
					number_buffer_pos = 0;
				}
			} else {
				keep_index = true;
				state = Scouting;
				number_buffer_pos = 0;
			}
		} else if(state == Flags){
			if(possible_do_flag && initial_search_pos < do_flag_length && current_char == do_flag[initial_search_pos]){
				
			} else {
				possible_do_flag = false;
			}
			if(possible_dont_flag && initial_search_pos < dont_flag_length && current_char == dont_flag[initial_search_pos]){

			} else {
				possible_dont_flag = false;
			}
			if(possible_do_flag || possible_dont_flag){
				initial_search_pos += 1;
			}
			if(initial_search_pos == do_flag_length && possible_do_flag){
				initial_search_pos = 0;
				state = Scouting;
				adding_flag = true;
			}
			if(initial_search_pos == dont_flag_length && possible_dont_flag){
				initial_search_pos = 0;
				state = Scouting;
				adding_flag = false;
			}
			if(!possible_do_flag && !possible_dont_flag){
				initial_search_pos = 0;
				keep_index = true;
				state = Scouting;
			}
		}

		if(!keep_index){
			index += 1;
		}
		current_char = file_data[index];
	}
	return sum;
}
