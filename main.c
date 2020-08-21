#include "main.h"
#include "data_input.h"
#include "fiduccia_mattheyses.h"

/*
An implementation of the Fiduccia-Mattheyses partitioning algorithm
Jan Kowalski 3/2020-8/2020
*/


int main(){

	//Run the demo if the user option is enabled
	if (RUN_DEMO_WITH_TESTDATA){
		import_data_and_run_algorithm(TEST_ARE_FILENAME, TEST_NETD_FILENAME, NULL);
		return 0;
	}


	//Define format for ibm benchmark files
	char are_filename[22] = "Data/Input/ibm00.are";
	char netD_filename[22] = "Data/Input/ibm00.netD";

	char results[50] = "Data/Results/ibm00_cutstate_vs_passes.csv";

	int i;
	char number[3];
	//Go through each benchmark chip and run FM
	for(i = 1; i <= 1; i++){
		snprintf(number, 10, "%d", i);
		if (i < 10){
			memcpy(are_filename+15, number, 1);
			memcpy(netD_filename+15, number, 1);
			memcpy(results+17, number, 1);
		}
		else{
			memcpy(are_filename+14, number, 2);
			memcpy(netD_filename+14, number, 2);
			memcpy(results+16, number, 2);
		}

		struct stat buffer;
		FILE *file_check1, *file_check2;
		//Check if files are available
		if ((stat(are_filename, &buffer) == 0) && (stat(netD_filename, &buffer) == 0)){
			import_data_and_run_algorithm(are_filename, netD_filename, results);
		}
		else{
			printf("Either %s or %s is inaccessible by the program\n", are_filename, netD_filename);
		}

	}
	return 0;
}


//Main control function for the program
//The ibm_testbench_number is 1-18, 0 for files not in the testbench suite.
void import_data_and_run_algorithm(char* are_filename, char* netD_filename, char* results_filename){


	clock_t begin = clock();

	//Obtain information from the two data files (are, netD)
	struct condensed* information = read_in_data_to_arrays(are_filename, netD_filename);

	//Keep track of which chip we are working on
	information->results_csv = results_filename;

	//Add useful information about partition sizes
	information->desired_area = (int) (RATIO * information->total_area);
	information->ratio = RATIO;

	//Create and initialize the two partition structs
	initialize_two_partitions(information);
	//Separate the cells into one of the two partitions
	populate_partitions(information);

	//Run the algorithm
	fiduccia_mattheyses_algorithm(information);

	printf("Lowest cutstate: %d\n", information->lowest_cutstate);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	FILE *data = fopen("pins.csv", "a");

	fprintf(data, "%d, %f\n", information->total_pin_count, time_spent);
	fclose(data);

	printf("Program execution time: %f\n", time_spent);

	free_all_memory(information);
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

