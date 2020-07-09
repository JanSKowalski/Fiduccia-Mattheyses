#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_input.h"


//Takes in are and netD filenames
//Only malloc CELL_array and NET_array in main
struct array_metadata* read_in_data_to_arrays(struct cell** CELL_array, struct net** NET_array, char* are_filename, char* netD_filename){

	//Create a record of the array sizes
	struct array_metadata* sizes = malloc(sizeof(sizes));

	//Malloc the CELL_array, using info from count_cells
	int number_of_cells = count_cells_in_are_file(are_filename);
	CELL_array = malloc(sizeof(struct cell*) * number_of_cells);

	//Populate the array with cell structs
	read_in_are_file(CELL_array, are_filename);


	int number_of_nets = count_nets_in_netD_file(netD_filename);
	NET_array = malloc(sizeof(struct net*) * number_of_nets);

	read_in_netD_file(CELL_array, NET_array, netD_filename);

	//Store the metadata information
	sizes->number_of_cells = number_of_cells;
	sizes->number_of_nets = number_of_nets;
	return sizes;

}

//Returns the number of cells (not pins) in the are file
int count_cells_in_are_file(char* are_filename){
	FILE *fp;
	char line[256];
	int counter = 0;
	fp = fopen(are_filename, "r");
	//loop through lines, count along the way
	while (fgets(line, sizeof(line), fp)){
		if (line[0] == 'a'){
			counter++;
		}
	}
	return counter;
}




void read_in_are_file(struct cell** CELL_array, char* are_filename){
	FILE *fp;
	char line[256];
	fp = fopen(are_filename, "r");
	//Each cell struct gets a unique identifier
	int index = 0;
	//loop through lines, malloc cells as they appear in the .are file
	while (fgets(line, sizeof(line), fp)){
		//    a (for cell) / p (for pin)
		if (line[0] == 'a'){
			//extract area information from line
			char* token = strtok(line, " ");
			token = strtok(NULL, " ");
			//Create cell, set index
			struct cell* new_cell = malloc(sizeof(*new_cell));
			//Set the index and area information
			initialize_cell(new_cell, index, atoi(token));
			//Add to CELL_array
			CELL_array[index] = new_cell;

			//insert_node(CELL_dll, identifier_index, new_cell);


			//Prepare for next cell
			index++;
		}
	}
	fclose(fp);
}

//Can be tossed
/*
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
*/



//Returns the number of nets in the netD file
int count_nets_in_netD_file(char* netD_filename){
	FILE *fp;
	char line[256];
	int counter = 0;
	fp = fopen(netD_filename, "r");
	//loop through lines, count along the way
	while (fgets(line, sizeof(line), fp)){
		//Split the line into tokens, look for 's' indicating the start of a net
		strtok(line, " ");
		char* second_token = NULL;
		second_token = strtok(NULL, " ");
		if (second_token != NULL && *second_token == 's'){
			counter++;
		}
	}
	return counter;
}


void read_in_netD_file(struct cell** CELL_array, struct net** NET_array, char* netD_filename){
	FILE *fp;
	char line[256];
	fp = fopen(netD_filename, "r");
	int net_index = 0;
	//A pointer to the current net, so that cells can be added to it.
	struct net* incubent_net = NULL;
	//loop through each line, extract information about the net connections
	while(fgets(line, sizeof(line), fp)){

		char* first_token = strtok(line, " ");
		char* second_token = NULL;
		second_token = strtok(NULL, " ");
		//If the cells are part of a new list, create a new list for them
		if (second_token != NULL && *second_token == 's'){
			//If incubent_net only has one cell (cell-pin net), delete
			net_index = check_net(incubent_net, net_index);
			//Setup new net with info
			struct net* new_net = malloc(sizeof(new_net));
			initialize_net(new_net, net_index);
			NET_array[net_index] = new_net;
			incubent_net = new_net;
			net_index++;
		}

		//If the cell is a regular cell (not a pin), add it to the current net, add net to the pin
		if (first_token[0] == 'a'){
			//**Check to see if the "lost" byte is deallocated in valgrind
			//Remove 'a' from cell ident.
			first_token += 1;
			//Transfer string to integer
			int cell_identifier = atoi(first_token);
			struct cell* accessed_cell = CELL_array[cell_identifier];
			printf("cell num: %d\n", accessed_cell->identifier);


			//Add cell to the first position in the net's cell list (O(1) operation)
			insert_node(incubent_net->free_cells, 0, accessed_cell);
			//Add net to the first position in the cell's netlist
			insert_node(accessed_cell->nets, 0, incubent_net);
			//Increase the net's cell counter
			incubent_net->number_of_cells += 1;
		}
	}
	//Check whether the last net is of only one cell, delete if it is
	net_index = check_net(incubent_net, net_index);
	fclose(fp);
}

//Returns the correct net_index for the new net after this function is called in read_in_netD_file
int check_net(struct net* incubent_net, int net_index){
	if(incubent_net != NULL && incubent_net->number_of_cells < 2){
		//Clean up many-to-many net cell relationships
		delete_net(incubent_net);
		//free memory
//				free(incubent_net);
		//Remove from NET_dll
		//remove_node_using_list(NET_dll, 0);
		//replace index
		net_index--;
	}
	return net_index;
}




//

//Assumes read_in_netD_file has been called
//struct net** create_NET_array(struct dll* NET_dll){
//	int number_of_nets = NET_dll->size;
//	struct net** NET_array = malloc(sizeof(struct net*) * number_of_nets);
//	struct node* placeholder_node = NET_dll->head;
//	int i;
//	for (i = number_of_nets - 1; i >= 0; i--){
//		placeholder_node = access_next_node(placeholder_node);
//		NET_array[i] = (struct net*) placeholder_node->data_structure;
//	}
//	return NET_array;
//}



