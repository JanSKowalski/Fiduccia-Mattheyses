#include "main.h"

/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/

//Toy files
//#define ARE_FILENAME "testdata.are"
//#define NETD_FILENAME "testdata.netD"
//The first testset
#define ARE_FILENAME "ibm01.are"
#define NETD_FILENAME "ibm01.netD"

//Ratio is a double between 0<r<1
#define RATIO 0.5

//Keep a list of nets in cutstate
//Balance is stored in main
int main(){


	struct array_metadata* input_data = read_in_data_to_arrays(ARE_FILENAME, NETD_FILENAME);
	struct cell** CELL_array = input_data->CELL_array;
	struct net** NET_array = input_data->NET_array;


	int desired_area = (int) (RATIO * input_data->total_area);
	printf("Desired area: %d\n", desired_area);

	struct partition_metadata* partitions = initialize_two_partitions();
	populate_partitions(partitions->partition_A, partitions->partition_B, NET_array, input_data->number_of_nets, CELL_array, input_data->number_of_cells, RATIO, desired_area, input_data->tolerance);

	//test_input_functions();
	//test_doubly_linked_list();
	return 0;
}


void test_input_functions(){


	struct array_metadata* input_data = read_in_data_to_arrays(ARE_FILENAME, NETD_FILENAME);
	struct cell** CELL_array = input_data->CELL_array;
	struct net** NET_array = input_data->NET_array;



	//Create print_NET_array() and print_CELL_array() functions;

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

