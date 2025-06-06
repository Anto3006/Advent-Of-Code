#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../lib/vector.h"
#include "../lib/utilities.h"

typedef struct {
	vector_uint_t before;
	vector_uint_t after;
	uint32_t number;
} order_rules_t;

void initialize_rule(order_rules_t*, uint32_t);
void add_rule(order_rules_t*,uint32_t,uint32_t);
order_rules_t* parse_rules(char*); //Numbers between 0 and 99 only
bool check_page_ordering(vector_uint_t* , order_rules_t*);
uint32_t calculate_sum_middle_correct_orders(char*,order_rules_t*);
void correct_page_ordering(vector_uint_t*, order_rules_t*);
uint32_t calculate_sum_middle_incorrect_orders(char*,order_rules_t*);

int main(int argc, char** argv){
	if (argc > 1){
		printf("%s\n",argv[1]);
		char* data = read_file(argv[1],1,2);
		vector_str_t information = split_string(data, "\n\n");
		char* rules_data = information.data[0];
		char* page_order_data = information.data[1];
		order_rules_t* rules = parse_rules(rules_data);
		uint32_t sum_correct = calculate_sum_middle_correct_orders(page_order_data,rules);
		uint32_t sum_incorrect = calculate_sum_middle_incorrect_orders(page_order_data,rules);
		for(size_t number = 0; number < 100; ++number){
			free_vector_uint(&rules[number].before);
			free_vector_uint(&rules[number].after);
		}
		free(rules);
		free_vector_str(&information);
		free(data);
		printf("Result part 1: %u\n",sum_correct);
		printf("Result part 2: %u\n",sum_incorrect);
	}
	return 0;
}

void add_rule(order_rules_t* rules, uint32_t before, uint32_t after){
	append_vector_uint(&rules[before].after,after);
	append_vector_uint(&rules[after].before,before);
}

void initialize_rule(order_rules_t* rule, uint32_t number){
	initialize_vector_uint(&rule->before);
	initialize_vector_uint(&rule->after);
	rule->number = number;
}

order_rules_t* parse_rules(char* rules_data){
	order_rules_t* rules = (order_rules_t*) calloc(100,sizeof(order_rules_t));
	for(size_t number = 0; number < 100; ++number){
		initialize_rule(&rules[number], number);
	}
	vector_str_t rule_lines = split_string(rules_data,"\n");
	for(size_t index = 0; index < rule_lines.length; ++index){
		char* rule_line = rule_lines.data[index];
		vector_str_t rule_data_str = split_string(rule_line,"|");
		uint32_t before = atoi(rule_data_str.data[0]);
		uint32_t after = atoi(rule_data_str.data[1]);
		add_rule(rules, before, after);
		free_vector_str(&rule_data_str);
	}
	free_vector_str(&rule_lines);
	return rules;
}

bool check_page_ordering(vector_uint_t* page_order, order_rules_t* rules){
	bool page_order_correct = true;
	for(size_t index = 0; index < page_order->length && page_order_correct; ++index){
		uint32_t number = page_order->data[index];
		if(index > 0){
			bool reached_beginning = false;
			for(size_t index_before = index-1;!reached_beginning && page_order_correct; --index_before){
				uint32_t number_before = page_order->data[index_before];
				if(is_in_vector_uint(&rules[number].after,number_before)){
					page_order_correct = false;
				}
				if(index_before == 0){
					reached_beginning = true;
				}
			}
		}
		for(size_t index_after = index+1;index_after < page_order->length && page_order_correct; ++index_after){
			uint32_t number_after = page_order->data[index_after];
			if(is_in_vector_uint(&rules[number].before,number_after)){
				page_order_correct = false;
			}
		}
	}
	return page_order_correct;
}

uint32_t calculate_sum_middle_correct_orders(char* page_order_data, order_rules_t* rules){
	vector_str_t page_orderings = split_string(page_order_data,"\n");
	uint32_t sum = 0;
	for(size_t index = 0; index < page_orderings.length; ++index){
		char* page_order = page_orderings.data[index];
		vector_str_t page_order_str = split_string(page_order,",");
		vector_uint_t page_order_uint = parse_vector_uint(&page_order_str);
		if(check_page_ordering(&page_order_uint, rules)){
			size_t middle = page_order_uint.length / 2;
			sum += page_order_uint.data[middle];
		}
		free_vector_uint(&page_order_uint);
		free_vector_str(&page_order_str);
	}
	free_vector_str(&page_orderings);
	return sum;
}

void correct_page_ordering(vector_uint_t* page_order, order_rules_t* rules){
	bool is_order_correct = false;
	bool found_error = false;
	while(!is_order_correct){
		found_error = false;
		for(size_t index = 0; index < page_order->length && !found_error; ++index){
			uint32_t number = page_order->data[index];
			if(index > 0){
				bool reached_beginning = false;
				for(size_t index_before = index-1;!reached_beginning && !found_error; --index_before){
					uint32_t number_before = page_order->data[index_before];
					if(is_in_vector_uint(&rules[number].after,number_before)){
						found_error = true;
						page_order->data[index] = number_before;
						page_order->data[index_before] = number;
					}
					if(index_before == 0){
						reached_beginning = true;
					}
				}
			}
			for(size_t index_after = index+1;index_after < page_order->length && !found_error; ++index_after){
				uint32_t number_after = page_order->data[index_after];
				if(is_in_vector_uint(&rules[number].before,number_after)){
					found_error = true;
					page_order->data[index] = number_after;
					page_order->data[index_after] = number;
				}
			}
		}
		if(!found_error){
			is_order_correct = true;
		}
	}
}

uint32_t calculate_sum_middle_incorrect_orders(char* page_order_data,order_rules_t* rules){
	vector_str_t page_orderings = split_string(page_order_data,"\n");
	uint32_t sum = 0;
	for(size_t index = 0; index < page_orderings.length; ++index){
		char* page_order = page_orderings.data[index];
		vector_str_t page_order_str = split_string(page_order,",");
		vector_uint_t page_order_uint = parse_vector_uint(&page_order_str);
		if(!check_page_ordering(&page_order_uint, rules)){
			correct_page_ordering(&page_order_uint, rules);
			size_t middle = page_order_uint.length / 2;
			sum += page_order_uint.data[middle];
		}
		free_vector_uint(&page_order_uint);
		free_vector_str(&page_order_str);
	}
	free_vector_str(&page_orderings);
	return sum;
}

