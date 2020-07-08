struct integer{
	int data;
};

struct cell{
	int identifier; //Decided by the order that cells are read in from input .are file
	int gain; //Initially 0
	struct dll* nets;
	int partition; //Partition A:1, Partition B:2, Unassigned:0
	int area; //Detailed in .are file
	int free_cell; //Initially true (1)
};


struct net{
	int number_of_cells;
	int identifier;
	struct dll* free_cells;
	struct dll* locked_cells;
	int num_cells_in_partition_A;
	int num_cells_in_partition_B;
};

struct partition{
	struct node* max_gain;
	struct dll* cells_sorted_by_gain;
	int total_cell_area;
};

void initialize_integer(struct integer*, int);
void print_integer(struct integer*);

void initialize_cell(struct cell*, int, int);
void print_cell(struct cell*);
void print_cell_area(struct cell*);

void initialize_net(struct net*, int);
void delete_net(struct net*);
void print_net(struct net*);

void initialize_partition(struct partition*);
void populate_partitions(struct partition*, struct partition*, struct cell**, struct dll*, struct dll*, double, int);
