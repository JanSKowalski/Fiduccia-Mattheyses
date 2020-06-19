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


	struct integer_data* a = initialize_integer_data(5);
	struct integer_data* b = initialize_integer_data(7);
	struct integer_data* c = initialize_integer_data(39);

	insert_node(list, 0, a);
	insert_node(list, 1, b);
	insert_node(list, 1, c);

//	insert_node_integer(list, 0, 5);
//	insert_node_integer(list, 1, 7);
//	insert_node_integer(list, 2, 9);

	print_dll(list);

	remove_node_using_list(list, 1);

	print_dll(list);



	garbage_collection(list);

	printf("######################################\n");

	return 0;
}

