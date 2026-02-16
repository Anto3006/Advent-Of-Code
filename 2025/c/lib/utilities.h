#ifndef UTILITIES_LIB_H
#define UTILITIES_LIB_H

#include <stdint.h>
#include <stdlib.h>

#define CHUNK_SIZE 1000

char* read_file(const char* file_path, const uint32_t initial_size, const uint8_t growth_rate);
char* get_string_slice(const char* str, const size_t initial_pos, const size_t length);


#endif // !UTILITIES_LIB_H
