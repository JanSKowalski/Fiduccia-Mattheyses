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
	struct node* max_gain_pointer;
	struct dll* cells_sorted_by_gain;
	struct dll* cells_in_partition;
	int total_partition_area;
};

struct partition_metadata{
	struct partition* partition_A;
	struct partition* partition_B;
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

struct partition_metadata* initialize_two_partitions();
void initialize_partition(struct partition*);
void populate_partitions(struct partition*, struct partition*, struct net**, int, struct cell**, int, double, int, int);
void delete_partition(struct partition*);
