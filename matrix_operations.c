// Copyright 2022 Alexandru Alin-Ioan 312CA
#include <stdio.h>
#include <stdlib.h>
#include "my_octave_lib.h"

// swaps two integers
void swap_ints(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// prints array
void print_simple_matrix(int **a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

// prints the dimensions of a given matrix
void print_dimensions(matrix *first)
{
	int index, nbr = 0;
	matrix *crt = first;
	scanf("%d", &index);
	for (; crt && nbr < index; nbr++, crt = crt->next)
		;
	if (!crt || index < 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", crt->m, crt->n);
}

// searches and prints a given matrix
void print_matrix(matrix *first)
{
	int index, nbr = 0;
	matrix *crt = first;
	scanf("%d", &index);
	for (; crt && nbr < index; nbr++, crt = crt->next)
		;
	if (!crt || index < 0)
		printf("No matrix with the given index\n");
	else
		print_simple_matrix(crt->mat, crt->m, crt->n);
}

// makes a new bidimensional array with just the lines and columns specified
int **make_resized_matrix(int **mat, int m, int n, int cols[], int lines[])
{
	int i, j, **new_mat;
	new_mat = alloc_matrix(m, n);
	if (!new_mat)
		return NULL;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			new_mat[i][j] = mat[lines[i]][cols[j]];
	}
	return new_mat;
}

// resizes a given matrix
void resize_matrix(matrix *first)
{
	int col_nbr, lin_nbr, *cols, *lines, **resized;
	int index, nbr = 0, i;
	matrix *crt = first;
	scanf("%d", &index);
	for (; crt && nbr < index; nbr++, crt = crt->next)
		;
	if (!crt || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		// reads the lines
		scanf("%d", &lin_nbr);
		lines = (int *)malloc(lin_nbr * sizeof(int));
		if (!lines) {
			fprintf(stderr, "To many lines");
			return;
		}
		for (i = 0; i < lin_nbr; i++)
			scanf("%d", &lines[i]);

		// reads the columns
		scanf("%d", &col_nbr);
		cols = (int *)malloc(col_nbr * sizeof(int));
		if (!cols) {
			fprintf(stderr, "To many columns");
			free(lines);
			return;
		}
		for (i = 0; i < col_nbr; i++)
			scanf("%d", &cols[i]);

		resized = make_resized_matrix(crt->mat, lin_nbr, col_nbr, cols, lines);
		if (!resized) {
			free(lines);
			free(cols);
			return;
		}
		// frees the old matrix
		free_simple_matrix(crt->mat, crt->m);
		crt->mat = resized;
		crt->m = lin_nbr;
		crt->n = col_nbr;

		crt->sum = 0;
		for (int i = 0; i < crt->m; i++) {
			for (int j = 0; j < crt->n; j++)
				crt->sum = sum_modulo(crt->sum, crt->mat[i][j]);
		}

		free(lines);
		free(cols);
	}
}

// calculates the result of the multiplication between two matrixes
int **multiply_calc(int **mat1, int **mat2, int m1, int n1, int n2)
{
	int **rslt, i, j, k;
	rslt = alloc_matrix(m1, n2);
	if (!rslt)
		return NULL;
	for (i = 0; i < m1; i++) {
		for (j = 0; j < n2; j++) {
			rslt[i][j] = 0;
			for (k = 0; k < n1; k++)
				rslt[i][j] = sum_modulo(rslt[i][j], (mat1[i][k] * mat2[k][j]));
		}
	}
	return rslt;
}

// multiplies two given matrixes
void multiply_matrixes(matrix *first, matrix **last)
{
	matrix *rslt, *mat1 = first, *mat2 = first;
	int index1, index2, nbr = 0, m1, n1, n2;
	scanf("%d", &index1);
	scanf("%d", &index2);

	for (nbr = 0; mat1 && nbr < index1; nbr++, mat1 = mat1->next)
		;
	for (nbr = 0; mat2 && nbr < index2; nbr++, mat2 = mat2->next)
		;
	if (!mat1 || !mat2 || index1 < 0 || index2 < 0) {
		printf("No matrix with the given index\n");
	} else if (mat1->n != mat2->m) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		rslt = (matrix *)malloc(sizeof(matrix));
		if (!rslt) {
			fprintf(stderr, "Result could not be loaded\n");
			return;
		}
		rslt->m = mat1->m;
		rslt->n = mat2->n;
		// dimensions for the multiplication
		m1 = mat1->m;
		n1 = mat1->n;
		n2 = mat2->n;
		rslt->mat = multiply_calc(mat1->mat, mat2->mat, m1, n1, n2);
		if (!rslt->mat) {
			fprintf(stderr, "Result could not be loaded\n");
			free(rslt);
			return;
		}
		// calculates the sum of the elemnts
		rslt->sum = 0;
		for (int i = 0; i < rslt->m; i++) {
			for (int j = 0; j < rslt->n; j++)
				rslt->sum = sum_modulo(rslt->sum, rslt->mat[i][j]);
		}
		// puts the matrix at the back of the list
		rslt->next = NULL;
		rslt->prev = *last;
		(*last)->next = rslt;
		*last = rslt;
	}
}

// determines the transpose of a given matrix
void transpose_matrix(matrix *first)
{
	int i, j;
	int index, nbr = 0;
	int **trans;
	matrix *crt = first;
	scanf("%d", &index);
	for (; crt && nbr < index; nbr++, crt = crt->next)
		;
	if (!crt || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		if (crt->m == crt->n) {
			// swaps the values above the main diagonal with the ones below
			for (i = 0; i < crt->m; i++) {
				for (j = i; j < crt->n; j++)
					swap_ints(&crt->mat[i][j], &crt->mat[j][i]);
			}
		} else {
			// makes a new matrix because there is no main diagonal
			trans = alloc_matrix(crt->n, crt->m);
			if (!trans) {
				fprintf(stderr, "Result could not be loaded\n");
				return;
			}
			for (i = 0; i < crt->m; i++) {
				for (j = 0; j < crt->n; j++)
					trans[j][i] = crt->mat[i][j];
			}
			free_simple_matrix(crt->mat, crt->m);
			swap_ints(&crt->m, &crt->n);
			crt->mat = trans;
		}
	}
}

// sorts the matrix list
void sort_list(matrix *first)
{
	matrix *crt_i, *crt_j;
	int **aux;
	for (crt_i = first; crt_i; crt_i = crt_i->next) {
		for (crt_j = crt_i; crt_j; crt_j = crt_j->next) {
			if (crt_i->sum > crt_j->sum) {
				// swap matrixes
				aux = crt_i->mat;
				crt_i->mat = crt_j->mat;
				crt_j->mat = aux;
				// swap the rest of the elements
				swap_ints(&crt_i->m, &crt_j->m);
				swap_ints(&crt_i->n, &crt_j->n);
				swap_ints(&crt_i->sum, &crt_j->sum);
			}
		}
	}
}

// exponentiates a given matrix
void exponetiate_matrix(matrix *first)
{
	int i, j, pow, index, nbr = 0, size;
	int **rslt, **del_rslt, **del_crt;
	matrix *crt = first;
	scanf("%d%d", &index, &pow);
	for (; crt && nbr < index; nbr++, crt = crt->next)
		;
	if (!crt || index < 0) {
		printf("No matrix with the given index\n");
	} else if (pow < 0) {
		printf("Power should be positive\n");
	} else if (crt->m != crt->n) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		// initializing the result with the identity matrix
		rslt = alloc_matrix(crt->m, crt->n);
		if (!rslt) {
			fprintf(stderr, "Result could not be loaded\n");
			return;
		}
		for (i = 0; i < crt->m; i++) {
			for (j = 0; j < crt->n; j++) {
				if (i == j)
					rslt[i][j] = 1;
				else
					rslt[i][j] = 0;
			}
		}
		del_rslt = rslt;
		// fast exponentiation algorithm
		// also frees the previous matrixes after calculating the next ones
		for (i = 0; (1 << i) <= pow; i++) {
			if (((1 << i) & pow) > 0) {
				rslt = multiply_calc(rslt, crt->mat, crt->m, crt->n, crt->n);
				free_simple_matrix(del_rslt, crt->m);
				del_rslt = rslt;
				if (!rslt) {
					fprintf(stderr, "Result could not be loaded\n");
					return;
				}
			}
			del_crt = crt->mat;
			size = crt->m;
			crt->mat = multiply_calc(crt->mat, crt->mat, size, size, size);
			free_simple_matrix(del_crt, crt->m);
			if (!crt->mat) {
				fprintf(stderr, "Result could not be loaded\n");
				return;
			}
		}
		del_crt = crt->mat;
		crt->mat = rslt;
		free_simple_matrix(del_crt, crt->m);
		crt->sum = 0;
		for (i = 0; i < crt->m; i++) {
			for (j = 0; j < crt->n; j++)
				crt->sum = sum_modulo(crt->sum, crt->mat[i][j]);
		}
	}
}
