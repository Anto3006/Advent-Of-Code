#include "argument_reader.h"
#include "map.h"
#include "string.h"
#include <stdio.h>

void read_arguments(ArgumentReader *argument_reader, char **args, size_t argc){
	size_t arg_pos = 1;
	bool found_argument_name = false;
	char* arg_name;
	while (arg_pos < argc){
		char* arg = args[arg_pos];
		if (str_len(arg) > 0 && arg[0] == '-'){
			if (found_argument_name){
				insert(&argument_reader->argument_map, arg_name, "");
				found_argument_name = false;
			}
			arg_name = arg;
			found_argument_name = true;
		} else {
			if (found_argument_name){
				insert(&argument_reader->argument_map, arg_name, arg);
			} else{
				printf("Error: Not found argument name for value: %s\n", arg);
			}
			found_argument_name = false;
		}
		arg_pos += 1;
	}
	if(found_argument_name){
		insert(&argument_reader->argument_map, arg_name, "");
	}
}

ArgumentReader new_argument_reader(){
	ArgumentReader arg_reader;
	arg_reader.argument_map = new_map(1024);
	return arg_reader;
}
