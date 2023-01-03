// Copyright 2022 Alexandru Alin-Ioan 312CA
#include <stdio.h>
#include <stdlib.h>
#include "my_octave_lib.h"

#define MOD 10007

// sum of two numbers, then modulo, then, if negative, makes it positve
int sum_modulo(int sum, int to_add)
{
	sum += to_add;
	sum %= MOD;
	if (sum < 0)
		sum += MOD;
	return sum;
}

// allocs memory for a bidimensional array
int **alloc_matrix(int m, int n)
{
	int i, j;
	int **a = (int **)malloc(sizeof(int *) * m);
	if (!a) {
		fprintf(stderr, "Matrix could not be loaded\n");
		return NULL;
	}
	for (i = 0; i < m; i++) {
		a[i] = (int *)malloc(sizeof(int) * n);
		if (!a[i]) {
			// goes back and frees what has been allocated
			for (j = i - 1; j >= 0; j--)
				free(a[j]);
			free(a);
			a = NULL;
			fprintf(stderr, "Matrix could not be loaded\n");
			return a;
		}
	}
	return a;
}

// reads an array
void read_matrix(int **a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			scanf("%d", &a[i][j]);
	}
}

// initilaizes a new node to be put in the linked list
void matrix_init(matrix **crt)
{
	*crt = (matrix *)malloc(sizeof(matrix));
	if (!(*crt)) {
		fprintf(stderr, "Matrix could not be loaded\n");
	} else {
		scanf("%d %d", &(*crt)->m, &(*crt)->n);
		(*crt)->mat = alloc_matrix((*crt)->m, (*crt)->n);
		if (!(*crt)->mat) {
			free(*crt);
			*crt = NULL;
			return;
		}
		read_matrix((*crt)->mat, (*crt)->m, (*crt)->n);
		(*crt)->sum = 0;
		for (int i = 0; i < (*crt)->m; i++) {
			for (int j = 0; j < (*crt)->n; j++)
				(*crt)->sum = sum_modulo((*crt)->sum, (*crt)->mat[i][j]);
		}
		(*crt)->next = NULL;
		(*crt)->prev = NULL;
	}
}

// puts the initialized node at the back of the linked list
void load_matrix(matrix **first, matrix **last)
{
	if (!(*first)) {
		matrix_init(first);
		if (!(*first))
			return;
		*last = *first;
	} else {
		matrix *crt = NULL;
		matrix_init(&crt);
		if (!crt)
			return;
		(*last)->next = crt;
		crt->prev = (*last);
		(*last) = crt;
	}
}
