#include "include/main.h"

/*
An implementation of the Fiduccia-Mattheyses partitioning algorithm
Jan Kowalski 3/2020-8/2020
*/


int main(){

	//Run the demo if the user option is enabled
	if (RUN_DEMO_WITH_TESTDATA){
		import_data_and_run_algorithm(TEST_ARE_FILENAME, TEST_NETD_FILENAME, NULL, 7.0, 9);
		return 0;
	}


	//Define format for ibm benchmark files
	char are_filename[22] = "data/ibm00.are";
	char netD_filename[22] = "data/ibm00.netD";

	char results[50] = "results.csv";

	char number[3];
	snprintf(number, 10, "%d", IBM_FILE_NUMBER);
	if (IBM_FILE_NUMBER < 10){
		memcpy(are_filename+9, number, 1);
		memcpy(netD_filename+9, number, 1);
	}
	else{
		memcpy(are_filename+8, number, 2);
		memcpy(netD_filename+8, number, 2);
	}

	struct stat buffer;
	FILE *file_check1, *file_check2;
	//Check if files are available
	if ((stat(are_filename, &buffer) == 0) && (stat(netD_filename, &buffer) == 0)){
		double i = 0.0;
		int j,k;
//		for(j=0;j<5;j++){
		for(i=0.0;i<1.0;i+=0.05){
		for(k=0;k<30;k+=2){

			import_data_and_run_algorithm(are_filename, netD_filename, results, i, k);
			printf("%.2f done, cutoff %d\n", i, k);
		}
		}
//		}
	}
	else{
		printf("Either %s or %s is inaccessible by the program\n", are_filename, netD_filename);
	}

	return 0;
}


//Main control function for the program
//The ibm_testbench_number is 1-18, 0 for files not in the testbench suite.
void import_data_and_run_algorithm(char* are_filename, char* netD_filename, char* results_filename, double freq, int cutoff){
	clock_t begin = clock();

	//Obtain information from the two data files (are, netD)
	struct condensed* information = read_in_data_to_arrays(are_filename, netD_filename);

	//Add useful information about partition sizes
	information->desired_area = (int) (RATIO * information->total_area);
	information->ratio = RATIO;
	information->results_filename = results_filename;

	//Set the FM_chromosome to NULL so that GA proceeds normally
	information->FM_chromosome = NULL;

information->mutation_frequency = freq;
information->genetic_cutoff = cutoff;

	FILE *data = fopen(results_filename, "a");

	int i;
	for(i = 0; i < FM_NUM_PASSES; i++){
//		printf("\nPass number: %d\n", i);

		if (i > 0 ){
			reset_cells_and_nets(information);
			delete_partition(information->partition_A);
			delete_partition(information->partition_B);
		}

		//Create and initialize the two partition structs
		initialize_two_partitions(information);

		//Separate the cells into one of the two partitions
		populate_partitions(information);

		//Important to create FM_chromosome after populate_partitions, as it checks for NULL
		free(information->FM_chromosome);
		information->FM_chromosome = malloc(sizeof(struct chromosome));
		int test = initialize_chromosome(information->FM_chromosome, information);
fprintf(data, "%.2f, %d, %d\n", information->mutation_frequency, information->genetic_cutoff, information->lowest_cutstate);

		//Run the algorithm
//		fiduccia_mattheyses_algorithm(information);
		//printf("Lowest cutstate: %d\n", information->lowest_cutstate);

	}


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

fclose(data);

//	printf("Program execution time: %f\n", time_spent);
	free_all_memory(information);
}

void reset_cells_and_nets(struct condensed* information){
	int i;
	struct cell* temp_cell;
	struct net* temp_net;
	struct node* locked_start;
	struct node* locked_end;
	struct node* free_end;

	for(i=0;i<information->CELL_array_size;i++){
		temp_cell = information->CELL_array[i];
		temp_cell->cell_state = FREE;
		temp_cell->gain = 0;
		temp_cell->GAIN_array_node = NULL;
	}


	for(i=0;i<information->NET_array_size;i++){
		//O(1) transfer of all locked cells to each free_cells list
		temp_net = information->NET_array[i];
		locked_start = temp_net->locked_cells->head->next;
		free_end = temp_net->free_cells->tail->previous;
		connect_two_nodes(free_end, locked_start);
		locked_end = temp_net->locked_cells->tail->previous;
		connect_two_nodes(locked_end, temp_net->free_cells->tail);
		connect_two_nodes(temp_net->locked_cells->head, temp_net->locked_cells->tail);

		temp_net->num_cells_in_[PARTITION_A] = 0;
		temp_net->num_cells_in_[PARTITION_B] = 0;
	}

	information->lowest_cutstate = information->current_cutstate;
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
	delete_chromosome(information->FM_chromosome);
	free(information);
}

