struct dll{
	int size;
	struct node* head;
	struct node* tail;
};

struct node{
	int data;
	struct node* previous;
	struct node* next;
};

void initialize(struct dll*);
void connect_two_nodes(struct node*, struct node*);
void insert_node(struct dll*, int, int);
void remove_node(struct dll*, int);


void garbage_collection(struct dll*);
void garbage_collection_recursive(struct node*, struct node*);

void print_dll(struct dll*);
