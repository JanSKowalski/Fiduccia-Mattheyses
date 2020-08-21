##############################################################
#Main compile for Fiduccia-Mattheyses
##############################################################
all: main.o dll_structure.o basic_objects.o populate_partitions.o data_input.o fiduccia_mattheyses.o genetic_algorithm.o
	gcc -o main.out main.o dll_structure.o basic_objects.o populate_partitions.o data_input.o fiduccia_mattheyses.o genetic_algorithm.o
	rm *.o
	./main.out

main: main.c main.h
	gcc -c main.c main.h

dll_structure: dll_structure.c 
	gcc -c dll_structure.c 

basic_objects: basic_objects.c 
	gcc -c basic_objects.c 

populate_partitions: populate_partitions.c populate_partitions.h
	gcc -c populate_partitions.c populate_partitions.h

data_input: data_input.c 
	gcc -c data_iput.c 

fiduccia_mattheyses: fiduccia_mattheyses.c fiduccia_mattheyses.h
	gcc -c fiduccia_mattheyses.c fiduccia_mattheyses.h

genetic_algorithm: genetic_algorithm.c genetic_algorithm.h
	gcc -c genetic_algorithm.c genetic_algorithm.h

##############################################################
#Extra options
##############################################################
#Check heap memory for leaks
valgrind: all 
	valgrind --leak-check=full --track-origins=yes ./main.out

figures: all
	gnuplot ibm_figures.plot
