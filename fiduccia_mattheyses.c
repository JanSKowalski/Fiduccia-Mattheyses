#include "main.h"
#include "basic_objects.h"
//#include "populate_partitions.h"
#include "dll_structure.h"
#include "fiduccia_mattheyses.h"

void calculate_initial_gains(struct condensed* information){
	printf("Gains for partition A\n");
	gains_for_partition_helper(information->partition_A, PARTITION_A, information->max_nets);
	printf("Gains for partition B\n");
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
	printf("Num cells in  partition: %d\n", ((struct dll*) partition->cells_in_partition)->size);
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
		printf("Cell placement: %d\n", cell_placement);
		cell_gain_dll = GAIN_array[cell_placement];
		//insert_node(cell_gain_dll, 0, temp_cell);


		printf("Cell identifier: %d, Gain: %d\n", temp_cell->identifier, temp_cell->gain);
		temp_cell_node = temp_cell_node->next;
	}
}

