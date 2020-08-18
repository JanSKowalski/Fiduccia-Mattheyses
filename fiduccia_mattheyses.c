#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "basic_objects.h"
//#include "populate_partitions.h"
#include "dll_structure.h"
#include "fiduccia_mattheyses.h"


void fiduccia_mattheyses_algorithm(struct condensed* information){

	calculate_initial_gains_wrapper(information);

	printf("Initial cutstate: %d\n", information->current_cutstate);

	int cells_can_still_be_moved = 1;
	int timestep = 0;


//	FILE *data = fopen(information->results_csv, "w");
//	fprintf(data, "Timesteps, Cutstate\n");
	while( cells_can_still_be_moved ){

		if (PRINT_PARTITION_STATES || RUN_DEMO_WITH_TESTDATA){
			print_gain_arrays(information->access_[PARTITION_A]);
			print_gain_arrays(information->access_[PARTITION_B]);
		}
//		fprintf(data, "%d, %d\n", timestep, information->current_cutstate);

		cells_can_still_be_moved = FM_pass(information);

		if (DOUBLE_CHECK_CUTSTATE_VALUES)
			check_cutstate_values(information);
		timestep++;
	}
//	fclose(data);

}

//Go through each net in NET_array, check to see if net has at least one cell in each
//Because this goes through the entire netlist at every pass, it should not be used during normal cycles
void check_cutstate_values(struct condensed* information){

	struct net* temp_net;
	int cutstate_count = 0;
	int i;
	for( i = 0; i < information->NET_array_size; i++){
		temp_net = information->NET_array[i];
		if ((temp_net->num_cells_in_[PARTITION_A] > 0) && (temp_net->num_cells_in_[PARTITION_B] > 0))
			cutstate_count++;
	}
	printf("Checked cutstate value: %d\n", cutstate_count);
}



//Prints the identifers of the cells in each list
void print_gain_arrays(struct partition* partition){
	int i;
	printf("*********\n");
	for (i =0; i< partition->GAIN_array_size; i++){
		printf("- ");
		print_dll(partition->GAIN_array[i], CELL);
	}

}

//Calls calculate_initial_gains for both partitions
void calculate_initial_gains_wrapper(struct condensed* information){
	calculate_initial_gains(information->partition_A, PARTITION_A, information->max_nets);
	calculate_initial_gains(information->partition_B, PARTITION_B, information->max_nets);
}

void calculate_initial_gains(struct partition* partition, partition_type label, int max_nets){
	//Access variables
	struct node* temp_cell_node;
	struct node* temp_net_node;
	struct dll* temp_netlist;
	struct cell* temp_cell;
	struct net* temp_net;
	//Placement variables
	int cell_placement;
	struct dll** GAIN_array = partition->GAIN_array;
	struct dll* cell_gain_dll;




	//Access the first node in the partition with cell data
	temp_cell_node = partition->cells_in_partition->head->next;
	//Go through each cell, calculate gain, add to correct place in GAIN_array
	while(temp_cell_node != partition->cells_in_partition->tail){
		temp_cell = (struct cell*) (temp_cell_node->data_structure);
		temp_netlist = temp_cell->nets;
		temp_net_node = ((struct node*)(temp_netlist->head))->next;
		while(temp_net_node != temp_netlist->tail){
			temp_net = temp_net_node->data_structure;
			//Check if the cell is on a critical net, change gain on whether it would be good/bad to move cell
			if (temp_net->num_cells_in_[label] == 1){
				temp_cell->gain += 1;
			}
			if (temp_net->num_cells_in_[!label] == 0){
				temp_cell->gain -= 1;
			}
			temp_net_node = temp_net_node->next;
		}


		//Now that initial gain has been calculated, add to the appropriate dll in GAIN_array
		cell_placement = max_nets + temp_cell->gain;
		cell_gain_dll = GAIN_array[cell_placement];
		temp_cell->GAIN_array_node = insert_node(cell_gain_dll, 0, temp_cell);
		temp_cell_node = temp_cell_node->next;
	}

	//Set max_gain for partition
	update_max_gain_pointer(partition);
}


void update_max_gain_pointer(struct partition* partition){
	//Set max_gain
	int i;
	int no_pointer_change = 1;
	struct dll* temp_gain_list;
	//Go through the highest gains first, check dll sizes until the first nonzero gain list
	for(i = (partition->GAIN_array_size - 1); i >= 0; i--){
		temp_gain_list = partition->GAIN_array[i];
		if (temp_gain_list->size < 0){
			printf("Gain array list LESS THAN zero!\n");
		}
		if (temp_gain_list->size > 0){
			partition->max_gain_pointer = ((struct node*) (temp_gain_list->head))->next;
			no_pointer_change = 0;
			break;
		}
	}
	//If no more cells in the partition, set the pointer to NULL
	if (no_pointer_change){
		partition->max_gain_pointer = NULL;
	}
}

//#####################################################################################################

//When updating the gains, remember to check for nets that are added/removed from cutstate
//Each pass goes through one cell's netlist 4 times, checking for different conditions to update the cell's gains
//This function choses one cell to move to the other partition, making sure that balance is not upset.
//  It then updates the gains of cells on connected nets
int FM_pass(struct condensed* information){

	//Access variables;
	struct dll* base_cell_netlist;
	struct node* temp_net_node;
	struct net* temp_net;
	struct node* temp_cell_node;

	//The cutsize will probably change after this code (hopefully to a lower number)
	int new_cutsize = information->current_cutstate;

	//base_cell is the notation for the cell to be moved between partitions
	struct cell* base_cell;
	struct node* node_to_be_freed;

	//Check both partitions with the max_gain_pointer with the higher cell
	struct node* node_A = information->partition_A->max_gain_pointer;
	struct node* node_B = information->partition_B->max_gain_pointer;

	//If no more cells, the algorithm halts
	if (node_A == NULL && node_B == NULL){
		return 0;
	}

	struct cell* cell_A;
	struct cell* cell_B;

	//Make sure null pointers aren't dereferenced
	if (node_A != NULL){
		cell_A = node_A->data_structure;
	}

	if (node_B != NULL){
		cell_B = node_B->data_structure;
	}

	//Chose base_cell
	//Either other base_cell is not a candidate or (cell has the highest gain and does not mess up balance)
	if ((node_A != NULL) && (information->partition_A->total_partition_area - cell_A->area > information->desired_area - information->tolerance)){
		base_cell = cell_A;
		node_to_be_freed = node_A;
	}
	else if ((node_B != NULL) && (information->partition_B->total_partition_area - cell_B->area > information->total_area - information->desired_area - information->tolerance)){
		base_cell = cell_B;
		node_to_be_freed = node_B;
	}
	else{
		printf("No cells can be chosen\n");
		return 0;
	}

	//Set origin and destination references
	int base_cell_origin, base_cell_destination;

	base_cell_origin = base_cell->which_partition;
	base_cell_destination = !(base_cell_origin);


	//update partitions now that base_cell is chosen
	//Find the dll the base_cell belongs to (GAIN_array_size is the same for both partitions)
	int position = information->access_[base_cell_origin]->GAIN_array_size/2 + base_cell->gain;
	//Update partition areas
	information->access_[base_cell_origin]->total_partition_area -= base_cell->area;
	information->access_[base_cell_destination]->total_partition_area += base_cell->area;


	//Delete node
	free(remove_node(node_to_be_freed, information->access_[base_cell_origin]->GAIN_array[position]));
	base_cell->GAIN_array_node = NULL;
	base_cell->cell_state = LOCKED;

	//Setup for the while loop
	base_cell_netlist = base_cell->nets;
	temp_net_node = base_cell_netlist->head->next;

	int num_cells_in_destination, num_cells_in_origin;

	//For each net in the base cell netlist
	while(temp_net_node != base_cell_netlist->tail){
		//Access the net
		temp_net = temp_net_node->data_structure;

		//Skip nets with only one cell
		if(temp_net->free_cells->size == 1){
			temp_net_node = temp_net_node->next;
			continue;
		}

		//if the net was previously not in the cutstate, increment the gains of all cells in net, increase cutsize
		if (temp_net->num_cells_in_[base_cell_destination] == 0){
			new_cutsize = change_gain_of_cell_in_net(base_cell, ALL, INCREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}
		//else if there was one cell previously in destination partition, decrement the gain of that cell
		else if(temp_net->num_cells_in_[base_cell_destination] == 1){
			new_cutsize = change_gain_of_cell_in_net(base_cell, ONE, DECREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}


		//Update net count now that base_cell has switched partitions
		temp_net->num_cells_in_[base_cell_destination] += 1;
		temp_net->num_cells_in_[base_cell_origin] -= 1;


		//if the origin partition now has no cells, decrement the gains of all cells in net, decrease cutsize
		if (temp_net->num_cells_in_[base_cell_origin] == 0){
			new_cutsize = change_gain_of_cell_in_net(base_cell, ALL, DECREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}
		else if(temp_net->num_cells_in_[base_cell_origin] == 1){
			new_cutsize = change_gain_of_cell_in_net(base_cell, ONE, INCREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}

		temp_net_node = temp_net_node->next;
	}

	//Print current cutstate
	if (PRINT_PASS_CUTSTATE_VALUES || RUN_DEMO_WITH_TESTDATA){
		printf("Pass cutstate value: %d\n", new_cutsize);
	}

	//Check and update cutstate value
	if (new_cutsize < information->lowest_cutstate){
		information->lowest_cutstate = new_cutsize;
	}
	information->current_cutstate = new_cutsize;


	//Set the max_gain pointers to the new highest cells
	update_max_gain_pointer(information->partition_A);
	update_max_gain_pointer(information->partition_B);

	return 1;
}



int change_gain_of_cell_in_net(struct cell* base_cell, cells_to_change scope, change_direction operation, int old_cutsize, struct dll* cellist, int partition_with_isolated_cell, struct condensed* information){
	struct node* temp_cell_node = cellist->head->next;

	struct cell* temp_cell;
	int position, old_gain;
	partition_type temp_cell_origin;

	//Go through every cell in the net's cellist
	while (temp_cell_node != cellist->tail){
		//Access the cell
		temp_cell = temp_cell_node->data_structure;
		temp_cell_origin = temp_cell->which_partition;

		//Remove locked cells from net's free_cell list
		if (temp_cell == base_cell || temp_cell->cell_state == LOCKED){
			//Move to next node, get rid of bad one
			temp_cell_node = temp_cell_node->next;
			free(remove_node(temp_cell_node->previous, cellist));
			continue;
		}

		old_gain = temp_cell->gain;

		if(scope == ONE){
			//Update gain
			if (temp_cell->which_partition == partition_with_isolated_cell){
				if (operation == INCREMENT)
					temp_cell->gain++;
				else
					temp_cell->gain--;
			}
		}
		else if(scope == ALL){
			//Update gain
			if (operation == INCREMENT)
				temp_cell->gain++;
			else
				temp_cell->gain--;
		}

		position = information->access_[temp_cell_origin]->GAIN_array_size/2 + old_gain;
		//Remove temp_cell from old position in GAIN_array
		free(remove_node(temp_cell->GAIN_array_node, information->access_[temp_cell_origin]->GAIN_array[position]));
		//Find the new position temp_cell should be in based on it's gain
		position = information->access_[temp_cell_origin]->GAIN_array_size/2 + temp_cell->gain;
		//Insert the cell and store the node information
		temp_cell->GAIN_array_node = insert_node(information->access_[temp_cell_origin]->GAIN_array[position], 0, temp_cell);

		//Move to next cell
		temp_cell_node = temp_cell_node->next;
	}

	int new_cutsize = old_cutsize;
	//Update cutsize
	if(scope == ALL){
		if (operation == INCREMENT)
			new_cutsize++;
		else
			new_cutsize--;
	}
	return new_cutsize;
}
