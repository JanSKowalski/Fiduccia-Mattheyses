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

struct integer_data{
	int data;
};

void initialize(struct dll*);
void connect_two_nodes(struct node*, struct node*);

void insert_node(struct dll*, int, void*);
void insert_node_integer(struct dll*, int, int);

void remove_node(struct dll*, int);


void garbage_collection(struct dll*);
void garbage_collection_recursive(struct node*, struct node*);

void print_dll(struct dll*);
