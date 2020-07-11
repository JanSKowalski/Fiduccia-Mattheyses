#include "dll_structure.h"
#include "basic_objects.h"

struct array_metadata{
	struct cell** CELL_array;
	int number_of_cells;
	struct net** NET_array;
	int number_of_nets;
	int total_area;
	int tolerance;
};

struct are_metadata{
	int total_area;
	int tolerance;
};


struct array_metadata* read_in_data_to_arrays(char*, char*);

int count_cells_in_are_file(char*);
struct are_metadata* read_in_are_file(struct cell**, char*);

int count_nets_in_netD_file(char*);
void read_in_netD_file(struct cell**, struct net**, char*);
