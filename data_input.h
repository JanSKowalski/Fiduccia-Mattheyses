#include "dll_structure.h"
#include "basic_objects.h"

void read_in_are_file(struct dll*);
struct cell** create_CELL_array(struct dll*);

void read_in_netD_file(struct cell**, struct dll*);
int check_net(struct net*, struct dll*, int);
struct net** create_NET_array(struct dll*);
