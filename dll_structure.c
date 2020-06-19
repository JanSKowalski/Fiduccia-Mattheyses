#include <stdlib.h>
#include <stdio.h>
#include "dll_structure.h"


/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/


/*
6/15/2020 Review
The O(n) implementation of remove() needs to be supplemented with a O(1) remove(),
	taking the cell pointer directly.
*/


/*
Initialize a doubly linked list
	nodes have arbitrary data type
	every list has a size (excluding sentinels)
	dlls are zero-indexed
Inputs: list
	list - a malloc'd pointer to a dll struct
Outputs: <none>
*/
void initialize(struct dll* list){

	struct node* head_sentinel = malloc(sizeof(*head_sentinel));
	struct node* tail_sentinel = malloc(sizeof(*tail_sentinel));

	list->size = 0;
	list->head = head_sentinel;
	list->tail = tail_sentinel;

	connect_two_nodes(head_sentinel, tail_sentinel);
}


//Assumes data pointer is to an integer value
void print_dll(struct dll* list){

	struct node* placeholder = list->head->next;
	int nodes_remaining = list->size;
//	int index = 0;

	printf("DLL: [");
	while (nodes_remaining > 0){
		struct integer_data* access = placeholder->data_structure;
		int integer_data = access->data;
		if (nodes_remaining == 1)
			printf("%d", integer_data);
		else{
			printf("%d, ", integer_data);
		}

		nodes_remaining--;
//		index++;
		placeholder = placeholder->next;
	}

	printf("]\n");
}

/*
Helper function, allows bidirectional travel
Inputs: first, second
	first - the first node
	second - the second node
Outputs: <none>
*/
void connect_two_nodes(struct node* first, struct node* second){
	first->next = second;
	second->previous = first;
}

/*
Mallocs a new node with the desired data object,
	adds it to the correct position in list
	Position is zero-indexed
	Does NOT handle out-of-bounds yet
Inputs: list, position, data_structure
	list - The list that the node will be added to
	position - Where the list will be included
	data_structure - a pointer to the data structure
			making this void* allows a generic data type in node
Outputs: <none>
*/
void insert_node(struct dll* list, int position, void* data_structure){

	//reach correct position
	int index = position;
	struct node* placeholder = list->head;
	while (index > 0){
		placeholder = placeholder->next;
		index--;
	}

	//initialize new node
	struct node* new_node = malloc(sizeof(*new_node));

	//link data_structure to node
	new_node->data_structure = data_structure;

	//include in list
	connect_two_nodes(new_node, placeholder->next);
	connect_two_nodes(placeholder, new_node);

	list->size = list->size + 1;
}

struct integer_data* initialize_integer_data(int data){
	struct integer_data* new_integer = malloc(sizeof(new_integer));
	new_integer->data = data;
	return new_integer;
}


/*
Potential dll data types: integer, cell, net
*/
void insert_node_integer(struct dll* list, int position, int data){
	struct integer_data* new_integer = malloc(sizeof(new_integer));
	new_integer->data = data;
	insert_node(list, position, new_integer);
}



void insert_node_cell(struct dll* list, int position, int gain, struct dll* nets, int partition, int size){
	struct cell* new_cell = malloc(sizeof(new_cell));
	new_cell->gain = gain;
	new_cell->nets = nets;
	new_cell->partition = partition;
	new_cell->size = size;
	insert_node(list, position, new_cell);
}

//Position is zero indexed, must be a value less than string size
//NOTE: This is an O(n) implementation
void remove_node_using_list(struct dll* list, int position){

	if (position >= list->size ){
		fprintf(stderr, "Error: Attempting to remove a nonexistant node.\n");
		return;
	}

	//Find position
	int index = position;
	struct node* placeholder = list->head;
	while (index >= 0){
		placeholder = placeholder->next;
		index--;
	}

	connect_two_nodes(placeholder->previous, placeholder->next);

	//Free data_structure linked with node
	free(placeholder->data_structure);

	//Free the node
	free(placeholder);

	list->size = list->size - 1;
}

//O(1) remove function requires the pointer to the node
struct node* remove_node(struct node* node_being_removed){
	connect_two_nodes(node_being_removed->previous, node_being_removed->next);
	return node_being_removed;
}

//Takes in an initialized dll struct
//Frees memory
void garbage_collection(struct dll* list){

	struct node* head = list->head;

	//Neither head nor tail have data_structures associated
	//It's simpler to treat them separately
	garbage_collection_recursive(head->next, list->tail);

	free(head);
	free(list);
}


//Input first(not head) and tail nodes of the ddl
void garbage_collection_recursive(struct node* temp, struct node* tail){
	if (temp != tail){
		garbage_collection_recursive(temp->next, tail);
		free(temp->data_structure);
	}
	free(temp);
}
