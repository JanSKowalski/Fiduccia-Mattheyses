#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_input.h"
#include "basic_objects.h"

void read_in_are_file(struct dll* CELL_array){
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
			struct cell* new_cell = malloc(sizeof(new_cell));
			insert_node(CELL_array, CELL_array->size, new_cell);
			new_cell->identifier = identifier_index;
			//extract area information from line
			char* token = strtok(line, " ");
			token = strtok(NULL, " ");
			new_cell->area = atoi(token);
			//Prepare for next cell
			identifier_index += 1;
		}
	}
	fclose(fp);
}


void read_in_netD_file(struct dll* CELL_array, struct dll* NET_array){
	FILE *fq;
	char line[256];
	//Open netlist file
	fq = fopen("testdata.netD", "r");
	//Unique identifier for every net
	int net_index = 0;
	//A pointer to the current net, so that cells can be added to it.
	struct net* incubent_net;
	while(fgets(line, sizeof(line), fq)){
		//Split the line into three tokens (first five lines not used [yet])
		char* first_token = strtok(line, " ");
		char* second_token = strtok(NULL, " ");
		//Not necessary
		//third_token = strtok(NULL, " ");

		//If the cells are part of a new list, create a new list for them
		if (*second_token == 's'){
			struct net* new_net = malloc(sizeof(new_net));
			incubent_net = new_net;
			printf("net added\n");
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

			//Add cell to the first position in the net's cell list (O(1) operation)
			insert_node(incubent_net->free_cells, 0, accessed_cell);
			//Add net to the first position in the cell's netlist
			insert_node(accessed_cell->nets, 0, incubent_net);
		}
	}
	fclose(fq);
}
