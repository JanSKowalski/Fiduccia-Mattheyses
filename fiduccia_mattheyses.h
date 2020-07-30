#include "populate_partitions.h"

typedef enum{
	TO,
	FROM
} change_direction;

struct condensed;

void fiduccia_mattheyses_algorithm(struct condensed*);

void calculate_initial_gains(struct condensed*);
void gains_for_partition_helper(struct partition*, partition_type, int);
void update_max_gain_pointer(struct partition*);

void print_gain_arrays(struct partition*);


void FM_pass(struct condensed*);
void FM_helper_partition_update(struct partition*, struct node*);
void FM_helper_check_and_update_cutstate_value(int, struct condensed*);
int FM_helper_change_gains_of_cells_in_net(int, struct net*, struct dll*, partition_type, change_direction);
