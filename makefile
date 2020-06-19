#Note, -g flag enabled by default

all: main.o dll_structure.o basic_objects.o
	gcc -o main.out main.o dll_structure.o basic_objects.o -g
	rm *.o
	./main.out

main: main.c
	gcc -c main.c

dll_structure: dll_structure.c dll_structure.h
	gcc -c dll_structure.c dll_structure.h

basic_objects: basic_objects.c basic_objects.h
	gcc -c basic_objects.c basic_objects.h



#Debug and memory checking commands
debug: all
	gdb ./main.out

valgrind: all 
	valgrind --leak-check=full --track-origins=yes ./main.out
	
