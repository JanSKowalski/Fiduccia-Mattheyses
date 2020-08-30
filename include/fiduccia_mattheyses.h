typedef enum{
	INCREMENT,
	DECREMENT
} change_direction;

typedef enum{
	ONE,
	ALL
} cells_to_change;

struct condensed;

void fiduccia_mattheyses_algorithm(struct condensed*);


void check_cutstate_values(struct condensed*);

void calculate_initial_gains_wrapper(struct condensed*);
void calculate_initial_gains(struct partition*, partition_type, int, struct chromosome*);
void update_max_gain_pointer(struct partition*);

void print_gain_arrays(struct partition*);


int FM_pass(struct condensed*);
int change_gain_of_cell_in_net(struct cell*, cells_to_change, change_direction, int, struct dll*, int, struct dll*, struct condensed*);
