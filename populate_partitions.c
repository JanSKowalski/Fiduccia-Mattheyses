#include <stdio.h>
#include <stdlib.h>
#include "basic_objects.h"
#include "dll_structure.h"
#include "populate_partitions.h"

//Assumes no particular order for the CELL_array
//This seems to work well when the majority of cells are roughly the same, with large outliers
void segregate_cells_randomly(struct cell** CELL_array, int CELL_array_size, struct partition* partition_A, struct partition* partition_B, double ratio, int desired_area, int tolerance){
	//Cells are divided randomly into two temporary partiton lists
	//	ex. If rand() >0.5, cell(i) goes to A, otherwise goes to B
	// The cell areas are meanwhile added to the two totals
	// At the end, the total areas are compared. If they don't match the desired ratio,
	//	lists are deleted and computer tries again.
	//If desired ratio met, the temporary lists are transcribed into the partition data_structures

	int i;


	struct dll* list_of_cells_A;
	struct dll* list_of_cells_B;

	int total_partition_area_A = 0;
	int total_partition_area_B = 0;

	//Loop until acceptable balance is found
	while(1){
		list_of_cells_A = malloc(sizeof(list_of_cells_A));
		list_of_cells_B = malloc(sizeof(list_of_cells_B));

		//Seed random for different results (adding rand() prevents the seed from being reused in short time intervals)
		srand(time(NULL)+rand());
		//Initialize the dlls
		initialize_dll(list_of_cells_A);
		initialize_dll(list_of_cells_B);

		//Assign every cell to a partition
		for (i = 0; i< CELL_array_size; i++){
			//Decide which partition the cell will go into
			if (rand() % (int)(1.0/ratio) == 0){
				total_partition_area_A += CELL_array[i]->area;
				insert_node(list_of_cells_A, 0, CELL_array[i]);
			}
			else{
				total_partition_area_B += CELL_array[i]->area;
				insert_node(list_of_cells_B, 0, CELL_array[i]);
			}
		}


		printf("Area: %d\n", total_partition_area_A);

		printf("Tolerance: %d\n", tolerance);


		//If the partition is within tolerance, break the loop and save to partition structs
		//Otherwise free dlls and try again.
		if (total_partition_area_A < (desired_area + tolerance) && total_partition_area_A > (desired_area - tolerance)){
			printf("Balance achieved!\n");
			break;
		}

		//Get ready for next loop
		garbage_collection_dll(list_of_cells_A, DO_NOT_DEALLOC_DATA);
		garbage_collection_dll(list_of_cells_B, DO_NOT_DEALLOC_DATA);
		total_partition_area_A = 0;
		total_partition_area_B = 0;

	}


	//printf("Partition A total area: %d\n", total_partition_area_A);
	//printf("Partition B total area: %d\n", total_partition_area_B);

	copy_cells_into_partitions(partition_A, partition_B, list_of_cells_A, list_of_cells_B, total_partition_area_A, total_partition_area_B);
}

//Add cells, return dll of cutstate nets
void copy_cells_into_partitions(struct partition* partition_A, struct partition* partition_B, struct dll* list_of_cells_A, struct dll* list_of_cells_B, int total_partition_area_A, int total_partition_area_B){

	partition_A->cells_in_partition = list_of_cells_A;
	partition_A->total_partition_area = total_partition_area_A;

/*
	//Go through each cell. Each cell has a list of nets. Go through each net. Add 1 to the number_cells_in_partition_X;
	struct node* temp_node;
	temp_node = list_of_cells_A->head;
	temp_node = temp_node->next;
	while (temp_node != list_of_cells_A->tail){
	
	}
*/
	partition_B->cells_in_partition = list_of_cells_B;
	partition_B->total_partition_area = total_partition_area_B;

}




void check_cutstate(){
	//Go through each net in NET_array, check to see if net has at least one cell in each 

	struct dll* cutstate_nets = malloc(sizeof(cutstate_nets));

}
