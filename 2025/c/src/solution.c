#include "solution.h"
#include "stdio.h"
#include "stdlib.h"

Solution *create_solution_array(){
	Solution *solution_array = calloc(SOLUTION_COUNT, sizeof(Solution));
	solution_array[0] = create_day_01_solution();
	solution_array[1] = create_day_02_solution();
	solution_array[2] = create_day_03_solution();
	solution_array[3] = create_day_04_solution();
	solution_array[4] = create_day_05_solution();
	for(size_t pos = 5; pos < SOLUTION_COUNT; ++pos){
		Solution sol;
		sol.test_input = 0;
		sol.part_1 = 0;
		sol.part_2 = 0;
		solution_array[pos] = sol;
	}
	return solution_array;
}

void run_solution(Solution *solutions, uint32_t day, uint32_t part, const char *input){
	if(day > SOLUTION_COUNT || day == 0){
		printf("Error: Day is incorrect\n");
		return;
	}
	if(part > 2 || part < 1){
		printf("Error: Part is incorrect\n");
		return;
	}
	if (input == 0){
		input = solutions[day-1].test_input;
	}
	if (part == 1){
		solutions[day-1].part_1(input);
	} else {
		solutions[day-1].part_2(input);
	}
}
