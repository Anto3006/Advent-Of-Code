#include "solution.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"

typedef struct {
	uint32_t upper_half;
	uint32_t lower_half;
} NumberHalfs;

static const char* test_input = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124";

static uint64_t power(uint64_t base, uint64_t exponent){
	if (exponent == 0){
		return 1;
	} else if (exponent == 1){
		return base;
	} else if(exponent % 2 == 0){
		return power(base * base, exponent / 2);
	} else {
		return power(base * base, (exponent - 1)/2) * base;
	}
}

static uint32_t count_digits(uint64_t number){
	uint32_t count = 0;
	while (number > 0){
		number = number / 10;
		count++;
	}
	return count;
}

static NumberHalfs separate_number(uint64_t number){
	uint32_t digit_count = count_digits(number);
	NumberHalfs result;
	result.upper_half = 0;
	result.lower_half = 0;
	if (digit_count % 2 == 1){
		printf("Cannot separate the number in two halves because digit count is not even\n");
	} else {
		uint64_t offset = power(10,digit_count/2); 
		result.upper_half = number / offset;
		result.lower_half = number % offset;
	}
	return result;
}

static uint64_t sum_invalid_ids(uint64_t begin, uint64_t end){
	uint32_t digit_count_begin = count_digits(begin);
	uint32_t digit_count_end = count_digits(end);
	uint64_t sum = 0;
	if(digit_count_begin == digit_count_end){
		if(digit_count_begin % 2 == 1){
			return 0;
		} else {
			NumberHalfs begin_halves = separate_number(begin);
			if(begin_halves.upper_half >= begin_halves.lower_half){
				uint32_t current_pattern = begin_halves.upper_half;
				uint64_t number = current_pattern * power(10,digit_count_begin/2) + current_pattern;
				while(number <= end){
					sum += number;
					current_pattern += 1;
					number = current_pattern * power(10,digit_count_begin/2) + current_pattern;
				}
			} else {
				uint32_t current_pattern = begin_halves.upper_half+1;
				uint64_t number = current_pattern * power(10,digit_count_begin/2) + current_pattern;
				while(number <= end){
					sum += number;
					current_pattern += 1;
					number = current_pattern * power(10,digit_count_begin/2) + current_pattern;
				}
			}
			return sum;
		}
	} else {
		if(digit_count_begin == digit_count_end - 1){
			uint64_t aux = power(10, digit_count_end-1);
			return sum_invalid_ids(begin, aux-1) + sum_invalid_ids(aux,end);
		} else {
			printf("More than one digit difference between begin and end\n");
		}
	}
	return sum;
}

static uint32_t get_first_sequence(uint64_t number, uint32_t sequence_size){
	uint64_t aux_power_10 = power(10, sequence_size);
	uint64_t sequence = number;
	while (sequence >= aux_power_10){
		sequence = sequence / 10;
	}
	return sequence;
}

static uint64_t create_number_with_repeating_pattern(uint32_t pattern, uint32_t pattern_count){
	uint64_t aux_power_10 = power(10, count_digits(pattern));
	uint64_t number = 0;
	for(uint32_t count = 0; count < pattern_count; ++count){
		number = number * aux_power_10;
		number += pattern;
	}
	return number;
}

static uint64_t sum_patterned_numbers_in_range_static_size(uint64_t begin, uint64_t end, uint32_t pattern_size){
	uint32_t digit_count_begin = count_digits(begin);
	uint32_t digit_count_end = count_digits(end);
	uint64_t sum = 0;
	if(digit_count_begin == digit_count_end){
		if(digit_count_begin % pattern_size != 0){
			return 0;
		} else {
			uint32_t current_pattern = get_first_sequence(begin,pattern_size);
			uint32_t pattern_count = digit_count_begin / pattern_size;
			uint64_t number = create_number_with_repeating_pattern(current_pattern, pattern_count);
			while (number < begin){
				current_pattern += 1;
				number = create_number_with_repeating_pattern(current_pattern, pattern_count);
			}
			while(number <= end){
				sum += number;
				current_pattern += 1;
				number = create_number_with_repeating_pattern(current_pattern, pattern_count);
			}
			return sum;
		}
	} else {
		printf("Begin and end must have the same amount of digits\n");
	}
	return sum;
}

static uint64_t sum_patterned_numbers_in_range(uint64_t begin, uint64_t end){
	uint32_t digit_count_begin = count_digits(begin);
	uint32_t digit_count_end = count_digits(end);
	uint64_t sum = 0;

	if(digit_count_begin == digit_count_end){
		switch (digit_count_begin) {
			case 1:
				break;
			case 2:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 1);
				break;
			case 3:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 1);
				break;
			case 4:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 2);
				break;
			case 5:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 1);
				break;
			case 6:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 3) + sum_patterned_numbers_in_range_static_size(begin, end, 2) - sum_patterned_numbers_in_range_static_size(begin, end, 1);
				break;
			case 7:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 1);
				break;
			case 8:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 4);
				break;
			case 9:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 3);
				break;
			case 10:
				sum = sum_patterned_numbers_in_range_static_size(begin, end, 5) + sum_patterned_numbers_in_range_static_size(begin, end, 2) - sum_patterned_numbers_in_range_static_size(begin, end, 1);
				break;
			default:
				printf("Not implemented for numbers with 11 or more digits\n");
				break;
		}
		return sum;
	} else {
		if(digit_count_begin == digit_count_end - 1){
			uint64_t aux = power(10, digit_count_end-1);
			return sum_patterned_numbers_in_range(begin, aux-1) + sum_patterned_numbers_in_range(aux,end);
		} else {
			printf("More than one digit difference between begin and end\n");
		}
	}
	return 0;
}

static void part_1(const char *input){
	uint64_t sum = 0;
	size_t str_pos = 0;
	uint64_t begin = 0;
	uint64_t end = 0;
	while (input[str_pos] >= '0'  && input[str_pos] <= '9'){
		begin = 0;
		while(input[str_pos] != '-'){
			begin = begin * 10;
			begin = begin + (input[str_pos] - '0');
			str_pos++;
		}
		str_pos++;
		end = 0;
		while(input[str_pos] >= '0'  && input[str_pos] <= '9'){
			end = end * 10;
			end = end + (input[str_pos] - '0');
			str_pos++;
		}
		if(input[str_pos] != '\0'){
			str_pos++;
		}

		sum += sum_invalid_ids(begin,end);
	}
	printf("Result: %ld\n",sum);
	return;
}

static void part_2(const char *input){
	uint64_t sum = 0;
	size_t str_pos = 0;
	uint64_t begin = 0;
	uint64_t end = 0;
	while (input[str_pos] >= '0'  && input[str_pos] <= '9'){
		begin = 0;
		while(input[str_pos] != '-'){
			begin = begin * 10;
			begin = begin + (input[str_pos] - '0');
			str_pos++;
		}
		str_pos++;
		end = 0;
		while(input[str_pos] >= '0'  && input[str_pos] <= '9'){
			end = end * 10;
			end = end + (input[str_pos] - '0');
			str_pos++;
		}
		if(input[str_pos] != '\0'){
			str_pos++;
		}

		sum += sum_patterned_numbers_in_range(begin,end);
	}
	printf("Result: %ld\n",sum);
	return;
}

Solution create_day_02_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

