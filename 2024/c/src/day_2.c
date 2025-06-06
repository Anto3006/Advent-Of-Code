#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/utilities.h"
#include "../lib/vector.h"

bool is_report_safe(vector_uint_t*);
bool is_report_safe_with_problem_dampener(vector_uint_t*);
bool is_report_safe_with_removal(vector_uint_t*, size_t);
uint32_t count_safe_reports(char*, bool);
uint32_t distance(uint32_t, uint32_t);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		uint32_t safe_count = count_safe_reports(data, false);
		uint32_t safe_count_with_problem_dampener = count_safe_reports(data, true);
		free(data);
		printf("Safe report count: %u\n",safe_count);
		printf("Safe report count with problem dampener: %u\n",safe_count_with_problem_dampener);
	}
}

uint32_t distance(uint32_t first, uint32_t second){
	if (first >= second){
		return first - second;
	} else {
		return second - first;
	}
}

bool is_report_safe(vector_uint_t* report){
	if (report->length <= 1){
		return true;
	}
	bool is_increasing = report->data[0] <= report->data[1];
	for(size_t index = 0; index < report->length-1; ++index){
		uint32_t current = report->data[index];
		uint32_t next = report->data[index+1];
		if (current == next || distance(current, next) > 3){
			return false;
		} else if(is_increasing && current > next){
			return false;
		} else if(!is_increasing && current < next){
			return false;
		}
	}
	return true;
}

uint32_t count_safe_reports(char* data, bool is_using_problem_dampener){
	vector_str_t lines = split_string(data, "\n");
	uint32_t safe_count = 0;
	for(size_t line_index = 0; line_index < lines.length; ++line_index){
		vector_str_t report_data = split_string(lines.data[line_index], " ");
		vector_uint_t report = parse_vector_uint(&report_data);
		if (!is_using_problem_dampener && is_report_safe(&report)){
			safe_count++;
		} else if (is_using_problem_dampener && is_report_safe_with_problem_dampener(&report)){
			safe_count++;
		}
		free_vector_str(&report_data);
		free_vector_uint(&report);
	}
	free_vector_str(&lines);
	return safe_count;
}

bool is_report_safe_with_problem_dampener(vector_uint_t* report){
	for(size_t index_removed = 0; index_removed < report->length; index_removed++){
		if(is_report_safe_with_removal(report, index_removed)){
			return true;
		}
	}
	return false;
}
bool is_report_safe_with_removal(vector_uint_t* report, size_t index_removed){
	if(index_removed >= report->length){
		return false;
	}
	if(report->length <= 2){
		return true;
	}
	size_t starting_index = 0;
	if(index_removed == 0){
		starting_index = 1;
	}
	size_t end_index = report->length-1;
	if(index_removed == end_index){
		end_index -= 1;
	}
	bool is_increasing;
	if(starting_index+1 == index_removed){
		is_increasing = report->data[starting_index] <= report->data[starting_index+2];
	} else {
		is_increasing = report->data[starting_index] <= report->data[starting_index+1];
	}
	for(size_t index = starting_index; index < end_index; index++){
		size_t current_index = index;
		if(current_index == index_removed){
			current_index -= 1;
		}
		size_t next_index = index+1;
		if(next_index == index_removed){
			next_index += 1;
		}
		uint32_t current = report->data[current_index];
		uint32_t next = report->data[next_index];
		if (current == next || distance(current, next) > 3){
			return false;
		} else if(is_increasing && current > next){
			return false;
		} else if(!is_increasing && current < next){
			return false;
		}
	}
	return true;
}
