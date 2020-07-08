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


void initialize_integer(struct integer*, int);
void initialize_cell(struct cell*, int, int);

void print_cell(struct cell*);
void print_cell_area(struct cell*);

void initialize_net(struct net*, int);
void print_net(struct net*);

void print_integer(struct integer*);
