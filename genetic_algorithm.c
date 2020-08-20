#include "genetic_algorithm.h"
#include "populate_partitions.h"
#include "dll_structure.h"
#include "basic_objects.h"


//Main organizing GA function
void segregate_cells_with_GA(struct condensed* information){

	//Generate POPULATION_SIZE chromosome structs with random inputs similar to random (close to balanced but not rigorously enforced)
	struct chromosome** CHROMOSOME_array = malloc(POPULATION_SIZE * sizeof(struct chromosome*));
	generate_chromosomes(CHROMOSOME_array, information);
	find_cutstates_of_population(CHROMOSOME_array, information);

	int i;
	//for NUM_GA_PASSES, cull the bad chromosomes, breed the good chromosomes, calculate the cutstate of the new chromosomes
	for(i=0;i<NUM_GA_PASSES;i++){
		//Cull bad chromosomes
		
		//Breed good chromosomes

		//Find the cutstates
	}
	//Check the CHROMOSOME_array for chromosomes within the balance tolerance.


	//Add cells to dll lists based on the winning chromosome, pass into copy_cells_into_partitions()
	//end with list of which cells go into which partition
//	copy_cells_into_partitions();

	free_CHROMOSOME_array(CHROMOSOME_array);
}


//Malloc all the chromosome structs, set the cell_genes to partition A or B, add to CHROMOSOME_array
void generate_chromosomes(struct chromosome** CHROMOSOME_array, struct condensed* information){

	struct chromosome* temp_chromosome;

	int i;
	//Create each chromosome
	for(i=0; i< POPULATION_SIZE; i++){
		temp_chromosome = malloc(sizeof(struct chromosome));
		initialize_chromosome(temp_chromosome, information->CELL_array_size);
		populate_chromosome(temp_chromosome, information);
		CHROMOSOME_array[i] = temp_chromosome;
	}

}


void initialize_chromosome(struct chromosome* temp_chromosome, int num_cells){
	int* net_state_array = malloc(num_cells * sizeof(int));
	temp_chromosome->net_state_array = net_state_array;

	struct dll* gene_list = malloc(sizeof(struct dll));
	initialize_dll(gene_list);

	int cutstate = 0;
}


void populate_chromosome(struct chromosome* temp_chromosome, struct condensed* information){
	//Access the list
	struct dll* gene_list = temp_chromosome->gene_list;
	struct integer* cell_gene;

	//for i in num_cells, insert_node an int
	int i;
	for(i=0;i < information->CELL_array_size;i++){
		//Randomize
		srand(time(NULL)+rand());
		//Create cell_gene to represent a cell's partition in the chromosome
		cell_gene = malloc(sizeof(struct integer));
		//Make the chromosome random but roughly balanced
		if (rand() % (int)(1.0/(information->ratio)) == 0)
			cell_gene->data = PARTITION_A;
		else
			cell_gene->data = PARTITION_B;
		insert_node(gene_list, 0, cell_gene);
	}
}



void find_cutstates_of_population(struct chromosome** CHROMOSOME_array, struct condensed* information){
	struct chromosome* temp_chromosome;

	int chromosome_index;
	for(chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++){
		temp_chromosome = CHROMOSOME_array[chromosome_index];

		//Access the c
		temp_cell_node =
		
	}
}















void delete_chromosome(struct chromosome* unwanted_chromosome){
	free(unwanted_chromosome->net_state_array);
	garbage_collection_dll(unwanted_chromosome->gene_list, DEALLOC_DATA);
	free(unwanted_chromosome);
}


void free_CHROMOSOME_array(struct chromosome** CHROMOSOME_array){
	int i;
	for(i=0; i<POPULATION_SIZE;i++){
		delete_chromosome(CHROMOSOME_array[i]);
	}
	free(CHROMOSOME_array);
}
