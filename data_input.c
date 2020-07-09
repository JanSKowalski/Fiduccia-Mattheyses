#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_input.h"

void read_in_are_file(struct dll* CELL_dll){
	FILE *fp;
	char line[256];
	//Open area data file
	fp = fopen("testdata.are", "r");
	int identifier_index = 0;
	//Loop through area file, check each line (delimited by newline)
	while (fgets(line, sizeof(line), fp)){
		//Options are a (for cell) and p (for pin)
		// We only want to add cells to the cell array
		if (line[0] == 'a'){
			//Create cell, set index
			struct cell* new_cell = malloc(sizeof(*new_cell));
			insert_node(CELL_dll, identifier_index, new_cell);
			//extract area information from line
			char* token = strtok(line, " ");
			token = strtok(NULL, " ");
			//Initialize cell
			initialize_cell(new_cell, identifier_index, atoi(token));
			//Prepare for next cell
			identifier_index += 1;
		}
	}
	fclose(fp);
}

//Assumes read_in_are_file has been called
struct cell** create_CELL_array(struct dll* CELL_dll){
	int number_of_cells = CELL_dll->size;
	struct cell** CELL_array = malloc(sizeof(struct cell*) * number_of_cells);
	struct node* placeholder_node = CELL_dll->head;
	int i;
	for (i = 0; i < number_of_cells; i++){
		placeholder_node = access_next_node(placeholder_node);
		CELL_array[i] = (struct cell*) placeholder_node->data_structure;
	}
	return CELL_array;
}


void read_in_netD_file(struct cell** CELL_array, struct dll* NET_dll){
	FILE *fq;
	char line[256];
	//Open netlist file
	fq = fopen("testdata.netD", "r");
	//Unique identifier for every net
	int net_index = 0;
	//A pointer to the current net, so that cells can be added to it.
	struct net* incubent_net = NULL;
	while(fgets(line, sizeof(line), fq)){
		//Split the line into three tokens (first five lines not used [yet])
		char* first_token = strtok(line, " ");
		char* second_token = NULL;
		second_token = strtok(NULL, " ");

		//If the cells are part of a new list, create a new list for them
		if (second_token != NULL && *second_token == 's'){

			//If incubent_net only has one cell (cell-pin net), delete
			net_index = check_net(incubent_net, NET_dll, net_index);

			//Form new net
			struct net* new_net = malloc(sizeof(new_net));
			initialize_net(new_net, net_index);
			//Add net to NET_array in first position
			insert_node(NET_dll, 0, new_net);
			//Replace old net
			incubent_net = new_net;
			net_index += 1;
		}

		//If the cell is a regular cell (not a pin), add it to the current net, add net to the pin
		if (first_token[0] == 'a'){
			//**Check to see if the "lost" byte is deallocated in valgrind
			//Remove 'a' from cell ident.
			first_token += 1;
			//Transfer string to integer
			int cell_identifier = atoi(first_token);
			//Access cell
			struct cell* accessed_cell = CELL_array[cell_identifier];
			int area = accessed_cell->area;
			//Add cell to the first position in the net's cell list (O(1) operation)
			insert_node(incubent_net->free_cells, 0, accessed_cell);
			//Add net to the first position in the cell's netlist
			insert_node(accessed_cell->nets, 0, incubent_net);
			//Increase the net's cell counter
			incubent_net->number_of_cells += 1;
		}
	}
	//Check whether the last net is of only one cell, delete if it is
	net_index = check_net(incubent_net, NET_dll, net_index);
	fclose(fq);
}

//Returns the correct net_index for the new net after this function is called in read_in_netD_file
int check_net(struct net* incubent_net, struct dll* NET_dll, int net_index){
	if(incubent_net != NULL && incubent_net->number_of_cells < 2){
		//Clean up many-to-many net cell relationships
		delete_net(incubent_net);
		//free memory
//				free(incubent_net);
		//Remove from NET_dll
		remove_node_using_list(NET_dll, 0);
		//replace index
		net_index -= 1;
	}
	return net_index;
}

//Assumes read_in_netD_file has been called
struct net** create_NET_array(struct dll* NET_dll){
	int number_of_nets = NET_dll->size;
	struct net** NET_array = malloc(sizeof(struct net*) * number_of_nets);
	struct node* placeholder_node = NET_dll->head;
	int i;
	for (i = number_of_nets - 1; i >= 0; i--){
		placeholder_node = access_next_node(placeholder_node);
		NET_array[i] = (struct net*) placeholder_node->data_structure;
	}
	return NET_array;
}
