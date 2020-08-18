#include "main.h"
#include "data_input.h"
#include "fiduccia_mattheyses.h"
#include <time.h>
#include <string.h>
#include <sys/stat.h>

/*
An implementation of the Fiduccia-Mettheyses partitioning algorithm
Jan Kowalski 3/2020-8/2020
*/

//Toy files
//#define ARE_FILENAME "Data/Input/testdata.are"
//#define NETD_FILENAME "Data/Input/testdata.netD"
//The first testset
#define ARE_FILENAME "Data/Input/ibm01.are"
#define NETD_FILENAME "Data/Input/ibm01.netD"

//Ratio is a double between 0<r<1
#define RATIO 0.5


//Keep a list of nets in cutstate
//Balance is stored in main
int main(){

	char are_filename[22] = "Data/Input/ibm00.are";
	char netD_filename[22] = "Data/Input/ibm00.netD";

	int i;
	char number[3];
	//Go through each chip and run FM
	for(i = 1; i <= 18; i++){
		snprintf(number, 10, "%d", i);
		if (i < 10){
			memcpy(are_filename+15, number, 1);
			memcpy(netD_filename+15, number, 1);
		}
		else{
			memcpy(are_filename+14, number, 2);
			memcpy(netD_filename+14, number, 2);
		}

		struct stat buffer;
		FILE *file_check1, *file_check2;
		//Check if files are available
		if ((stat(are_filename, &buffer) == 0) && (stat(netD_filename, &buffer) == 0)){
			import_data_and_run_algorithm(are_filename, netD_filename);
		}
		else{
			printf("Either %s or %s is inaccessible by the program\n", are_filename, netD_filename);
		}

	}
	return 0;
}


//Main control function for the program
void import_data_and_run_algorithm(char* are_filename, char* netD_filename){


	clock_t begin = clock();

	//Obtain information from the two data files (are, netD)
//	struct condensed* information = read_in_data_to_arrays(ARE_FILENAME, NETD_FILENAME);
	struct condensed* information = read_in_data_to_arrays(are_filename, netD_filename);


	//Add useful information about partition sizes
	information->desired_area = (int) (RATIO * information->total_area);
	information->ratio = RATIO;

	//Create and initialize the two partition structs
	initialize_two_partitions(information);
	//Separate the cells into one of the two partitions
	populate_partitions(information);

	fiduccia_mattheyses_algorithm(information);

	printf("Lowest cutstate: %d\n", information->lowest_cutstate);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
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

