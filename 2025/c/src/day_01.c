#include "solution.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"

const char* test_input =
	"L68\n"
	"L30\n"
	"R48\n"
	"L5\n"
	"R60\n"
	"L55\n"
	"L1\n"
	"L99\n"
	"R14\n"
	"L82\n";

static void part_1(const char *input){
	uint64_t str_pos = 0;
	char direction;
	uint16_t rotation = 0;
	int16_t position = 50;
	uint32_t count = 0;
	while (input[str_pos] != '\0'){
		direction = input[str_pos];
		str_pos++;
		while(input[str_pos] != '\n' && input[str_pos] != '\0'){
			rotation *= 10;
			rotation += (input[str_pos] - '0');
			str_pos++;
		}
		if (input[str_pos] == '\n'){
			str_pos++;
		}
		rotation = rotation % 100;
		if(direction == 'L'){
			position -= rotation;
			if(position < 0){
				position += 100;
			}
		} else if(direction == 'R'){
			position += rotation;
			if(position >= 100){
				position = position - 100;
			}
		} else {
			printf("Error in direction: Direction can only be L or R\n");
			return;
		}
		if (position == 0){
			count += 1;
		}
		rotation = 0;
	}
	printf("Result: %d\n",count);
}

static void part_2(const char *input){
	uint64_t str_pos = 0;
	char direction;
	uint32_t rotation = 0;
	int32_t position = 50;
	uint32_t count = 0;
	bool was_at_zero = false;
	while (input[str_pos] != '\0'){
		direction = input[str_pos];
		str_pos++;
		while(input[str_pos] != '\n' && input[str_pos] != '\0'){
			rotation *= 10;
			rotation += (input[str_pos] - '0');
			str_pos++;
		}
		if (input[str_pos] == '\n'){
			str_pos++;
		}
		count += rotation / 100;
		rotation = rotation % 100;
		if(direction == 'L'){
			position -= rotation;
			if(position < 0){
				position += 100;
				if(!was_at_zero){
					count += 1;
				}
			}
		} else if(direction == 'R'){
			position += rotation;
			if(position > 100){
				position -= 100;
				if(!was_at_zero){
					count += 1;
				}
			} else if (position == 100){
				position -= 100;
			}
		} else {
			printf("Error in direction: Direction can only be L or R\n");
			return;
		}
		if (position == 0){
			if(rotation > 0){
				count += 1;
			}
		}
		was_at_zero = position == 0;
		rotation = 0;
	}
	printf("Result: %d\n",count);
}

Solution create_day_01_solution(){
	Solution solution;
	solution.part_1 = part_1;
	solution.part_2 = part_2;
	solution.test_input = test_input;
	return solution;
}

