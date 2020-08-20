#include "dll_structure.h"
#include "basic_objects.h"

struct array_metadata{
	int tolerance;
	int total_area;
	int number_of_cells;
	int number_of_nets;
	struct net** NET_array;
	//For some reason, the struct fails to free unless this dummy_variable is not assigned a value
	//  I believe this is true because it follows the struct net**, although I don't know why
	int dummy_variable;
	struct cell** CELL_array;
};

struct are_metadata{
	int total_area;
	int tolerance;
};


struct condensed* read_in_data_to_arrays(char*, char*);

int count_cells_in_are_file(char*);
struct are_metadata* read_in_are_file(struct cell**, char*);

int count_nets_in_netD_file(char*);
int read_in_netD_file(struct cell**, struct net**, char*);
