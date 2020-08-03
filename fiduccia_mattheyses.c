#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "basic_objects.h"
//#include "populate_partitions.h"
#include "dll_structure.h"
#include "fiduccia_mattheyses.h"


void fiduccia_mattheyses_algorithm(struct condensed* information){

	calculate_initial_gains(information);

	print_gain_arrays(information->partition_A);
	print_gain_arrays(information->partition_B);

	int i;
	for (i=0;i<4;i++){
		FM_pass(information);
		print_gain_arrays(information->partition_A);
		print_gain_arrays(information->partition_B);

	}

}


void print_gain_arrays(struct partition* partition){

	int i;
	printf("*********\n");
	for (i =0; i< partition->GAIN_array_size; i++){
		printf("- ");
		print_dll(partition->GAIN_array[i], CELL);
	}

}



void calculate_initial_gains(struct condensed* information){
	gains_for_partition_helper(information->partition_A, PARTITION_A, information->max_nets);
	gains_for_partition_helper(information->partition_B, PARTITION_B, information->max_nets);
}

void gains_for_partition_helper(struct partition* partition, partition_type label, int max_nets){
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




	//Access first node with cell data
	temp_cell_node = partition->cells_in_partition->head->next;
	//Go through each cell, calculate gain, add to correct place in GAIN_array
	while(temp_cell_node != partition->cells_in_partition->tail){
		temp_cell = (struct cell*) (temp_cell_node->data_structure);
		temp_netlist = temp_cell->nets;
		temp_net_node = ((struct node*)(temp_netlist->head))->next;
		while(temp_net_node != temp_netlist->tail){
			temp_net = temp_net_node->data_structure;
			if (label == PARTITION_A){
				if (temp_net->num_cells_in_partition_A == 1){
					temp_cell->gain += 1;
				}
				if (temp_net->num_cells_in_partition_B == 0){
					temp_cell->gain -= 1;
				}
			}
			else{
				if (temp_net->num_cells_in_partition_B == 1){
					temp_cell->gain += 1;
				}
				if (temp_net->num_cells_in_partition_A == 0){
					temp_cell->gain -= 1;
				}
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
		if (temp_gain_list->size != 0){
			partition->max_gain_pointer = ((struct node*) (temp_gain_list->head))->next;
			no_pointer_change = 0;
			break;
		}
	}
	//If no more cells in the partition, set the pointer to NULL
	if (no_pointer_change){
		printf("Assigned max_pointer to NULL\n");
		partition->max_gain_pointer = NULL;
	}
}


//When updating the gains, remember to check for nets that are added/removed from cutstate
//Each pass goes through one cell's netlist 4 times, checking for different conditions to update the cell's gains
void FM_pass(struct condensed* information){

	int new_cutsize = information->current_cutstate;

	//The cell moved between partitions
	struct cell* base_cell;
	int to_partition;
	int from_partition;

	//Check both partitions with the max_gain_pointer with the higher cell
	struct node* node_A = information->partition_A->max_gain_pointer;
	struct node* node_B = information->partition_B->max_gain_pointer;

	//If no more cells, the algorithm is complete
	if (node_A == NULL && node_B == NULL){
		printf("All cells locked\n");
		return;
	}

	struct cell* cell_A;
	struct cell* cell_B;
	printf("middle\n");
	//Make sure null pointers aren't dereferenced
	if (node_A != NULL)
		cell_A = node_A->data_structure;
	if (node_B != NULL)
		cell_B = node_B->data_structure;

	printf("północ\n");

	//Add tolerance to the decision process, so that the partitions remain balanced.

	//Decide which cell to make base_cell
	if (node_B == NULL || (node_A != NULL && cell_A->gain >= cell_B->gain)){



		if (node_A == NULL){
			printf("BLOODY MURDER, NULL NODE A\n");
		}


		base_cell = cell_A;
		from_partition = PARTITION_A;
		to_partition = PARTITION_B;
		//Update partition gain list, and size
		FM_helper_partition_update(information->partition_A, node_A);


	}
	//if node_A is NULL
	else {


		if (node_B == NULL){
			printf("BLOODY MURDER, NULL NODE B\n");
		}

		base_cell = cell_B;
		from_partition = PARTITION_B;
		to_partition = PARTITION_A;
		//Update partition gain list, and size
		FM_helper_partition_update(information->partition_B, node_B);
	}
	//else {
	//	printf("You need to figure something out\n");
	//	return 1;
	//}


	struct dll* base_cell_netlist = base_cell->nets;
	struct node* temp_net_node = base_cell_netlist->head->next;
	struct net* temp_net;
	struct node* temp_cell_node;
	//For each net in the base cell netlist
	while(temp_net_node != base_cell_netlist->tail){
		//Access the net
		temp_net = temp_net_node->data_structure;
		//Update gains of the cells in the net
		//Only cells on critical nets are updated
		new_cutsize = FM_helper_change_gains_of_cells_in_net(new_cutsize, temp_net, temp_net->free_cells, to_partition, TO, information->max_nets, information);

		//Update net count
		if (from_partition == PARTITION_A){
			temp_net->num_cells_in_partition_A -= 1;
			temp_net->num_cells_in_partition_B += 1;
		} else{
			temp_net->num_cells_in_partition_A += 1;
			temp_net->num_cells_in_partition_B -= 1;
		}
		//Remove cell from net free cells list
		//Finding and removing takes a long time O(n) for each net. Maybe not necessary?
		//find_node_in_cell(

		new_cutsize = FM_helper_change_gains_of_cells_in_net(new_cutsize, temp_net, temp_net->free_cells, from_partition, FROM, information->max_nets, information);

		temp_net_node = temp_net_node->next;
	}


	FM_helper_check_and_update_cutstate_value(new_cutsize, information);
	//Set the max_gain pointers to the new highest cells
	update_max_gain_pointer(information->partition_A);
	update_max_gain_pointer(information->partition_B);
}

void FM_helper_partition_update(struct partition* chosen_partition, struct node* base_cell_node){

	printf("FM helper 1\n");
	struct cell* base_cell = base_cell_node->data_structure;
	printf("Base cell: %d\n", base_cell->identifier);
	int position = chosen_partition->GAIN_array_size/2 + ((struct cell*) base_cell_node->data_structure)->gain;
	printf("Gain: %d, Position: %d\n", ((struct cell*) base_cell_node->data_structure)->gain, position);
	remove_node(base_cell_node, chosen_partition->GAIN_array[position]);

	printf("FM helper 2\n");

}

//Go through all the cells in the net's cell list
//Return modifies cutsize
int FM_helper_change_gains_of_cells_in_net(int old_cutsize, struct net* net, struct dll* cell_list, partition_type base_cell_position, change_direction option, int max_nets, struct condensed* information){
	struct node* temp_node = cell_list->head->next;
	struct cell* temp_cell;


	int position_in_GAIN_array;
	int cell_placement;
	struct dll* cell_gain_dll;

	int cutsize = old_cutsize;

	if (base_cell_position == PARTITION_A){
		//Tf the net was previously not in the cutstate, change the gains
		if (net->num_cells_in_partition_B == 0){
			while(temp_node != cell_list->tail){
				//Access cell
				temp_cell = temp_node->data_structure;

		//NEW		//Remove node from GAIN_array
				position_in_GAIN_array = temp_cell->gain + max_nets;
				free(remove_node(temp_cell->GAIN_array_node, temp_cell->partition->GAIN_array[position_in_GAIN_array]));



				//Modify gain
				if (option == TO)
					temp_cell->gain += 1;
				else
					temp_cell->gain -= 1;

		//NEW		//Reposition cell in GAIN_array				
				cell_placement = max_nets + temp_cell->gain;
				cell_gain_dll = temp_cell->partition->GAIN_array[cell_placement];
				temp_cell->GAIN_array_node = insert_node(cell_gain_dll, 0, temp_cell);


				//Move to next cell
				temp_node = temp_node->next;
			}
			//Update cutsize
			if (option ==TO)
				cutsize++;
			else
				cutsize--;
		}
		//If the net was previously critical, find the one alternate cell and decrease its importance
		if (net->num_cells_in_partition_B == 1){
			while(temp_node != cell_list->tail){
				temp_cell = temp_node->data_structure;
				if (temp_cell->partition == information->partition_B)
					if (option == TO)
						temp_cell->gain -= 1;
					else
						temp_cell->gain += 1;
				temp_node = temp_node->next;
			}
		}
	}
	else{
		//Tf the net was previously not in the cutstate, change the gains
		if (net->num_cells_in_partition_A == 0){
			while(temp_node != cell_list->tail){
				//Access cell
				temp_cell = temp_node->data_structure;



		//NEW		//Remove node from GAIN_array
				position_in_GAIN_array = temp_cell->gain + max_nets;
				free(remove_node(temp_cell->GAIN_array_node, temp_cell->partition->GAIN_array[position_in_GAIN_array]));





				if (option == TO)
					temp_cell->gain += 1;
				else
					temp_cell->gain -= 1;

				//Reposition cell in GAIN_array
		//NEW		//Reposition cell in GAIN_array				
				cell_placement = max_nets + temp_cell->gain;
				cell_gain_dll = temp_cell->partition->GAIN_array[cell_placement];
				temp_cell->GAIN_array_node = insert_node(cell_gain_dll, 0, temp_cell);



				//Move to next cell
				temp_node = temp_node->next;
			}
			//Update cutsize;
			if (option == TO)
				cutsize++;
			else
				cutsize--;
		}
		//If the net was previously critical, find the one alternate cell and decrease its importance
		if (net->num_cells_in_partition_A == 1){
			while(temp_node != cell_list->tail){
				temp_cell = temp_node->data_structure;
				if (temp_cell->partition == information->partition_A)
					if (option == TO)
						temp_cell->gain -= 1;
					else
						temp_cell->gain += 1;
				temp_node = temp_node->next;
			}
		}
	}
	return cutsize;
}

//To be called after every FM pass
void FM_helper_check_and_update_cutstate_value(int new_pass_value, struct condensed* information){
	printf("Pass cutstate value: %d\n", new_pass_value);
	if (new_pass_value < information->lowest_cutstate){
		information->lowest_cutstate = new_pass_value;
	}
}

