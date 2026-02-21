#include "stdlib.h"
#include "../lib/argument_reader.h"
#include "stdint.h"
#include "solution.h"
#include "../lib/utilities.h"
#include "stdio.h"
#include "../lib/string.h"

int main(int argc, char** argv){
	ArgumentReader arg_reader = new_argument_reader();
	read_arguments(&arg_reader, argv,  argc);

	uint32_t day;
	uint32_t part;
	char *day_str = get_value(&arg_reader.argument_map, "-d");
	if (day_str == 0) {
		printf("Day value not found, defaulting to day 1.\n");
		day = 1;
	} else {
		if(is_str_integer(day_str)){
			day = atoi(day_str);
		} else {
			printf("Day value invalid. Must be an integer. Defaulting to day 1.\n");
			day = 1;
		}
	}
	char *part_str = get_value(&arg_reader.argument_map, "-p");
	if (day_str == 0) {
		printf("Part value not found, defaulting to part 1\n");
		part = 1;
	} else {
		if(is_str_integer(part_str)){
			part = atoi(part_str);
		}else {
			printf("Part value invalid. Must be an integer. Defaulting to part 1.\n");
			part = 1;
		}

	}

	char *file_addr = get_value(&arg_reader.argument_map, "-f");
	char *input = 0;
	if(file_addr != 0){
		input = read_file(file_addr, 1024, 2);
	}
	Solution *solutions = create_solution_array();
	run_solution(solutions, day, part, input);
	free(solutions);
	free(input);
	free_map(&arg_reader.argument_map);
	return 0;
}
