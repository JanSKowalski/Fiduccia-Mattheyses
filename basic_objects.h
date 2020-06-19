struct integer_data{
	int data;
};

struct cell{
	int gain;
	struct dll* nets;
	int partition;
	int area;
};


struct net{
	struct dll* cells;
};


struct integer_data* initialize_integer_data(int);
struct cell* initialize_cell(int, struct  dll*, int, int);


void print_integer_data(struct integer_data*);
