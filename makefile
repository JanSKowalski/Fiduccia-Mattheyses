all: dll_structure.c dll_structure.h
	gcc -o dll_structure.out dll_structure.c dll_structure.h
	./dll_structure.out

#Intermediate step, compiles with -g flag
info: dll_structure.c dll_structure.h
	gcc -o dll_structure.out dll_structure.c dll_structure.h -g

debug: info
	gdb ./dll_structure.out

valgrind: info 
	valgrind --leak-check=full ./dll_structure.out 
	
	
