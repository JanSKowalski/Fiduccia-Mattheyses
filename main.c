#include <stdlib.h>
#include <stdio.h>
#include "dll_structure.h"
#include "basic_objects.h"

/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/


int main(){
	printf("######################################\n");
	printf("Attempting intitialization of dll\n");


	struct dll* list = malloc(sizeof(*list));

	initialize_dll(list);

	printf("Initialization successful\n");


	print_dll(list);

	//Memory allocated in main is automatically released, no need for free
	struct integer* a = malloc(sizeof(a));
	struct integer* b = malloc(sizeof(b));
	struct integer* c = malloc(sizeof(c));

	//Assign values
	initialize_integer(a, 5);
	initialize_integer(b, 4);
	initialize_integer(c, 23);


	insert_node(list, 0, a);
	insert_node(list, 1, b);
	insert_node(list, 1, c);

	print_dll(list);

	remove_node_using_list(list, 1);

	//remove_node requires a pointer to the node, so this doesn't work
	//remove_node(b);

	print_dll(list);

	printf("Size: %d\n", dll_size(list));


	garbage_collection_dll(list);

	printf("######################################\n");

	return 0;
}

