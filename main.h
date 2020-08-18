#include <stdio.h>
#include <stdlib.h>

struct condensed{
	struct cell** CELL_array;
	int CELL_array_size;
	struct net** NET_array;
	int NET_array_size;
	struct partition** access_; //0 is partition A, 1 is partition B
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
	//The smallest number of nets between partitions encountered during FM passes
	int lowest_cutstate;
	//The cutstate value during the current pass
	int current_cutstate;
};

void import_data_and_run_algorithm(char *, char *);
void free_all_memory(struct condensed*);

