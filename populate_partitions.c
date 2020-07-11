#include <stdio.h>
#include <stdlib.h>
#include "basic_objects.h"
#include "dll_structure.h"

//Need the desired ratio (double)
//Need the tolerance for the ratio
//Need the total cell list
//Need the partitions
void populate_partitions_randomly(struct cell** CELL_array, int CELL_array_size, struct partition* A, struct partition* B, double desired_ratio, int tolerance){
	//Cells are divided randomly into two temporary partiton lists
	//	ex. If rand() >0.5, cell(i) goes to A, otherwise goes to B
	// The cell areas are meanwhile added to the two totals
	// At the end, the total areas are compared. If they don't match the desired ratio,
	//	lists are deleted and computer tries again.
	//If desired ratio met, the temporary lists are transcribed into the partition data_structures

	int i;


	struct dll* list_of_cells_A = malloc(sizeof(list_of_cells_A));
	struct dll* list_of_cells_B = malloc(sizeof(list_of_cells_B));

	int total_partition_area_A = 0;
	int total_partition_area_B = 0;
	//while(
	//Seed random for different results
	srand(time(NULL));
	//Initialize the dlls
	initialize_dll(list_of_cells_A);
	initialize_dll(list_of_cells_B);

	//Assign every cell to a partition
	for (i = 0; i< CELL_array_size; i++){
		//Decide which partition the cell will go into
		if (rand() % 2){
			total_partition_area_A += CELL_array[i]->area;
			insert_node(list_of_cells_A, 0, CELL_array[i]);
		}
		else{
			total_partition_area_B += CELL_array[i]->area;
			insert_node(list_of_cells_B, 0, CELL_array[i]);
		}
	}


	printf("Partition A total area: %d\n", total_partition_area_A);
	printf("Partition B total area: %d\n", total_partition_area_B);





}
