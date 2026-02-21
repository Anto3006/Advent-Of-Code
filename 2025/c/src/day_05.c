#include "solution.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include <stdint.h>

typedef struct {
	uint64_t begin;
	uint64_t end;
} NumberRange;

static const char* test_input = 
	"3-5\n"
	"10-14\n"
	"16-20\n"
	"12-18\n"
	"\n"
	"1\n"
	"5\n"
	"8\n"
	"11\n"
	"17\n"
	"32\n";

static void part_1(const char *input){
	NumberRange ranges[1024];
	size_t range_count = 0;
	size_t str_pos = 0;
	bool is_reading_ranges = true;
	uint64_t count = 0;
	while(input[str_pos] != '\0'){
		char ch = input[str_pos];
		if(is_reading_ranges){
			uint64_t begin = 0;
			uint64_t end = 0;
			while(ch != '-'){
				begin = begin * 10;
				begin = begin + (ch - '0');
				str_pos++;
				ch = input[str_pos];
			}
			str_pos++;
			ch = input[str_pos];
			while(ch != '\n'){
				end = end * 10;
				end = end + (ch - '0');
				str_pos++;
				ch = input[str_pos];
			}
			ranges[range_count].begin = begin;
			ranges[range_count].end = end;
			range_count++;
		} else {
			uint64_t id = 0;
			while(ch != '\n'){
				id = id * 10;
				id = id + (ch - '0');
				str_pos++;
				ch = input[str_pos];
			}
			for(size_t range_pos = 0; range_pos < range_count; ++range_pos){
				NumberRange range = ranges[range_pos];
				if(range.begin <= id && id <= range.end){
					count++;
					break;
				}
			}
		}
		str_pos++;
		if(input[str_pos] == '\n'){
			str_pos++;
			is_reading_ranges = false;
		}
	}
	printf("Result: %ld\n",count);
}

static void part_2(const char *input){
	NumberRange ranges[1024];
	size_t range_count = 0;
	size_t str_pos = 0;
	bool is_reading_ranges = true;
	uint64_t count = 0;
	while(input[str_pos] != '\0' && is_reading_ranges){
		char ch = input[str_pos];
		if(is_reading_ranges){
			uint64_t begin = 0;
			uint64_t end = 0;
			while(ch != '-'){
				begin = begin * 10;
				begin = begin + (ch - '0');
				str_pos++;
				ch = input[str_pos];
			}
			str_pos++;
			ch = input[str_pos];
			while(ch != '\n'){
				end = end * 10;
				end = end + (ch - '0');
				str_pos++;
				ch = input[str_pos];
			}
			for(size_t range_pos = 0; range_pos < range_count; ++range_pos){
				if(begin <= ranges[range_pos].begin && ranges[range_pos].end <= end){
					for(size_t pos = range_pos; pos < range_count-1; ++pos){
						ranges[pos] = ranges[pos+1];
					}
					range_count--;
				} else{
					if(ranges[range_pos].begin <= begin && begin <= ranges[range_pos].end){
						begin = ranges[range_pos].end + 1;
					}
					if(ranges[range_pos].begin <= end && end <= ranges[range_pos].end){
						end = ranges[range_pos].begin - 1;
					}
				}
			}
			if(begin <= end){
				ranges[range_count].begin = begin;
				ranges[range_count].end = end;
				range_count++;
			}
		}		
		str_pos++;
		if(input[str_pos] == '\n'){
			str_pos++;
			is_reading_ranges = false;
		}
	}
	for(size_t range_pos = 0; range_pos < range_count; ++range_pos){
		count += ranges[range_pos].end - ranges[range_pos].begin + 1;
	}
	printf("Result: %ld\n",count);
}

Solution create_day_05_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

