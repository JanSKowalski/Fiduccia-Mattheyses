#include "main.h"
#include "data_input.h"

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

	struct condensed* information = read_in_data_to_arrays(ARE_FILENAME, NETD_FILENAME);


	information->desired_area = (int) (RATIO * information->total_area);
	information->ratio = RATIO;

	printf("Desired area: %d\n", information->desired_area);

	initialize_two_partitions(information);
//	populate_partitions(partitions->partition_A, partitions->partition_B, information->NET_array, information->NET_array_size, information->CELL_array, information->CELL_array_size, RATIO, desired_area, information->tolerance);


//	calculate_initial_gains();


	free_all_memory(information);
	return 0;
}

void free_all_memory(struct condensed* information){
	int i;
	//Nets should be deallocated before cells
	for (i = 0; i< information->NET_array_size; i++){
		delete_net(information->NET_array[i]);
	}

	for (i = 0; i< information->CELL_array_size; i++){
		delete_cell(information->CELL_array[i]);
	}
	free(information->NET_array);
	free(information->CELL_array);
	free(information);
}

