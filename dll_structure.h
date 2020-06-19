struct dll{
	int size;
	struct node* head;
	struct node* tail;
};

struct node{
	void* data_structure;
	struct node* previous;
	struct node* next;
};


//Data types that can be used in nodes
struct integer_data{
	int data;
};

struct cell{
	int gain;
	struct dll* nets;
	int partition;
	int size;
};

void initialize(struct dll*);
void connect_two_nodes(struct node*, struct node*);


struct integer_data* initialize_integer_data(int);


void insert_node(struct dll*, int, void*);
void insert_node_integer(struct dll*, int, int);
void insert_node_cell(struct dll*, int, int, struct dll*, int, int);



void remove_node_using_list(struct dll*, int);
struct node* remove_node(struct node*);

void garbage_collection(struct dll*);
void garbage_collection_recursive(struct node*, struct node*);

void print_dll(struct dll*);
