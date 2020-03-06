#include <stdlib.h>
#include <stdio.h>
#include "dll_structure.h"


/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/


int main(){
	printf("######################################\n");
	printf("Attempting intitialization of dll\n");


	struct dll* list = malloc(sizeof(*list));

	initialize(list);

	printf("Initialization successful\n");


	print_dll(list);

	insert_node(list, 0, 5);
	insert_node(list, 1, 7);
	insert_node(list, 2, 9);

	print_dll(list);

	remove_node(list, 1);

	print_dll(list);



	garbage_collection(list);

	printf("######################################\n");

	return 0;
}


void initialize(struct dll* list){

	struct node* head_sentinel = malloc(sizeof(*head_sentinel));
	struct node* tail_sentinel = malloc(sizeof(*tail_sentinel));

	list->size = 0;
	list->head = head_sentinel;
	list->tail = tail_sentinel;

	connect_two_nodes(head_sentinel, tail_sentinel);
}

void print_dll(struct dll* list){
	struct node* placeholder = list->head->next;
	int nodes_remaining = list->size;
	int index = 0;
	printf("DLL: [");
	while (nodes_remaining > 0){
		if (nodes_remaining == 1)
			printf("%d", placeholder->data);
		else{
			printf("%d, ", placeholder->data);
		}
		nodes_remaining--;
		index++;
		placeholder = placeholder->next;
	}
	printf("]\n");
}

void connect_two_nodes(struct node* first, struct node* second){
	first->next = second;
	second->previous = first;
}


//Position is zero-indexed
//Does NOT handle out-of-bounds yet
void insert_node(struct dll* list, int position, int data){

	//reach correct position
	int index = position;
	struct node* placeholder = list->head;
	while (index > 0){
		placeholder = placeholder->next;
		index--;
	}

	//initialize new node
	struct node* new_node = malloc(sizeof(*new_node));
	new_node->data = data;

	connect_two_nodes(new_node, placeholder->next);
	connect_two_nodes(placeholder, new_node);

	list->size = list->size + 1;
}



//Position is zero indexed, must be a value less than string size
void remove_node(struct dll* list, int position){

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
	free(placeholder);

	list->size = list->size - 1;
}


//Takes in an initialized dll struct
//Frees memory
void garbage_collection(struct dll* list){
	garbage_collection_recursive(list->head, list->tail);
	free(list);
}


//Input head and tail nodes of the ddl
void garbage_collection_recursive(struct node* temp, struct node* tail){
	//printf("Temp, tail refrences: %d, %d\n", temp, tail);
	//printf("Deleting Node data: %d\n", temp->data);
	if (temp != tail){
		garbage_collection_recursive(temp->next, tail);
	}
	free(temp);
}
