#include <stdlib.h>
#include <stdio.h>
#include "basic_objects.h"
#include "dll_structure.h"
#include "populate_partitions.h"

//This is a collection of objects that can be stored in the doubly-linked list

//############################################################



//Integer_data is mostly a test data object for debugging
void initialize_integer(struct integer* new_integer, int data){
	new_integer->data = data;
}

void print_integer(struct integer* object){
	printf("%d ", object->data);
}


//############################################################



//The most fundamental aspect of FM is the movement of cells to the correct partition
void initialize_cell(struct cell* new_cell, int identifier, int area){
	new_cell->identifier = identifier;
	new_cell->area = area;

	struct dll* nets = malloc(sizeof(nets));
	initialize_dll(nets);
	new_cell->nets = nets;

	new_cell->partition = 0;
	new_cell->gain = 0;
	new_cell->free_cell = 1;
}

//To find number of pins, call size_dll(cell->nets)
int cell_pinsize(struct cell* cell){
	//Should be an O(1) call
	dll_size(cell->nets);
}

//Adds a net to the the beginning of the cell's netlist
void add_net_to_cell(struct cell* cell, struct net* net){
	insert_node(cell->nets, 0, net);
}


void print_cell(struct cell* cell){
	printf("%d ", cell->identifier);
}

//Consider deleting
void print_cell_area(struct cell* object){
	printf("%d ", object->area);
}



//Garbage collection for cells!
//Need to demalloc nets dll, then cell struct


//This might cause issues, if you deallocate all the nets in this cell, and then another cell tries to access that deallocated net.
//This is a many-to-many
void garbage_collection_cell(struct cell* cell){
	garbage_collection_dll(cell->nets, DEALLOC_DATA);
	free(cell);
}



//############################################################



void initialize_net(struct net* net, int identifier){
	net->identifier = identifier;

	struct dll* free_cells = malloc(sizeof(free_cells));
	initialize_dll(free_cells);
	net->free_cells = free_cells;

	struct dll* locked_cells = malloc(sizeof(locked_cells));
	initialize_dll(locked_cells);
	net->locked_cells = locked_cells;

	//This causes errors, for some reason
	//net->number_cells_in_partition_B = 0;
	//net->number_cells_in_partition_A = 1;

	net->number_of_cells=0;
}

void print_net(struct net* net){
	struct dll* cells = net->free_cells;
	printf("Net %d has %d cell[s]\n", net->identifier, cells->size);
	print_dll(cells, CELL);
}


//Deletes the net and the metadata, but doesn't delete the cells
//Does not remove the references in NET_dll, NET_array -- that should be done manually
//  Avoids freeing memory twice
void delete_net(struct net* undesired_net){
	//Nets have a list of free cells and a list of locked cells, both of which need to be dealt with
	//List of free cells
	struct dll* free_cells = undesired_net->free_cells;
	//The first datanode in free_cells
	struct node* temp_node_for_cell = ((struct node*)free_cells->head)->next;

	delete_net_helper(undesired_net, free_cells, temp_node_for_cell);
	garbage_collection_dll(undesired_net->free_cells, DO_NOT_DEALLOC_DATA);


	struct dll* locked_cells = undesired_net->locked_cells;
	temp_node_for_cell = ((struct node*)locked_cells->head)->next;

	delete_net_helper(undesired_net, locked_cells, temp_node_for_cell);
	garbage_collection_dll(undesired_net->locked_cells, DO_NOT_DEALLOC_DATA);

}


void delete_net_helper(struct net* undesired_net, struct dll* cellist, struct node* temp_node_for_cell){
	//The cell data structure being accessed at each node
	struct cell* temp_cell;
	//The cell's netlist being accessed at each node
	struct dll* temp_cell_netlist;
	//Access the netlist
	struct node* temp_node_for_net;
	//temp_net for checking whether the net is in the netlist to be deleted
	struct net* temp_net;
	//


	//Remove references to the net from every cell's netlist
	while (temp_node_for_cell != cellist->tail){
		temp_cell = temp_node_for_cell->data_structure;
		temp_cell_netlist = temp_cell->nets;
		temp_node_for_net = ((struct node*)temp_cell_netlist->head)->next;
		//print_dll(temp_cell_netlist, NET);
		//Look through each net in the netlist
		while (temp_node_for_net != temp_cell_netlist->tail){
			temp_net = temp_node_for_net->data_structure;
			if (temp_net == undesired_net){
				//Take the node out of the node_list
				remove_node(temp_node_for_net, temp_cell_netlist);
				//Free the node (but not the net)
				free(temp_node_for_net);
				//We've removed the net from the cell's netlist, we can skip the rest of the netlist and move to the next cell that has the undesired_net
				break;
			}
			temp_node_for_net = temp_node_for_net->next;
		}
		//Move to next
		temp_node_for_cell = temp_node_for_cell->next;

		//print_dll(temp_cell_netlist, NET);

	}



}

//############################################################


//Two partitions and a metadata struct get malloc'ed
struct partition_metadata* initialize_two_partitions(){
	struct partition* A = malloc(sizeof(A));
	initialize_partition(A);
	struct partition* B = malloc(sizeof(B));
	initialize_partition(B);

	struct partition_metadata* output = malloc(sizeof(output));
	output->partition_A = A;
	output->partition_B = B;
	return output;
}

void initialize_partition(struct partition* partition){
	//Create main gain dll
	struct dll* cells_sorted_by_gain = malloc(sizeof(cells_sorted_by_gain));
	initialize_dll(cells_sorted_by_gain);
	partition->cells_sorted_by_gain=cells_sorted_by_gain;
	//Create list of cells
	struct dll* cells_in_partition = malloc(sizeof(cells_in_partition));
	initialize_dll(cells_in_partition);
	partition->cells_in_partition = cells_in_partition;
	//Partition starts with 0 cell area
	partition->total_partition_area=0;
	partition->max_gain=0;

}


void populate_partitions(struct partition* partition_A, struct partition* partition_B, struct net** NET_array, int NET_array_size, struct cell** CELL_array, int CELL_array_size, double ratio, int desired_area, int tolerance){
//	struct cell_assignments* determined;
	segregate_cells_randomly(CELL_array, CELL_array_size, partition_A, partition_B, ratio, desired_area, tolerance);
	//determined = divide_cells_largest_cell_first();
	//determined = divide_cells_through_genetic_algorithm();
	//determined = divide_cells_with_neural_network();

	//Copy these cell lists into the appropriate partition, determine cutset
	//copy_cells_into_partitions(NET_array, partition_A, partition_B, determined->list_of_cells_for_A, determined->list_of_cells_for_B);
}
