// Copyright 2022 Alexandru Alin-Ioan 312CA
#include <stdio.h>
#include <stdlib.h>
#include "my_octave_lib.h"

int main(void)
{
	matrix *first = NULL, *last = NULL;
	char command;
	// reads commands until the quit(Q) command
	scanf("%c", &command);
	while (command != 'Q') {
		switch (command) {
		case 'L':
			load_matrix(&first, &last);
			break;
		case 'D':
			print_dimensions(first);
			break;
		case 'P':
			print_matrix(first);
			break;
		case 'F':
			free_matrix(&first, &last);
			break;
		case 'C':
			resize_matrix(first);
			break;
		case 'M':
			multiply_matrixes(first, &last);
			break;
		case 'O':
			sort_list(first);
			break;
		case 'T':
			transpose_matrix(first);
			break;
		case 'R':
			exponetiate_matrix(first);
			break;
		case 'S':
			strassen_setup(first, &last);
			break;
		default:
			if (command >= 'A' && command <= 'Z')
				printf("Unrecognized command\n");
			break;
		}
		scanf("%c", &command);
	}
	// This is for the Q command
	free_list(&first, &last);
}
