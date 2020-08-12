#include "populate_partitions.h"

typedef enum{
	INCREMENT,
	DECREMENT
} change_direction;

struct condensed;

void fiduccia_mattheyses_algorithm(struct condensed*);

void calculate_initial_gains_wrapper(struct condensed*);
void calculate_initial_gains(struct partition*, partition_type, int);
void update_max_gain_pointer(struct partition*);

void print_gain_arrays(struct partition*);


int FM_pass(struct condensed*);
//void FM_helper_partition_update(struct partition*, struct node*);
void update_partitions_now_that_base_cell_is_chosen(struct cell*, struct node*, struct condensed*);
int FM_helper_change_gains_of_cells_in_net(int, struct net*, struct dll*, partition_type, change_direction, int, struct condensed*);
int change_gains_of_all_cells_in_net(int, change_direction, struct dll*);
void change_gain_of_one_cell_in_net(change_direction, struct dll*, int);
