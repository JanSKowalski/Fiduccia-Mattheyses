#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_input.h"
#include "basic_objects.h"

void read_in_are_file(struct dll* CELL_array){
	FILE *fp;
	char line[256];

	fp = fopen("testdata.are", "r");

	while (fgets(line, sizeof(line), fp)){

		int identifier_index = 0;

		//Options are a (for cell) and p (for pin)
		// We only want to add cells to the cell array
		if (line[0] == 'a'){


			//Create cell, set index
			struct cell* new_cell = malloc(sizeof(new_cell));
			new_cell->identifier = identifier_index;


			char* token = strtok(line, " ");
			token = strtok(NULL, " ");

			new_cell->area = atoi(token);
			identifier_index += 1;

		}
	}

	fclose(fp);
}
