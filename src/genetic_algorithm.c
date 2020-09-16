#include "../include/main.h"

//Main organizing GA function
void segregate_cells_with_GA(struct condensed* information){

	//Generate POPULATION_SIZE chromosome structs with random inputs similar to random (close to balanced but not rigorously enforced)
	struct chromosome** CHROMOSOME_array = malloc(POPULATION_SIZE * sizeof(struct chromosome*));

	if(information->FM_chromosome == NULL){
		generate_chromosomes(CHROMOSOME_array, information);
	}
	else{
		introduce_FM_chromosome(CHROMOSOME_array, information);
		breed_chromosome_offspring(CHROMOSOME_array, information);
	}

	find_cutstates_and_balance_of_population(CHROMOSOME_array, information);

	int no_balanced_chromosomes = 1;
	int pass_number = 0;
	struct dll* good_chromosomes;

	if(PRINT_CHROMOSOMES)
		print_CHROMOSOME_array(CHROMOSOME_array);

	int repeat_cutoff = 5;
	//for NUM_GA_PASSES, cull the bad chromosomes, breed the good chromosomes, calculate the cutstate of the new chromosomes
	while((pass_number < NUM_GA_PASSES) || (no_balanced_chromosomes && (pass_number >= NUM_GA_PASSES))){

		if(repeat_cutoff == 0){
			printf("Reset to random\n");
			segregate_cells_randomly(information);
			return;
		}

		//Cull bad chromosomes
		cull_bad_chromosomes(CHROMOSOME_array, information);

		//Take parts of two chromosomes with better than average cutstates
		breed_chromosome_offspring(CHROMOSOME_array, information);

		//Find the cutstates of chromosomes and note whether a balanced chromosome exists
		no_balanced_chromosomes = !(find_cutstates_and_balance_of_population(CHROMOSOME_array, information));

		if(PRINT_CHROMOSOMES)
			print_CHROMOSOME_array(CHROMOSOME_array);

		pass_number++;
		if (no_balanced_chromosomes && pass_number > NUM_GA_PASSES)
			repeat_cutoff--;
	}

	//Check the CHROMOSOME_array for chromosomes within the balance tolerance.
	struct chromosome* chosen_chromosome = choose_best_balanced_chromosome(CHROMOSOME_array, information);

	if (chosen_chromosome == NULL)
		printf("Chosee NULL\n");

	int* gene_array = chosen_chromosome->gene_array;

	//Housekeeping data structures
	struct dll* list_of_cells_A = malloc(sizeof(struct dll));
	struct dll* list_of_cells_B = malloc(sizeof(struct dll));
	initialize_dll(list_of_cells_A);
	initialize_dll(list_of_cells_B);
	int total_partition_area_A = 0;
	int total_partition_area_B = 0;

	int i;
	//Add cells the the appropriate lists
	for(i=0;i< information->CELL_array_size; i++){
		if(gene_array[i] == PARTITION_A){
			insert_node(list_of_cells_A, 0, information->CELL_array[i]);
			total_partition_area_A += information->CELL_array[i]->area;
			information->CELL_array[i]->partition = information->partition_A;
			information->CELL_array[i]->which_partition = PARTITION_A;
		}
		else{
			insert_node(list_of_cells_B, 0, information->CELL_array[i]);
			total_partition_area_B += information->CELL_array[i]->area;
			information->CELL_array[i]->partition = information->partition_A;
			information->CELL_array[i]->which_partition = PARTITION_B;
		}
	}


	//Add cells to dll lists based on the winning chromosome, pass into copy_cells_into_partitions()
	//end with list of which cells go into which partition
	copy_cells_into_partitions(information->partition_A, information->partition_B, list_of_cells_A, list_of_cells_B, total_partition_area_A, total_partition_area_B);

	//free individual chromosomes, then the structure
	free_CHROMOSOME_array(CHROMOSOME_array);

	garbage_collection_dll(list_of_cells_A, DO_NOT_DEALLOC_DATA);
	garbage_collection_dll(list_of_cells_B, DO_NOT_DEALLOC_DATA);
}

//Assumes that at least one chromosome will be balanced
// This is guaranteed by the find_cutstates_and_balance_of_population() function, which will force loop until a suitable chromosome appears
struct chromosome* choose_best_balanced_chromosome(struct chromosome** CHROMOSOME_array, struct condensed* information){
	struct chromosome* chosen_chromosome = NULL;
	int i;
	struct chromosome* temp_chromosome;
	for(i=0; i< POPULATION_SIZE;i++){
		temp_chromosome = CHROMOSOME_array[i];
		if (temp_chromosome->balanced && (chosen_chromosome == NULL || (temp_chromosome->cutstate < chosen_chromosome->cutstate)))
			chosen_chromosome = temp_chromosome;
	}
	return chosen_chromosome;
}

//Malloc all the chromosome structs, set the cell_genes to partition A or B, add to CHROMOSOME_array
void generate_chromosomes(struct chromosome** CHROMOSOME_array, struct condensed* information){

	struct chromosome* temp_chromosome;

	int i;
	//Create each chromosome
	for(i=0; i< POPULATION_SIZE; i++){
		temp_chromosome = malloc(sizeof(struct chromosome));
		initialize_chromosome(temp_chromosome, information);
		populate_chromosome(temp_chromosome, information);
		CHROMOSOME_array[i] = temp_chromosome;
	}

}

//Set the first element of CHROMOSOME_array to FM_chromosome, set the rest to NULL
void introduce_FM_chromosome(struct chromosome** CHROMOSOME_array, struct condensed* information){
	int i;
//	CHROMOSOME_array[0] = information->FM_chromosome;
	//Copy over the data from FM_chromosome
	struct chromosome* FM_chromosome_copy = malloc(sizeof(struct chromosome));
	initialize_chromosome(FM_chromosome_copy, information);
	for(i=0;i<information->CELL_array_size;i++){
		FM_chromosome_copy->gene_array[i] = information->FM_chromosome->gene_array[i];
	}
	CHROMOSOME_array[0] = FM_chromosome_copy;

	//Set the rest of the chromosomes to NULL
	for(i=1; i< POPULATION_SIZE; i++){
		CHROMOSOME_array[i] = NULL;
	}
}


void initialize_chromosome(struct chromosome* temp_chromosome, struct condensed* information){
	int* netstate_array = malloc(information->NET_array_size * sizeof(int));
	temp_chromosome->netstate_array = netstate_array;

	int* gene_array = malloc(information->CELL_array_size * sizeof(int));
	temp_chromosome->gene_array = gene_array;

	temp_chromosome->cutstate = 0;
}


void populate_chromosome(struct chromosome* temp_chromosome, struct condensed* information){
	//Access the list
	int* gene_array = temp_chromosome->gene_array;

	//for i in num_cells, insert_node an int
	//Also copy this information into the gene_list_array
	int i;
	for(i=0;i < information->CELL_array_size;i++){
		//Randomize
		srand(time(NULL)+rand());
		//Make the chromosome random but roughly balanced
		if (rand() % (int)(1.0/(information->ratio)) == 0)
			gene_array[i] = PARTITION_A;
		else
			gene_array[i] = PARTITION_B;
	}

	//Set all the netstates to no_data, to be updated in find_cutstates_of_population()
	for(i=0;i< information->NET_array_size; i++){
		temp_chromosome->netstate_array[i] = NO_DATA;
	}
}



void cull_bad_chromosomes(struct chromosome** CHROMOSOME_array, struct condensed* information){

	//find the average of cutstates
	int i;
	int cutstate_average = 0;
	int smallest_cutstate = 999999999;
	for(i=0;i<POPULATION_SIZE;i++){
		cutstate_average += CHROMOSOME_array[i]->cutstate;
		if (CHROMOSOME_array[i]->cutstate < smallest_cutstate)
			smallest_cutstate = CHROMOSOME_array[i]->cutstate;
	}
	cutstate_average = cutstate_average / POPULATION_SIZE;
	//It's possible to weigh this closer or farther from the average
//	int threshold = (cutstate_average + WEIGH_TOWARDS_TOP_CHROMOSOME*smallest_cutstate)/(WEIGH_TOWARDS_TOP_CHROMOSOME + 1);
	int threshold = (cutstate_average + (information->genetic_cutoff)*smallest_cutstate)/(information->genetic_cutoff + 1);
	if (PRINT_AVERAGE_CUTSTATE)
		printf("Cutstate_average: %d\n", cutstate_average);
	for(i=0;i<POPULATION_SIZE;i++){
		if(CHROMOSOME_array[i]->cutstate > threshold){
			delete_chromosome(CHROMOSOME_array[i]);
			CHROMOSOME_array[i] = NULL;
		}
	}


}

//Assumes bad chromosomes have been replaced by NULLs by cull_bad_chromosomes()
void breed_chromosome_offspring(struct chromosome** CHROMOSOME_array, struct condensed* information){
	int i;
	struct chromosome* parents[2];
	int num_good_candidates = 0;

	for(i=0;i<POPULATION_SIZE;i++){
		if (CHROMOSOME_array[i] != NULL)
			num_good_candidates++;
	}

	struct chromosome* good_candidates[num_good_candidates];

	int gc_index = 0;
	for(i=0;i<POPULATION_SIZE;i++){
		if (CHROMOSOME_array[i] != NULL){
			good_candidates[gc_index] = (struct chromosome*) CHROMOSOME_array[i];
			gc_index++;
		}
	}

	int cell_gene_index;
	int donor_parent;
	int crossovers_to_occur;
	int crossover_location;
	struct chromosome* temp_chromosome;
	//go through CHROMOSOME_array
	for(i=0; i< POPULATION_SIZE; i++){
		temp_chromosome = CHROMOSOME_array[i];
		//Replace NULLs with new chromosomes
		if (temp_chromosome == NULL){

			//Create new chromosome
			temp_chromosome = malloc(sizeof(struct chromosome));
			initialize_chromosome(temp_chromosome, information);

			//Decide on the parents
			if (num_good_candidates == 1){
				parents[0] = good_candidates[0];
				parents[1] = good_candidates[0];
			}
			else{
				srand(time(NULL) + rand());
				parents[0] = good_candidates[rand() % num_good_candidates];
				srand(time(NULL) + rand());
				parents[1] = good_candidates[rand() % num_good_candidates];
			}

			cell_gene_index = 0;
			donor_parent = 0;
			crossovers_to_occur = NUM_CROSSOVERS;
			crossover_location = (rand() % (information->CELL_array_size));
			for(cell_gene_index = 0; cell_gene_index < information->CELL_array_size; cell_gene_index++){
				//Switch if allowed at random
				if ((crossovers_to_occur > 0) && (cell_gene_index == crossover_location)){
					donor_parent = !(donor_parent);
					crossovers_to_occur--;
					srand(time(NULL) + rand());
					crossover_location = (rand() % (information->CELL_array_size));
				}

				temp_chromosome->gene_array[cell_gene_index] = parents[donor_parent]->gene_array[cell_gene_index];
			}

			mutate_offspring(temp_chromosome, information->CELL_array_size, information);

		}
		//Place the chromosomes back with the others
		CHROMOSOME_array[i] = temp_chromosome;
	}

}



void mutate_offspring(struct chromosome* offspring, int num_cells, struct condensed* information){
	int i;
	int* gene_array = offspring->gene_array;
	for(i=0;i<num_cells;i++){
		srand(time(NULL)+rand());
//		if(rand() % (int)( 100.0 / (MUTATION_FREQUENCY) ) == 0)
		if(rand() % (int)( 100.0 / (information->mutation_frequency) ) == 0)
			gene_array[i] = !(gene_array[i]);
	}
}


//Returns true if at least one chromosome is balanced
int find_cutstates_and_balance_of_population(struct chromosome** CHROMOSOME_array, struct condensed* information){
	struct chromosome* temp_chromosome;

	//Assume there is no balanced chromosome at the start
	int are_there_balanced_chromosomes = 0;
	int temp_partition_A_area;

	int chromosome_index;
	int cell_gene_index;

	struct cell* temp_cell;
	int temp_cell_partition;

	struct node* temp_net_node;
	int net_id;

	int i;
	int cutstate_counter;
	for(chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++){
		temp_chromosome = CHROMOSOME_array[chromosome_index];

		//Sanity check, this should never be true
		if(temp_chromosome == NULL)
			printf("NULL chromosomes, %d\n", chromosome_index);

		//Reset netstate_information
		for(i=0;i< information->NET_array_size; i++){
			temp_chromosome->netstate_array[i] = NO_DATA;
		}

		temp_partition_A_area = 0;

		//Access the cells, find the cutstate
		for(cell_gene_index = 0;cell_gene_index < information->CELL_array_size; cell_gene_index++){
			//Find the partition of the cell
			temp_cell_partition = temp_chromosome->gene_array[cell_gene_index];


			//Use the actual cell to find out which nets are involved
			temp_cell = information->CELL_array[cell_gene_index];
			temp_net_node = temp_cell->nets->head->next;

			//Add the cell area to the counter so that we can determine if the chromosome is balanced
			if(temp_cell_partition == PARTITION_A)
				temp_partition_A_area += temp_cell->area;

			//For each net access the netid, update the chromosome's netstate_array
			while(temp_net_node != temp_cell->nets->tail){
				net_id = ((struct net*) temp_net_node->data_structure)->identifier;

				//Update netstate values
				if(temp_chromosome->netstate_array[net_id] == NO_DATA){
					temp_chromosome->netstate_array[net_id] = temp_cell_partition;
				}
				else if(temp_chromosome->netstate_array[net_id] == !(temp_cell_partition)){
					temp_chromosome->netstate_array[net_id] = IN_CUTSTATE;
				}

				temp_net_node = temp_net_node->next;
			}
		}

		//Figure out the balance of the chromosome
		if ((temp_partition_A_area > (information->desired_area - information->tolerance)) && (temp_partition_A_area < (information->desired_area + information->tolerance))){
			temp_chromosome->balanced = 1;
			are_there_balanced_chromosomes = 1;
		} else{
			temp_chromosome->balanced = 0;
		}

		//Figure out the cutstate value of the chromosome
		cutstate_counter = 0;
		for(i=0;i < information->NET_array_size; i++){
			if(temp_chromosome->netstate_array[i] == IN_CUTSTATE){
				cutstate_counter++;
			}
		}

		//Go through netstate_array, count instances of IN_CUTSTATE
		temp_chromosome->cutstate = cutstate_counter;
	}

	return are_there_balanced_chromosomes;
}



void print_CHROMOSOME_array(struct chromosome** CHROMOSOME_array){
	int i;
	printf("Chromosome array:\n");
	for(i=0;i<POPULATION_SIZE;i++){
		printf("Chromosome %d cutstate: %d\n", i, CHROMOSOME_array[i]->cutstate);
	}
}


void delete_chromosome(struct chromosome* unwanted_chromosome){
	if (unwanted_chromosome != NULL){
		free(unwanted_chromosome->netstate_array);
		free(unwanted_chromosome->gene_array);
		free(unwanted_chromosome);
	}
}


void free_CHROMOSOME_array(struct chromosome** CHROMOSOME_array){
	int i;
	for(i=0; i<POPULATION_SIZE;i++){
		delete_chromosome(CHROMOSOME_array[i]);
	}
	free(CHROMOSOME_array);
}
