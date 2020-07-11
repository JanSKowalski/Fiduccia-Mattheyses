#include <stdio.h>
#include <stdlib.h>
#include "basic_objects.h"

//Need the desired ratio (double)
//Need the tolerance for the ratio
//Need the total cell list
//Need the partitions
void populate_partitions_randomly(struct cell** CELL_array, struct partition* A, struct partition* B, double ratio, int tolerance){
	//Cells are divided randomly into two temporary partiton lists
	//	ex. If rand() >0.5, cell(i) goes to A, otherwise goes to B
	// The cell areas are meanwhile added to the two totals
	// At the end, the total areas are compared. If they don't match the desired ratio,
	//	lists are deleted and computer tries again.


	//If desired ratio met, the temporary lists are transcribed into the partition data_structures

	int i = 9;

}
