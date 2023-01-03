# Alexandru Alin-Ioan 312CA 2022

# compiler setupc
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# define targets
TARGETS= my_octave_main load operations strassen free

build: $(TARGETS) 
	$(CC) $(CFLAGS) my_octave.o load_matrix.o matrix_operations.o strassen.o free_memory.o -o my_octave

my_octave_main:
	$(CC) $(CFLAGS) -c my_octave.c -o my_octave.o

load:
	$(CC) $(CFLAGS) -c load_matrix.c -o load_matrix.o

operations:
	$(CC) $(CFLAGS) -c matrix_operations.c -o matrix_operations.o

free:
	$(CC) $(CFLAGS) -c free_memory.c -o free_memory.o

strassen:
	$(CC) $(CFLAGS) -c strassen.c -o strassen.o

pack:
	zip -FSr 312CA_AlexandruAlin-Ioan_Tema2.zip README Makefile *.c *.h

clean:
	rm my_octave *.o

.PHONY: pack clean