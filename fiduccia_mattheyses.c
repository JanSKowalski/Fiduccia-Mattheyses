#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "basic_objects.h"
//#include "populate_partitions.h"
#include "dll_structure.h"
#include "fiduccia_mattheyses.h"


void fiduccia_mattheyses_algorithm(struct condensed* information){

	calculate_initial_gains_wrapper(information);

	int cells_can_still_be_moved = 1;

	while( cells_can_still_be_moved ){
		print_gain_arrays(information->access_[PARTITION_A]);
		print_gain_arrays(information->access_[PARTITION_B]);
		cells_can_still_be_moved = FM_pass(information);
	}

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
		printf("All cells locked\n");
		return 0;
	}

	struct cell* cell_A;
	struct cell* cell_B;

	//Make sure null pointers aren't dereferenced
	if (node_A != NULL)
		cell_A = node_A->data_structure;
	if (node_B != NULL)
		cell_B = node_B->data_structure;




	//Chose base_cell
	//Either other base_cell is not a candidate or (cell has the highest gain and does not mess up balance)
	if (node_B == NULL || ((cell_A->gain >= cell_B->gain) && (information->partition_A->total_partition_area - cell_A->area > information->desired_area - information->tolerance))){
		base_cell = cell_A;
		node_to_be_freed = node_A;
	}
	else if (node_A == NULL || (information->partition_B->total_partition_area - cell_B->area > information->total_area - information->desired_area - information->tolerance)){
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
	information->access_[!base_cell_destination]->total_partition_area += base_cell->area;
	//Delete node
	free(remove_node(node_to_be_freed, information->access_[base_cell_origin]->GAIN_array[position]));
	base_cell->GAIN_array_node = NULL;
	base_cell->cell_state = LOCKED;

	//Lock base_cell, no longer free
	//base_cell->cell_state = LOCKED;
	printf("%d chosen\n", base_cell->identifier);

	//Setup for the while loop
	base_cell_netlist = base_cell->nets;
	temp_net_node = base_cell_netlist->head->next;

	int num_cells_in_destination, num_cells_in_origin;

	//For each net in the base cell netlist
	while(temp_net_node != base_cell_netlist->tail){
		//Access the net
		temp_net = temp_net_node->data_structure;




		//if the net was previously not in the cutstate, increment the gains of all cells in net, increase cutsize
		if (temp_net->num_cells_in_[base_cell_destination] == 0){
//			new_cutsize = change_gains_of_all_cells_in_net(new_cutsize, INCREMENT, temp_net->free_cells, information);
			new_cutsize = change_gain_of_cell_in_net(ALL, INCREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}
		//else if there was one cell previously in destination partition, decrement the gain of that cell
		else if(temp_net->num_cells_in_[base_cell_destination] == 1){
//			change_gain_of_one_cell_in_net(DECREMENT, temp_net->free_cells, base_cell_destination, information);
			new_cutsize = change_gain_of_cell_in_net(ONE, DECREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}


		//Update net count now that base_cell has switched partitions
		temp_net->num_cells_in_[base_cell_destination] += 1;
		temp_net->num_cells_in_[base_cell_origin] -= 1;


		//if the origin partition now has no cells, decrement the gains of all cells in net, decrease cutsize
		if (temp_net->num_cells_in_[base_cell_origin] == 0){
//			new_cutsize = change_gains_of_all_cells_in_net(new_cutsize, DECREMENT, temp_net->free_cells, information);
			new_cutsize = change_gain_of_cell_in_net(ALL, DECREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}
		else if(temp_net->num_cells_in_[base_cell_origin] == 1){
//			change_gain_of_one_cell_in_net(INCREMENT, temp_net->free_cells, base_cell_origin, information);
			new_cutsize = change_gain_of_cell_in_net(ONE, INCREMENT, new_cutsize, temp_net->free_cells, base_cell_destination, information);
		}

		temp_net_node = temp_net_node->next;
	}


	//Check and update cutstate value
	printf("Pass cutstate value: %d\n", new_cutsize);
	if (new_cutsize < information->lowest_cutstate){
		information->lowest_cutstate = new_cutsize;
	}
	information->current_cutstate = new_cutsize;


	//Set the max_gain pointers to the new highest cells
	update_max_gain_pointer(information->partition_A);
	update_max_gain_pointer(information->partition_B);

	return 1;
}





//Update partition areas, remove base cell from GAIN_array
void update_partitions_now_that_base_cell_is_chosen(struct cell* base_cell, struct node* node_to_be_freed, struct condensed* information){
	int position;
	partition_type base_cell_origin = base_cell->which_partition;
	//Find the dll the base_cell belongs to (GAIN_array_size is the same for both partitions)
	position = information->access_[base_cell_origin]->GAIN_array_size/2 + base_cell->gain;
	//Update partition areas, delete node

	information->access_[base_cell_origin]->total_partition_area -= base_cell->area;
	information->access_[!base_cell_origin]->total_partition_area += base_cell->area;

printf("Update_partitions\n");
		print_gain_arrays(information->access_[base_cell_origin]);
	free(remove_node(node_to_be_freed, information->access_[base_cell_origin]->GAIN_array[position]));
		print_gain_arrays(information->access_[base_cell_origin]);

	printf("base_cell: %d\n", base_cell->identifier);
	base_cell->GAIN_array_node = NULL;
}


int change_gain_of_cell_in_net(cells_to_change scope, change_direction operation, int old_cutsize, struct dll* cellist, int partition_with_isolated_cell, struct condensed* information){
	struct node* temp_cell_node = cellist->head->next;

	struct cell* temp_cell;
	int position;
	partition_type temp_cell_origin;

	while (temp_cell_node != cellist->tail){
		temp_cell = temp_cell_node->data_structure;
		printf("START temp cell: %d\n", temp_cell->identifier);
		temp_cell_origin = temp_cell->which_partition;

		print_gain_arrays(information->access_[temp_cell_origin]);

		//Remove locked cells from net's free_cell list
		if (temp_cell->cell_state == LOCKED){
			//Move to next node, get rid of bad one
			temp_cell_node = temp_cell_node->next;
			free(remove_node(temp_cell_node->previous, cellist));
			continue;
		}
		printf("After temp cell: %d\n", temp_cell->identifier);

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

		//Move cell to correct position in GAIN_array
		free(remove_node(temp_cell_node, cellist));
		print_gain_arrays(information->access_[temp_cell_origin]);
		position = information->access_[temp_cell_origin]->GAIN_array_size/2 + temp_cell->gain;
		insert_node(information->access_[temp_cell_origin]->GAIN_array[position], 0, temp_cell);
		print_gain_arrays(information->access_[temp_cell_origin]);

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

int change_gains_of_all_cells_in_net(int old_cutsize, change_direction operation, struct dll* cellist, struct condensed* information){
	struct node* temp_cell_node = cellist->head->next;

	struct cell* temp_cell;
	int position;
	partition_type temp_cell_origin;

	while (temp_cell_node != cellist->tail){
		temp_cell = temp_cell_node->data_structure;
		printf("START temp cell: %d\n", temp_cell->identifier);
		temp_cell_origin = temp_cell->which_partition;

		print_gain_arrays(information->access_[temp_cell_origin]);

		//Remove locked cells from net's free_cell list
		if (temp_cell->cell_state == LOCKED){
			//Move to next node, get rid of bad one
			temp_cell_node = temp_cell_node->next;
			free(remove_node(temp_cell_node->previous, cellist));
			continue;
		}

		//Update gain
		if (operation == INCREMENT)
			temp_cell->gain++;
		else
			temp_cell->gain--;


		//Move cell to correct position in GAIN_array
		free(remove_node(temp_cell_node, cellist));
		print_gain_arrays(information->access_[temp_cell_origin]);
		position = information->access_[temp_cell_origin]->GAIN_array_size/2 + temp_cell->gain;
		insert_node(information->access_[temp_cell_origin]->GAIN_array[position], 0, temp_cell);
		print_gain_arrays(information->access_[temp_cell_origin]);

		//Move to next cell
		temp_cell_node = temp_cell_node->next;
	}

	//Update cutsize
	int new_cutsize = old_cutsize;
	if (operation == INCREMENT)
		new_cutsize++;
	else
		new_cutsize--;

	return new_cutsize;
}

void change_gain_of_one_cell_in_net(change_direction operation, struct dll* cellist, int partition_with_isolated_cell, struct condensed* information){
	struct node* temp_cell_node = cellist->head->next;
	struct cell* temp_cell;
	int position;
	partition_type temp_cell_origin;

	while (temp_cell_node != cellist->tail){
		temp_cell = temp_cell_node->data_structure;
		printf("START\n");
		temp_cell_origin = temp_cell->which_partition;
		print_gain_arrays(information->access_[temp_cell_origin]);

		//Remove locked cells from net's free_cell list
		if (temp_cell->cell_state == LOCKED){
			//Move to next node, get rid of bad one
			temp_cell_node = temp_cell_node->next;
			free(remove_node(temp_cell_node->previous, cellist));
			continue;
		}

		//Update gain
		if (temp_cell->which_partition == partition_with_isolated_cell){
			if (operation == INCREMENT)
				temp_cell->gain++;
			else
				temp_cell->gain--;
		}


		//Move cell to correct position in GAIN_array
		free(remove_node(temp_cell_node, cellist));
		print_gain_arrays(information->access_[temp_cell_origin]);
		position = information->access_[temp_cell_origin]->GAIN_array_size/2 + temp_cell->gain;
		insert_node(information->access_[temp_cell_origin]->GAIN_array[position], 0, temp_cell);
		print_gain_arrays(information->access_[temp_cell_origin]);

		//Move to next cell
		temp_cell_node = temp_cell_node->next;
	}

}

