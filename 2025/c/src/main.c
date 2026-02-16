#include "stdlib.h"
#include "../lib/argument_reader.h"
#include "stdint.h"
#include "solution.h"
#include "../lib/utilities.h"

int main(int argc, char** argv){
	ArgumentReader arg_reader = new_argument_reader();
	read_arguments(&arg_reader, argv,  argc);
	uint32_t day;
	uint32_t part;
	day = atoi(get_value(&arg_reader.argument_map,"-d"));
	part = atoi(get_value(&arg_reader.argument_map,"-p"));
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
