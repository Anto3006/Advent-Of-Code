#include "string.h"
#include "stdlib.h"

char* str_copy(const char *string){
	size_t len = str_len(string);
	char *copy = calloc(len+1, sizeof(char));
	for (size_t pos = 0; pos < len; pos++){
		copy[pos] = string[pos];
	}
	copy[len] = '\0';
	return copy;
}

size_t str_len(const char *string){
	size_t len = 0;
	while (string[len] != '\0') {
		len += 1;
	}
	return len;
}

bool is_str_eq(const char *string, const char *other){
	size_t pos = 0;
	while (string[pos] != '\0'){
		if (other[pos] == '\0'){
			return false;
		} else if(string[pos] != other[pos]){
			return false;
		}
		pos++;
	}
	if (other[pos] == '\0'){
		return true;
	} else {
		return false;
	}
}
