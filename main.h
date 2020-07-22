#include <stdio.h>
#include <stdlib.h>

struct condensed{
	struct cell** CELL_array;
	int CELL_array_size;
	struct net** NET_array;
	int NET_array_size;
	struct partition* partition_A;
	struct partition* partition_B;
	int tolerance;
	int total_area;
	int desired_area;
	double ratio;
};

void free_all_memory(struct condensed*);

