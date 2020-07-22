#include "populate_partitions.h"

struct condensed;


void calculate_initial_gains(struct condensed*);
void gains_for_partition_helper(struct partition*, partition_type, int);
