
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

//	struct partition_metadata* partitions = initialize_two_partitions();
//	populate_partitions(partitions->partition_A, partitions->partition_B, NET_array, input_data->number_of_nets, CELL_array, input_data->number_of_cells, RATIO, desired_area, input_data->tolerance);

	free_all_memory(input_data, CELL_array, NET_array);
	return 0;
}

void free_all_memory(struct array_metadata* input_data, struct cell** CELL_array, struct net** NET_array){
	int number_of_nets = input_data->number_of_nets;
	int number_of_cells = input_data->number_of_cells;
	int i;
	//Nets should be deallocated before cells
	for (i = 0; i< number_of_nets; i++){
		delete_net(NET_array[i]);
	}

	for (i = 0; i< number_of_cells; i++){
		delete_cell(CELL_array[i]);
	}
	free(NET_array);
	free(CELL_array);
	free(input_data);
}

