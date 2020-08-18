#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

//Toy files
#define TEST_ARE_FILENAME "Data/Input/testdata.are"
#define TEST_NETD_FILENAME "Data/Input/testdata.netD"

//*************************************************************************
//*******************      USER DEFINED OPTIONS    ************************
//*************************************************************************

//This constant decides the size of the partitions relative to one another
// As an example, 0.5 indicates that both partitions should have the same area
//Ratio is a double between 0<r<1
#define RATIO 0.5

//This option runs the Fiduccia-Mattheyses algorithm on the provided
// testdata.are and testdata.netD files. It also turns on verbose features,
// such as printing partition states and pass cutstate values
//Turn this to FALSE in order to run the IBM testbenches (assuming download)
#define RUN_DEMO_WITH_TESTDATA FALSE

//This option activates a function that at every pass goes through the netlist
// and counts from scratch the nets with cells in both partitions
//This option is time inefficient and should be FALSE normally, set to TRUE to
// ease doubts about the program results
#define DOUBLE_CHECK_CUTSTATE_VALUES FALSE

//This option prints the GAIN_array and cell identifiers of every partition at
// every pass. This is helpful to show how the algorithm works for small chips
// like testdata, but can be unwieldy for larger chips like the benchmarks
//By default, this is set to FALSE
#define PRINT_PARTITION_STATES FALSE

//This option simply prints the cutstate value of the system at every pass
//Set to FALSE by default
#define PRINT_PASS_CUTSTATE_VALUES FALSE

//*************************************************************************


struct condensed{
	struct cell** CELL_array;
	int CELL_array_size;
	struct net** NET_array;
	int NET_array_size;
	struct partition** access_; //0 is partition A, 1 is partition B
	struct partition* partition_A;
	struct partition* partition_B;
	char* results_csv;

	int max_number_of_pins_on_a_cell;
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
};

int find_max_number_of_pins_on_a_cell(struct cell**, int);
void import_data_and_run_algorithm(char *, char *, char*);
void free_all_memory(struct condensed*);

