//*************************************************************************
//*******************      USER DEFINED OPTIONS    ************************
//*************************************************************************

//This dicates the number of possible partitions the GA plays with
//Larger values allow for more variance
#define POPULATION_SIZE 60

//Prints the cutstates of individual chromosomes on every pass
#define PRINT_CHROMOSOMES 0

//This raises the culling threshold towards only the best chromosomes
//range of 0-arbitrary, 0 resulting in half the population culled (average becomes threshold)
#define WEIGH_TOWARDS_TOP_CHROMOSOME 4

//This dictates the number of iterations the GA goes through
//Higher numbers result in more recombination
#define NUM_GA_PASSES 40

//The max_number of crossovers that will occur
// The breeding function allows less than the max to occur
#define NUM_CROSSOVERS 5

//The frequency that any particular offspring gene will mutate
//This is written as percent, so the default 5 is a 5% chance of mutation
// Floats or integers should work, (0,100) - recommended 2-5%
#define MUTATION_FREQUENCY 3

//*************************************************************************


//The order of these enum options matter, A should be 0, B should be 1
typedef enum{
	ONLY_PARTITION_A,
	ONLY_PARTITION_B,
	NO_DATA,
	IN_CUTSTATE
} netstate;

struct chromosome{
	int* netstate_array;
	//For reading
	int* gene_array;
	int cutstate;
	int balanced;
};

void segregate_cells_with_GA(struct condensed*);
struct chromosome* choose_best_balanced_chromosome(struct chromosome**, struct condensed*);

void generate_chromosomes(struct chromosome**, struct condensed*);
void initialize_chromosome(struct chromosome*, struct condensed*);
void populate_chromosome(struct chromosome*, struct condensed*);

void cull_bad_chromosomes(struct chromosome**, struct condensed*);
void breed_chromosome_offspring(struct chromosome**, struct condensed*);
void mutate_offspring(struct chromosome*, int);

void print_CHROMOSOME_array(struct chromosome** CHROMOSOME_array);
void delete_chromosome(struct chromosome*);
void free_CHROMOSOME_array(struct chromosome**);

int find_cutstates_and_balance_of_population(struct chromosome** CHROMOSOME_array, struct condensed* information);
