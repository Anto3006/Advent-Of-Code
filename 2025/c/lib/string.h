#ifndef STRING_H
#define STRING_H

#include "stdlib.h"
#include "stdbool.h"

char* str_copy(const char *string);
size_t str_len(const char *string);
bool is_str_eq(const char *string, const char *other);
bool is_str_integer(const char *string);

#endif
