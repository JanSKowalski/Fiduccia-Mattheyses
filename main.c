#include "main.h"

/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/

#define ARE_FILENAME "testdata.are"
#define NETD_FILENAME "testdata.netD"


//Keep a list of nets in cutstate
//Balance is stored in main
int main(){
	cleaner_readin();
	//test_input_functions();
	//test_doubly_linked_list();
	return 0;
}


void cleaner_readin(){
	struct array_metadata* read_in_output;
	read_in_output = read_in_data_to_arrays(ARE_FILENAME, NETD_FILENAME);

	struct cell** CELL_array = read_in_output->CELL_array;
	struct net** NET_array = read_in_output->NET_array;
}


void test_input_functions(){
/*
	printf("######################################\n");
	printf("Creating CELL array\n");
	//CELL dll is used to create CELL_array. The array is more desirable because of O(1) access
	struct dll* CELL_dll = malloc(sizeof(*CELL_dll));
	initialize_dll(CELL_dll);
	read_in_are_file(CELL_dll);


	struct cell** CELL_array = create_CELL_array(CELL_dll);

	printf("Main function\n");

	int i;
	for (i=0; i < CELL_dll->size; i++){
		printf("Cell %d area: %d\n",i, CELL_array[i]->area);
	}

	printf("Cell_dll: ");
	print_dll(CELL_dll, CELL);



	printf("######################################\n");
	printf("Creating NET array\n");
	struct dll* NET_dll = malloc(sizeof(NET_dll));
	initialize_dll(NET_dll);
	//Both cell and net arrays are useful here
	read_in_netD_file(CELL_array, NET_dll);

	print_dll(NET_dll, NET);


	struct net** NET_array = create_NET_array(NET_dll);

	//Print results
	//for (i=0; i < NET_array->size; i++){
	//print_dll(NET_dll, NET);
	//printf("\n");
	//}
*/
/*
	printf("Attempting to delete net 2\n");
	delete_net(NET_array[2]);
	remove_node_using_list(NET_dll, 0);
	printf("Net 2 deleted\n");

	printf("\n");
	print_dll(NET_dll, NET);
*/
	//Free references to Nets in NET_array
	//int size = NET_dll->size;
	//for (i = 0; i < size; i++){
	//	free(NET_array[i]);
	//}

	//printf("Attempt to dealloc net\n");
	//free(NET_array
	//garbage_collection_dll(NET_array, DEALLOC_DATA);
	//If you delete the cells, they can't be accessed. Always put gc at the end, don't be clever.
	//garbage_collection_dll(CELL_dll);

}


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


	garbage_collection_dll(list, DEALLOC_DATA);

	printf("######################################\n");
}

