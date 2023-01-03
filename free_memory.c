// Copyright 2022 Alexandru Alin-Ioan 312CA
#include <stdio.h>
#include <stdlib.h>
#include "my_octave_lib.h"

// frees a bidimensional array
void free_simple_matrix(int **a, int m)
{
	int i;
	for (i = 0; i < m; i++)
		free(a[i]);
	free(a);
}

// Frees one element from the list
void free_matrix(matrix **first, matrix **last)
{
	int index, nbr = 0;
	matrix *crt = *first, *del;
	scanf("%d", &index);
	for (; crt && nbr < index; nbr++, crt = crt->next)
		;
	if (!crt || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		del = crt;
		// Checks the position of the element to properly remake the
		// the connections between nodes
		if (crt != *first && crt != *last) {
			crt->prev->next = crt->next;
			crt->next->prev = crt->prev;
		} else if (crt == *first) {
			if ((*first)->next)
				(*first)->next->prev = NULL;
			*first = (*first)->next;
		} else {
			(*last)->prev->next = NULL;
			*last = (*last)->prev;
		}
		free_simple_matrix(del->mat, del->m);
		free(del);
	}
}

// Frees the list
void free_list(matrix **first, matrix **last)
{
	matrix *crt = *first, *del;
	while (crt) {
		del = crt;
		crt = crt->next;
		free_simple_matrix(del->mat, del->m);
		free(del);
	}
	*first = NULL;
	*last = NULL;
}
