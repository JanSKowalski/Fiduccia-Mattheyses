#include <stdio.h>
#include <stdlib.h>
//#include "dll_structure.h"
//#include "basic_objects.h"
#include "data_input.h"

struct everything_important{
	struct cell** CELL_array;
	struct net** NET_array;
	struct partition* partition_A;
	struct partition* partition_B;
};


void test_doubly_linked_list();
void test_input_functions();
