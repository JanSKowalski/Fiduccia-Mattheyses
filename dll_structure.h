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

typedef enum{
	INTEGER,
	CELL,
	NET
} datatype;

typedef enum{
	DEALLOC_DATA,
	DO_NOT_DEALLOC_DATA
} garbage_handler;

void initialize_dll(struct dll*);
void connect_two_nodes(struct node*, struct node*);


struct node* access_next_node(struct node*);
struct node* find_node_in_cell(struct dll*, void*);

struct node* insert_node(struct dll*, int, void*);
void remove_node_using_list(struct dll*, int);
struct node* remove_node(struct node*, struct dll*);

void garbage_collection_dll(struct dll*, garbage_handler);
void garbage_collection_dll_recursive(struct node*, struct node*, garbage_handler);

void print_dll(struct dll*, datatype);
