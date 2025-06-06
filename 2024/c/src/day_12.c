#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

#define REGION_CAPACITY 256

const size_t INITIAL_REGION_CAP = 3;


typedef struct{
	uint32_t perimeter;
	uint64_t area;
	char id;
} region_t;

typedef struct {
	region_t* regions[REGION_CAPACITY];
	size_t region_capacity[REGION_CAPACITY];
	size_t region_count[REGION_CAPACITY];
	char* garden;
	size_t row_count;
	size_t col_count;
} garden_t;

void initialize_garden(garden_t* garden, char* garden_data);
void free_garden(garden_t* garden);
void detect_regions(garden_t* garden);
void discover_region(garden_t* garden, size_t row, size_t col, region_t* region, bool* visited);
char get_garden_value(garden_t* garden, size_t row, size_t col);
region_t* add_region(garden_t* garden, size_t id);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		garden_t garden;
		initialize_garden(&garden, data);
		detect_regions(&garden);
		uint64_t price = 0;
		for(size_t region_id = 0; region_id < REGION_CAPACITY; ++region_id){
			for(size_t region_index = 0; region_index < garden.region_count[region_id]; ++region_index){
				region_t* region = &garden.regions[region_id][region_index];
				price += region->area*region->perimeter;
			}
		}
		printf("Result part 1: %lu\n", price);
		free_garden(&garden);
	}
	return 0;
}

void initialize_garden(garden_t* garden, char* garden_data){
	for(size_t index = 0; index < REGION_CAPACITY; ++index){
		garden->regions[index] = calloc(INITIAL_REGION_CAP, sizeof(region_t));
		garden->region_capacity[index] = INITIAL_REGION_CAP;
		garden->region_count[index] = 0;
	}
	garden->garden = garden_data;
	garden->row_count = 0;
	garden->col_count = 0;
	char c = 1;
	size_t index = 0;
	while(c != '\0'){
		c = garden_data[index];
		if(c == '\n'){
			if(garden->col_count == 0){
				garden->col_count = index;
			}
			garden->row_count++;
		}
		index++;
	}
}

void free_garden(garden_t* garden){
	free(garden->garden);
	for(size_t index = 0; index < REGION_CAPACITY; ++index){
		free(garden->regions[index]);
	}
}

char get_garden_value(garden_t* garden, size_t row, size_t col){
	return garden->garden[row*garden->col_count + col + row];
}

void detect_regions(garden_t* garden){
	bool* visited = calloc(garden->row_count*garden->col_count, sizeof(bool));
	for(size_t index = 0; index < garden->row_count*garden->col_count; ++index){
		visited[index] = false;
	}
	for(size_t row = 0; row < garden->row_count; ++row){
		for(size_t col = 0; col < garden->col_count; ++col){
			if(!visited[row*garden->col_count + col]){
				size_t region_id = get_garden_value(garden, row, col);
				region_t* region = add_region(garden, region_id);
				discover_region(garden, row, col, region, visited);
			}
		}
	}
}

size_t row_col_to_index(size_t col_count, size_t row, size_t col){
	return row*col_count + col;
}

void discover_region(garden_t* garden, size_t row, size_t col, region_t* region, bool* visited){
	visited[row*garden->col_count + col] = true;
	region->area += 1;
	region->perimeter += 4;
	if(row > 0 && get_garden_value(garden, row-1, col) == region->id){
		region->perimeter -= 1;
		if(!visited[row_col_to_index(garden->col_count, row-1,col)]){
			discover_region(garden, row-1, col, region, visited);
		}
	}
	if(row < garden->row_count-1 && get_garden_value(garden, row+1, col) == region->id){
		region->perimeter -= 1;
		if(!visited[row_col_to_index(garden->col_count, row+1,col)]){
			discover_region(garden, row+1, col, region, visited);
		}
	}
	if(col > 0 && get_garden_value(garden, row, col-1) == region->id){
		region->perimeter -= 1;
		if(!visited[row_col_to_index(garden->col_count, row,col-1)]){
			discover_region(garden, row, col-1, region, visited);
		}
	}
	if(col < garden->col_count-1 && get_garden_value(garden, row, col+1) == region->id){
		region->perimeter -= 1;
		if(!visited[row_col_to_index(garden->col_count, row,col+1)]){
			discover_region(garden, row, col+1, region, visited);
		}
	}
}

region_t* add_region(garden_t* garden, size_t id){
	size_t count = garden->region_count[id];
	size_t capacity = garden->region_capacity[id];
	if(count == capacity){
		garden->region_capacity[id] = 2*capacity;
		garden->regions[id] = realloc(garden->regions[id],2*capacity*sizeof(region_t));
	}
	garden->regions[id][count].id = id;
	garden->regions[id][count].area = 0;
	garden->regions[id][count].perimeter = 0;
	garden->region_count[id]++;
	return &garden->regions[id][count];
}
