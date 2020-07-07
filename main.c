#include "main.h"

/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/


//Keep a list of nets in cutstate
//Balance is stored in main

int main(){
	test_input_functions();
	//test_doubly_linked_list();
	return 0;
}


void test_input_functions(){
	printf("######################################\n");
	printf("Creating CELL array\n");
	//CELL dll is used to create CELL_array. The array is more desirable because of O(1) access
	struct dll* CELL_dll = malloc(sizeof(*CELL_dll));
	initialize_dll(CELL_dll);
	read_in_are_file(CELL_dll);

	struct cell** CELL_array = create_CELL_array(CELL_dll);

	int i;
	for (i=0; i < CELL_dll->size; i++){
		printf("Cell %d area: %d\n",i, CELL_array[i]->area);
	}

	print_dll(CELL_dll, CELL);

	garbage_collection_dll(CELL_dll);


	printf("######################################\n");
	printf("Creating NET array\n");
	struct dll* NET_array = malloc(sizeof(NET_array));
	initialize_dll(NET_array);
	//Both cell and net arrays are useful here
	read_in_netD_file(CELL_array, NET_array);
	//Print results
	//for (i=0; i < NET_array->size; i++){
		print_dll(NET_array, NET);
		printf("\n");
	//}


	//free(CELL_array);
	garbage_collection_dll(NET_array);
}



//Will not work unless the print_dll function is edited to print
// integer objects (default is cell objects)
void test_doubly_linked_list(){
	printf("######################################\n");
	printf("Attempting intitialization of dll\n");


	struct dll* list = malloc(sizeof(*list));

	initialize_dll(list);

	printf("Initialization successful\n");


	print_dll(list, INTEGER);

	//Memory allocated in main is automatically released, no need for free
	struct integer* a = malloc(sizeof(*a));
	struct integer* b = malloc(sizeof(*b));
	struct integer* c = malloc(sizeof(*c));

	//Assign values
	initialize_integer(a, 5);
	initialize_integer(b, 4);
	initialize_integer(c, 23);


	insert_node(list, 0, a);
	insert_node(list, 1, b);
	insert_node(list, 1, c);

	print_dll(list, INTEGER);

	remove_node_using_list(list, 1);

	//remove_node requires a pointer to the node, so this doesn't work
	//remove_node(b);

	print_dll(list, INTEGER);

	printf("Size: %d\n", dll_size(list));


	garbage_collection_dll(list);

	printf("######################################\n");
}

