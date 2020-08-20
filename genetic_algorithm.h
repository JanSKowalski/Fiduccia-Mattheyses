#include "main.h"

//*************************************************************************
//*******************      USER DEFINED OPTIONS    ************************
//*************************************************************************

//This dicates the number of possible partitions the GA plays with
//Larger values allow for more variance
#define POPULATION_SIZE 200


//This dictates the number of iterations the GA goes through
//Higher numbers result in more recombination
#define NUM_GA_PASSES 4

//*************************************************************************

typedef enum{
	NO_DATA,
	ONLY_PARTITION_A,
	ONLY_PARTITION_B,
	IN_CUTSTATE
} net_state;

struct chromosome{
	int* net_state_array;
	struct dll* gene_list;
	int cutstate;
};

void segregate_cells_with_GA(struct condensed*);

void generate_chromosomes(struct chromosome**, struct condensed*);
void initialize_chromosome(struct chromosome*, int);
void populate_chromosome(struct chromosome*, struct condensed*);

void delete_chromosome(struct chromosome*);
void free_CHROMOSOME_array(struct chromosome**);

void find_cutstates_of_population(struct chromosome** CHROMOSOME_array, struct condensed* information);
