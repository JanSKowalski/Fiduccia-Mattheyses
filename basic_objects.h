struct condensed;

struct integer{
	int data;
};

typedef enum{
	FREE,
	LOCKED
} cell_state;

struct cell{
	int identifier; //Decided by the order that cells are read in from input .are file
	int gain; //Initially 0
	struct dll* nets;
	struct partition* partition; //The inital partition assigned
	int which_partition; //PARTITION_A or PARTITION_B, partition_type enum
	int area; //Detailed in .are file
	int cell_state; //FREE or LOCKED, cell_state enum
	struct node* GAIN_array_node;
};

struct net{
	int number_of_cells; //free + locked, not necessarily the size of free_cells
	int identifier;
	struct dll* free_cells;
	int* num_cells_in_; // position 0 is partition A, position 1 is partition B
};

struct partition{
	//Points to the node of the cell with the highest gain
	struct node* max_gain_pointer;
	//Array of dlls
	struct dll** GAIN_array;
	int GAIN_array_size;
	struct dll* cells_in_partition;
	int total_partition_area;
};

void initialize_integer(struct integer*, int);
void print_integer(struct integer*);

void initialize_cell(struct cell*, int, int);
//Cells should be deleted after nets by going through CELL_array
void delete_cell(struct cell*);
void print_cell(struct cell*);
void print_cell_area(struct cell*);

void initialize_net(struct net*, int);
//Nets should be deleted first by going through NET_array
void delete_net(struct net*);
void delete_net_helper(struct net*, struct dll*, struct node*);
void print_net(struct net*);

int calculate_max_nets_on_cell(struct cell**, int);
void initialize_two_partitions(struct condensed*);
void initialize_partition(struct partition*, int);
void populate_partitions(struct condensed*);
//void populate_partitions(struct partition*, struct partition*, struct net**, int, struct cell**, int, double, int, int);
void delete_partition(struct partition*);
