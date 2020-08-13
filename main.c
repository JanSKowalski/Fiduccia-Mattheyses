#include "main.h"
#include "data_input.h"
#include "fiduccia_mattheyses.h"

/*
An implementation of the Fiduccia-Mettheyses partitioning algorithm
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

	//Obtain information from the two data files (are, netD)
	struct condensed* information = read_in_data_to_arrays(ARE_FILENAME, NETD_FILENAME);

	//Add useful information about partition sizes
	information->desired_area = (int) (RATIO * information->total_area);
	information->ratio = RATIO;

	//Create and initialize the two partition structs
	initialize_two_partitions(information);
	//Separate the cells into one of the two partitions
	populate_partitions(information);

	fiduccia_mattheyses_algorithm(information);


	free_all_memory(information);
	return 0;
}

void free_all_memory(struct condensed* information){
	int i;
	//Partitions should be deallocated before nets
	delete_partition(information->partition_A);
	delete_partition(information->partition_B);
	//Nets should be deallocated before cells
	for (i = 0; i< information->NET_array_size; i++){
		delete_net(information->NET_array[i]);
	}
	//Cells deallocated last
	for (i = 0; i< information->CELL_array_size; i++){
		delete_cell(information->CELL_array[i]);
	}
	free(information->access_);
	free(information->NET_array);
	free(information->CELL_array);
	free(information);
}

