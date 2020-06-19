#include <stdlib.h>
#include <stdio.h>
#include "basic_objects.h"

//This is a collection of objects that can be stored in the doubly-linked list

struct integer_data* initialize_integer_data(int data){
	struct integer_data* new_integer = malloc(sizeof(new_integer));
	new_integer->data = data;
	return new_integer;
}

void print_integer_data(struct integer_data* object){
	printf("%d ", object->data);
}

struct cell* initialize_cell(int gain, struct dll* nets, int partition, int area){
	struct cell* new_cell = malloc(sizeof(new_cell));
	new_cell->gain = gain;
	new_cell->nets = nets;
	new_cell->partition = partition;
	new_cell->area = area;
	return new_cell;
}


