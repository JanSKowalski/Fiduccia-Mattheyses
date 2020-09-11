#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "dll_structure.h"
#include "data_input.h"
#include "basic_objects.h"
#include "populate_partitions.h"
#include "genetic_algorithm.h"
#include "fiduccia_mattheyses.h"

#define YES 1
#define NO 0

//Toy files
#define TEST_ARE_FILENAME "data/testdata.are"
#define TEST_NETD_FILENAME "data/testdata.netD"

//*************************************************************************
//*******************      USER DEFINED OPTIONS    ************************
//*************************************************************************


//This option runs the Fiduccia-Mattheyses algorithm on the provided
// testdata.are and testdata.netD files. It also turns on verbose features,
// such as printing partition states and pass cutstate values
//The demo should be run with PARTITION_CELLS set to RANDOMLY
//Turn this to NO in order to run the IBM testbenches (assuming download)
#define RUN_DEMO_WITH_TESTDATA NO

//Assuming RUN_DEMO_WITH_TESTDATA is NO, this value will determine which
// circuit will be analyzed.
//Number should be an integer [1,18]
#define IBM_FILE_NUMBER 1

//This constant decides the size of the partitions relative to one another
// As an example, 0.5 indicates that both partitions should have the same area
//Ratio is a double between 0<r<1
#define RATIO 0.5

//This option activates a function that at every pass goes through the netlist
// and counts from scratch the nets with cells in both partitions
//This option is time inefficient and should be NO normally, set to YES to
// ease doubts about the program results
#define DOUBLE_CHECK_CUTSTATE_VALUES NO

//This option prints the GAIN_array and cell identifiers of every partition at
// every pass. This is helpful to show how the algorithm works for small chips
// like testdata, but can be unwieldy for larger chips like the benchmarks
//By default, this is set to NO
#define PRINT_PARTITION_STATES NO

//This option simply prints the cutstate value of the system at every pass
//Set to NO by default
#define PRINT_PASS_CUTSTATE_VALUES NO

//The inputs for this option are defined under the how_to_partition enum
//This determines how cells are initially distributed between the two partitions
//More specific options for the genetic algorithm are under genetic_algorithm.h
//The default option is RANDOMLY
#define PARTITION_CELLS GENETIC_ALGORITHM
//#define PARTITION_CELLS RANDOMLY


//*************************************************************************
//Options for the FM algorithm itself
//Cutoff can be turned on without repeats, but repeats shouldn't be turned on without cutoff

//Does the algorithm stop after the cutstate starts to increase again?
#define FM_CUTOFF YES

//At what point does the algorithm stop?
//Keep in mind that 0 allows no movement for the algorithm to get out of local minima
//Minimum 0, integer
#define FM_CUTOFF_THRESHOLD 5


//Should never be turned on without cutoff also being on.
//This is because the last pass is always the partition copied over
//Without cutoff, the last partition is rubbish
#define FM_REPEAT NO

//How many times do you apply FM to the same circuit, using the last
// output as the starting position
//Minimum 1, integer values
//MUST BE 1 if FM_REPEAT is NO (errors otherwise)
#define FM_NUM_PASSES 1

//*************************************************************************


typedef enum{
	RANDOMLY,
	GENETIC_ALGORITHM
} how_to_partition;

struct condensed{
	struct cell** CELL_array;
	int CELL_array_size;
	struct net** NET_array;
	int NET_array_size;
	struct partition** access_; //0 is partition A, 1 is partition B
	struct partition* partition_A;
	struct partition* partition_B;
	char* results_filename;

	int total_pin_count;
	//The area of the largest cell is used as the balance tolerance
	int tolerance;
	//The sum of all cell areas
	int total_area;
	int desired_area;
	double ratio;
	//The highest number of nets connected to a single cell
	int max_nets;
	//The smallest number of nets between partitions encountered during FM passes
	int lowest_cutstate;
	//The cutstate value during the current pass
	int current_cutstate;

	double mutation_frequency;
	int genetic_cutoff;

	struct chromosome* FM_chromosome;
};

void import_data_and_run_algorithm(char *, char *, char*, double, int);
void reset_cells_and_nets(struct condensed*);
void free_all_memory(struct condensed*);

