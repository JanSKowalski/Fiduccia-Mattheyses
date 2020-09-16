#include "../include/main.h"
/*
An implementation of doubly-linked lists
Jan Kowalski 3/2020
*/


//DLL lists need to be initialized before they can be used!

/*
Initialize a doubly linked list
	nodes have arbitrary data type
	every list has a size (excluding sentinels)
	dlls are zero-indexed
Inputs: list
	list - a malloc'd pointer to a dll struct
Outputs: <none>
NOTE: Assumes the list has been malloc'ed
*/
void initialize_dll(struct dll* list){
	struct node* head_sentinel = malloc(sizeof(struct node));
	struct node* tail_sentinel = malloc(sizeof(struct node));

	list->size = 0;
	list->head = head_sentinel;
	list->tail = tail_sentinel;

	connect_two_nodes(head_sentinel, tail_sentinel);
}


//Assumes data pointer is to an integer value
//Used for debugging only, O(n) function
void print_dll(struct dll* list, datatype node_data){
	struct node* placeholder = list->head->next;
	int nodes_remaining = list->size;
	while (nodes_remaining > 0){
		if (node_data == INTEGER){
			print_integer(placeholder->data_structure);
		}
		else if (node_data == CELL){
			print_cell(placeholder->data_structure);
		}
		else if (node_data == NET){
			print_net(placeholder->data_structure);
		}
		nodes_remaining--;
		placeholder = placeholder->next;
	}
	printf("\n");
}

/*
Helper function, allows bidirectional travel
Inputs: first, second
	first - the first node
	second - the second node
Outputs: <none>
*/
void connect_two_nodes(struct node* first, struct node* second){
	first->next = second;
	second->previous = first;
}

/*
Mallocs a new node with the desired data object,
	adds it to the correct position in list
	Position is zero-indexed
	Does NOT handle out-of-bounds yet
Inputs: list, position, data_structure
	list - The list that the node will be added to
	position - Where the list will be included
	data_structure - a pointer to the data structure
			making this void* allows a generic data type in node
Outputs: The node it created
*/
struct node* insert_node(struct dll* list, int position, void* data_structure){

	//reach correct position
	int index = position;
	struct node* placeholder = list->head;
	while (index > 0){
		placeholder = placeholder->next;
		index--;
	}

	//initialize new node
	struct node* new_node = (struct node*) malloc(sizeof(struct node));

	//link data_structure to node
	new_node->data_structure = data_structure;

	//include in list
	connect_two_nodes(new_node, placeholder->next);
	connect_two_nodes(placeholder, new_node);

	list->size = list->size + 1;
	return new_node;
}


/*
Counts the number of nodes between the two sentinels
*/
int dll_size(struct dll* list){
	return list->size;
}

/*
Given a list and a particular object, find the node in the list that contains that object
*/
struct node* find_node_in_cell(struct dll* list, void* data_structure){
	struct node* temp_node = list->head->next;
	while(temp_node != list->tail){
		if (temp_node->data_structure == data_structure)
			return temp_node;
	}
	return 0;
}


/*
Given a node in the list, this function returns the next node
*/
struct node* access_next_node(struct node* current_node){
	//Does not check to see if current_node is tail
	return current_node->next;
}



//Position is zero indexed, must be a value less than string size
//Frees the data structure
//NOTE: This is an O(n) implementation
void remove_node_using_list(struct dll* list, int position){

	if (position >= list->size ){
		fprintf(stderr, "Error: Attempting to remove a nonexistant node.\n");
		return;
	}

	//Find position
	int index = position;
	struct node* placeholder = list->head;
	while (index >= 0){
		placeholder = placeholder->next;
		index--;
	}

	connect_two_nodes(placeholder->previous, placeholder->next);

	//Free data_structure linked with node
	free(placeholder->data_structure);

	//Free the node
	free(placeholder);

	list->size = list->size - 1;
}

//Does not free the data structure, returns the node
//O(1) remove function requires the pointer to the node
struct node* remove_node(struct node* node_being_removed, struct dll* list){
	connect_two_nodes(node_being_removed->previous, node_being_removed->next);
	list->size -= 1;
	return node_being_removed;
}

//Takes in an initialized dll struct
//Frees memory
void garbage_collection_dll(struct dll* list, garbage_handler option){

	struct node* head = list->head;

	//Neither head nor tail have data_structures associated
	//It's simpler to treat them separately
	garbage_collection_dll_recursive(head->next, list->tail, option);
	free(head);
	free(list);
}


//Input first(not head) and tail nodes of the ddl
void garbage_collection_dll_recursive(struct node* temp, struct node* tail, garbage_handler option){
	if (temp != tail){
		garbage_collection_dll_recursive(temp->next, tail, option);
		if (option == DEALLOC_DATA){
			free(temp->data_structure);
		}
	}
	free(temp);
}

