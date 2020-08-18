#Note, -g flag enabled by default

all: main.o dll_structure.o basic_objects.o populate_partitions.o data_input.o fiduccia_mattheyses.o
	gcc -o main.out main.o dll_structure.o basic_objects.o populate_partitions.o data_input.o fiduccia_mattheyses.o -g
	rm *.o
	./main.out
	gnuplot figure.plot
	xdg-open Data/Figures/ibm01_cutstate.png

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

#Debug and memory checking commands
debug: all
	gdb ./main.out

valgrind: all 
	valgrind --leak-check=full --track-origins=yes ./main.out
	
