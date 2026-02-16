#ifndef ARGUMENT_READER_H
#define ARGUMENT_READER_H

#include "map.h"

typedef struct {
	MapStringString argument_map;
} ArgumentReader;

void read_arguments(ArgumentReader *argument_reader, char** args, size_t argc);
ArgumentReader new_argument_reader();
#endif
