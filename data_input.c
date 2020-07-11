#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_input.h"


//Takes in are and netD filenames
//Only malloc CELL_array and NET_array in main
struct array_metadata* read_in_data_to_arrays(char* are_filename, char* netD_filename){

	//Create a record of the array sizes
	struct array_metadata* read_in_output = malloc(sizeof(read_in_output));

	//Malloc the CELL_array, using info from count_cells
	int number_of_cells = count_cells_in_are_file(are_filename);
	struct cell** CELL_array = malloc(sizeof(struct cell*) * number_of_cells);

	struct are_metadata* are_output = read_in_are_file(CELL_array, are_filename);
	int tolerance = are_output->tolerance;
	int total_area = are_output->total_area;

	//Populate the array with cell structs
	int number_of_nets = count_nets_in_netD_file(netD_filename);
	struct net** NET_array = malloc(sizeof(struct net*) * number_of_nets);

	read_in_netD_file(CELL_array, NET_array, netD_filename);

	//Store the metadata information
	read_in_output->number_of_cells = number_of_cells;
	read_in_output->number_of_nets = number_of_nets;
	read_in_output->CELL_array = CELL_array;
	read_in_output->NET_array = NET_array;
	read_in_output->tolerance = tolerance;
	read_in_output->total_area = total_area;
	return read_in_output;

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



//are_metadata mallec'ed
struct are_metadata* read_in_are_file(struct cell** CELL_array, char* are_filename){
	FILE *fp;
	char line[256];
	fp = fopen(are_filename, "r");
	struct are_metadata* output = malloc(sizeof(output));
	//Each cell struct gets a unique identifier
	int index = 0;
	//Keep track of the largest cell, as it will be the tolerance for partitioning
	int total_area = 0;
	int largest_cell_area = 0;
	//loop through lines, malloc cells as they appear in the .are file
	while (fgets(line, sizeof(line), fp)){
		//    a (for cell) / p (for pin)
		if (line[0] == 'a'){
			//extract area information from line
			char* token = strtok(line, " ");
			token = strtok(NULL, " ");
			//Create cell, set index
			struct cell* new_cell = malloc(sizeof(*new_cell));
			int cell_area = atoi(token);
			//Add cell area to total_area
			total_area += cell_area;
			//Set the index and area information
			initialize_cell(new_cell, index, cell_area);
			//Check for largest cell
			if (cell_area > largest_cell_area){
				largest_cell_area = cell_area;
			}
			//Add to CELL_array
			CELL_array[index] = new_cell;
			//Prepare for next cell
			index++;
		}
	}
	fclose(fp);
	output->tolerance = largest_cell_area;
	output->total_area = total_area;
	return output;
}


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
			//Add cell to the first position in the net's cell list (O(1) operation)
			insert_node(incubent_net->free_cells, 0, accessed_cell);
			//Add net to the first position in the cell's netlist
			insert_node(accessed_cell->nets, 0, incubent_net);
			//Increase the net's cell counter
			incubent_net->number_of_cells += 1;
		}
	}
	fclose(fp);
}
