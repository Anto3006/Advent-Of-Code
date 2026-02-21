#ifndef SOLUTION_H
#define SOLUTION_H

#include "stdint.h"
#include "stddef.h"
#define SOLUTION_COUNT 12

typedef struct {
	void (*part_1)(const char*);
	void (*part_2)(const char*);
	const char* test_input;
} Solution;


Solution create_day_01_solution();
Solution create_day_02_solution();
Solution create_day_03_solution();
Solution create_day_04_solution();
void run_solution(Solution *solutions, uint32_t day, uint32_t part, const char* input);
Solution *create_solution_array();

#endif

