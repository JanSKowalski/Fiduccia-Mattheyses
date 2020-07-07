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

void initialize_dll(struct dll*);
void connect_two_nodes(struct node*, struct node*);


struct node* access_next_node(struct node*);


void insert_node(struct dll*, int, void*);
void remove_node_using_list(struct dll*, int);
struct node* remove_node(struct node*);

void garbage_collection_dll(struct dll*);
void garbage_collection_dll_recursive(struct node*, struct node*);

void print_dll(struct dll*);
