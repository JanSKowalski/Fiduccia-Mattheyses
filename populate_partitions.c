#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "basic_objects.h"
#include "dll_structure.h"
#include "populate_partitions.h"

//Assumes no particular order for the CELL_array
//This seems to work well when the majority of cells are roughly the same, with large outliers
void segregate_cells_randomly(struct condensed* information){
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

	struct cell** CELL_array = information->CELL_array;

	//Loop until acceptable balance is found
	while(1){
		list_of_cells_A = malloc(sizeof(struct dll));
		list_of_cells_B = malloc(sizeof(struct dll));

		//Seed random for different results (adding rand() prevents the seed from being reused in short time intervals)
		srand(time(NULL)+rand());
		//Initialize the dlls
		initialize_dll(list_of_cells_A);
		initialize_dll(list_of_cells_B);

		//Assign every cell to a partition
		for (i = 0; i< information->CELL_array_size; i++){
			//Decide which partition the cell will go into
			if (rand() % (int)(1.0/(information->ratio)) == 0){
				total_partition_area_A += CELL_array[i]->area;
				insert_node(list_of_cells_A, 0, CELL_array[i]);
				CELL_array[i]->partition = information->partition_A;
				CELL_array[i]->which_partition = PARTITION_A;
			}
			else{
				total_partition_area_B += CELL_array[i]->area;
				insert_node(list_of_cells_B, 0, CELL_array[i]);
				CELL_array[i]->partition = information->partition_B;
				CELL_array[i]->which_partition = PARTITION_B;
			}
		}


//		printf("Area: %d\n", total_partition_area_A);

//		printf("Tolerance: %d\n", information->tolerance);


		//If the partition is within tolerance, break the loop and save to partition structs
		//Otherwise free dlls and try again.
		if (total_partition_area_A < (information->desired_area + information->tolerance) && total_partition_area_A > (information->desired_area - information->tolerance)){
//			printf("Balance achieved!\n");
			break;
		}

		//Get ready for next loop
		garbage_collection_dll(list_of_cells_A, DO_NOT_DEALLOC_DATA);
		garbage_collection_dll(list_of_cells_B, DO_NOT_DEALLOC_DATA);
		total_partition_area_A = 0;
		total_partition_area_B = 0;

	}


//	printf("Partition A total area: %d\n", total_partition_area_A);
//	printf("Partition B total area: %d\n", total_partition_area_B);

	copy_cells_into_partitions(information->partition_A, information->partition_B, list_of_cells_A, list_of_cells_B, total_partition_area_A, total_partition_area_B);

//These three lines delete data, but they shouldn't. I'm not sure why they do
	garbage_collection_dll(list_of_cells_A, DO_NOT_DEALLOC_DATA);
	garbage_collection_dll(list_of_cells_B, DO_NOT_DEALLOC_DATA);

}

//Add cells, return dll of cutstate nets
void copy_cells_into_partitions(struct partition* partition_A, struct partition* partition_B, struct dll* list_of_cells_A, struct dll* list_of_cells_B, int total_partition_area_A, int total_partition_area_B){

	partition_A->total_partition_area = total_partition_area_A;


	//Go through each cell. Each cell has a list of nets. Go through each net. Add 1 to the number_cells_in_partition_X;
	struct node* temp_node;
	struct cell* temp_cell;

	//Access the first data node
	temp_node = ((struct node*) list_of_cells_A->head)->next;
	//Go through list of cells
	while (temp_node != list_of_cells_A->tail){
		temp_cell = temp_node->data_structure;
		//Add cell to partition
		insert_node(partition_A->cells_in_partition, 0, temp_cell);
		update_net_partition_count(temp_cell, PARTITION_A);
		//Move to next cell
		temp_node = temp_node->next;
	}

	partition_B->total_partition_area = total_partition_area_B;

	//Access the first data node
	temp_node = ((struct node*) list_of_cells_B->head)->next;
	//Go through list of cells
	while (temp_node != list_of_cells_B->tail){
		temp_cell = temp_node->data_structure;
		//Add cell to partition
		insert_node(partition_B->cells_in_partition, 0, temp_cell);
		update_net_partition_count(temp_cell, PARTITION_B);
		//Move to next cell
		temp_node = temp_node->next;
	}

}

//Go through each net and increment the appropriate counter variable
void update_net_partition_count(struct cell* assigned_cell, partition_type partition){
		struct dll* netlist = assigned_cell->nets;
		struct node* temp_net_node = ((struct node*) netlist->head)->next;
		struct net* temp_net;
		while (temp_net_node != netlist->tail){
			temp_net = temp_net_node->data_structure;

			temp_net->num_cells_in_[partition] += 1;
			//Move to next net
			temp_net_node = temp_net_node->next;
		}
}


void calculate_initial_cutstate(struct net** NET_array, int NET_array_size, struct condensed* information){
	//Go through each net in NET_array, check to see if net has at least one cell in each

//	struct dll* cutstate_nets = malloc(sizeof(struct dll));
	struct net* temp_net;
	int cutstate_count = 0;
	int i;
	for( i = 0; i < NET_array_size; i++){
		temp_net = NET_array[i];
		if ((temp_net->num_cells_in_[PARTITION_A] > 0) && (temp_net->num_cells_in_[PARTITION_B] > 0))
			cutstate_count++;
	}
	printf("Initial cutstate value: %d\n", cutstate_count);
	information->current_cutstate = cutstate_count;
	information->lowest_cutstate = cutstate_count;
}


