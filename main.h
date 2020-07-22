#include <stdio.h>
#include <stdlib.h>

struct condensed{
	struct cell** CELL_array;
	int CELL_array_size;
	struct net** NET_array;
	int NET_array_size;
	struct partition* partition_A;
	struct partition* partition_B;
	//The area of the largest cell is used as the balance tolerance
	int tolerance;
	//The sum of all cell areas
	int total_area;
	int desired_area;
	double ratio;
	//The highest number of nets connected to a single cell
	int max_nets;
};

void free_all_memory(struct condensed*);

