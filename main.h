#include <stdio.h>
#include <stdlib.h>
#include "data_input.h"

struct everything_important{
	struct cell** CELL_array;
	struct net** NET_array;
	struct partition* partition_A;
	struct partition* partition_B;
};

void free_all_memory(struct array_metadata*, struct cell**, struct net**);

