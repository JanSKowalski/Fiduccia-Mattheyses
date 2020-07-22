#include "main.h"
#include "basic_objects.h"
#include "dll_structure.h"
#include "fiduccia_mattheyses.h"


void calculate_initial_gains(struct condensed* information){

	struct partition* partition_A = information->partition_A;
	struct partition* partition_B = information->partition_B;

	int num_cells_in_partition_A = partition_A->cells_in_partition->size;
	printf("Num cells in partiiton_A: %d\n", num_cells_in_partition_A);

}

