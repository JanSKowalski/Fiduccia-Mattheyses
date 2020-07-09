#include "dll_structure.h"
#include "basic_objects.h"

struct array_metadata{
	int number_of_cells;
	int number_of_nets;
};


struct array_metadata* read_in_data_to_arrays(struct cell**, struct net**, char*, char*);

int count_cells_in_are_file(char*);
void read_in_are_file(struct cell**, char*);


//struct cell** create_CELL_array(struct dll*);

int count_nets_in_netD_file(char*);

void read_in_netD_file(struct cell**, struct net**, char*);
int check_net(struct net*, int);
//struct net** create_NET_array(struct dll*);
