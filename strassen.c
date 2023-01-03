// Copyright 2022 Alexandru Alin-Ioan 312CA
#include <stdio.h>
#include <stdlib.h>
#include "my_octave_lib.h"
#define YES_SBTRCT -1
#define NO_SBTRCT 1
#define MOD 10007
#define SUBMAT_NBR 4
#define M_NBR 7

// calculates the sum of two matrixes if the subtract parameter is 1
// calculates the diffrence of two matrixes if the subtract parameter is -1
void sum_matrix(int **rslt, int **mat1, int **mat2, int n, int subtract)
{
	int i, j, to_add;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			rslt[i][j] = 0;
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			to_add = mat1[i][j] + subtract * mat2[i][j];
			rslt[i][j] = sum_modulo(rslt[i][j], to_add);
		}
	}
}

// gets the top two submateixes form an initial matrix
void get_top_submatrixes(int **mat, int **sub1, int **sub2, int n)
{
	int i, j;
	for (i = 0; i < n / 2; i++) {
		for (j = 0; j < n / 2; j++)
			sub1[i][j] = mat[i][j];
	}
	for (i = 0; i < n / 2; i++) {
		for (j = n / 2; j < n; j++)
			sub2[i][j - n / 2] = mat[i][j];
	}
}

// gets the bottom two submateixes form an initial matrix
void get_bottom_submatrixes(int **mat, int **sub1, int **sub2, int n)
{
	int i, j;
	for (i = n / 2; i < n; i++) {
		for (j = 0; j < n / 2; j++)
			sub1[i - n / 2][j] = mat[i][j];
	}
	for (i = n / 2; i < n; i++) {
		for (j = n / 2; j < n; j++)
			sub2[i - n / 2][j - n / 2] = mat[i][j];
	}
}

// puts the top two matrixes in the result matrix
void put_top_back(int **mat, int **sub1, int **sub2, int n)
{
	int i, j;
	for (i = 0; i < n / 2; i++) {
		for (j = 0; j < n / 2; j++)
			mat[i][j] = sub1[i][j];
	}
	for (i = 0; i < n / 2; i++) {
		for (j = n / 2; j < n; j++)
			mat[i][j] = sub2[i][j - n / 2];
	}
}

// puts the bottom two matrixes in the result matrix
void put_bottom_back(int **mat, int **sub1, int **sub2, int n)
{
	int i, j;
	for (i = n / 2; i < n; i++) {
		for (j = 0; j < n / 2; j++)
			mat[i][j] = sub1[i - n / 2][j];
	}
	for (i = n / 2; i < n; i++) {
		for (j = n / 2; j < n; j++)
			mat[i][j] = sub2[i - n / 2][j - n / 2];
	}
}

// frees the auxiliary matrixes used
void free_aux(int **m[M_NBR], int **a[SUBMAT_NBR], int **b[SUBMAT_NBR], int n)
{
	int i;
	for (i = 0; i < SUBMAT_NBR; i++)
		free_simple_matrix(a[i], n / 2);
	for (i = 0; i < SUBMAT_NBR; i++)
		free_simple_matrix(b[i], n / 2);
	for (i = 0; i < M_NBR; i++)
		free_simple_matrix(m[i], n / 2);
}

// calculates the M marixes with the formulas needed
void calc_m(int **m[M_NBR], int **a[SUBMAT_NBR], int **b[SUBMAT_NBR], int n,
			int **sum1, int **sum2)
{
		sum_matrix(sum1, a[0], a[3], n / 2, NO_SBTRCT);
		sum_matrix(sum2, b[0], b[3], n / 2, NO_SBTRCT);
		strassen(m[0], sum1, sum2, n / 2);

		sum_matrix(sum1, a[2], a[3], n / 2, NO_SBTRCT);
		strassen(m[1], sum1, b[0], n / 2);

		sum_matrix(sum1, b[1], b[3], n / 2, YES_SBTRCT);
		strassen(m[2], a[0], sum1, n / 2);

		sum_matrix(sum1, b[2], b[0], n / 2, YES_SBTRCT);
		strassen(m[3], a[3], sum1, n / 2);

		sum_matrix(sum1, a[0], a[1], n / 2, NO_SBTRCT);
		strassen(m[4], sum1, b[3], n / 2);

		sum_matrix(sum1, a[2], a[0], n / 2, YES_SBTRCT);
		sum_matrix(sum2, b[0], b[1], n / 2, NO_SBTRCT);
		strassen(m[5], sum1, sum2, n / 2);

		sum_matrix(sum1, a[1], a[3], n / 2, YES_SBTRCT);
		sum_matrix(sum2, b[2], b[3], n / 2, NO_SBTRCT);
		strassen(m[6], sum1, sum2, n / 2);
}

void strassen(int **rslt, int **mat1, int **mat2, int n)
{
	int i, j;
	int **m[M_NBR], **a[SUBMAT_NBR], **b[SUBMAT_NBR], **sum1, **sum2;
	if (n == 1) {
		rslt[0][0] = (mat1[0][0] * mat2[0][0]) % MOD;
		if (rslt[0][0] < 0)
			rslt[0][0] += MOD;
	} else {
		// alloc A matrixes and get them from the starting matrix
		for (i = 0; i < SUBMAT_NBR; i++) {
			a[i] = alloc_matrix(n / 2, n / 2);
			if (!a[i]) {
				for (j = i - 1; j >= 0; j--)
					free(a[i]);
				rslt = NULL;
				return;
			}
		}
		get_top_submatrixes(mat1, a[0], a[1], n);
		get_bottom_submatrixes(mat1, a[2], a[3], n);
		// alloc B matrixes and get them from the starting matrix
		for (i = 0; i < SUBMAT_NBR; i++) {
			b[i] = alloc_matrix(n / 2, n / 2);
			if (!b[i]) {
				for (j = 0; j <= SUBMAT_NBR; j++)
					free(a[i]);
				for (j = i - 1; j >= 0; j--)
					free(b[i]);
				rslt = NULL;
				return;
			}
		}
		get_top_submatrixes(mat2, b[0], b[1], n);
		get_bottom_submatrixes(mat2, b[2], b[3], n);
		// alloc M matrixes
		for (i = 0; i < M_NBR; i++) {
			m[i] = alloc_matrix(n / 2, n / 2);
			if (!b[i]) {
				for (j = 0; j <= SUBMAT_NBR; j++)
					free(a[i]);
				for (j = 0; j <= SUBMAT_NBR; j++)
					free(b[i]);
				for (j = i - 1; j >= 0; j--)
					free(m[i]);
				rslt = NULL;
				return;
			}
		}
		// alloc auxiliary matrixes for the sums calculated
		sum1 = alloc_matrix(n / 2, n / 2);
		sum2 = alloc_matrix(n / 2, n / 2);
		if (!sum1 || !sum2) {
			free_aux(m, a, b, n);
			rslt = NULL;
			return;
		}
		// calculate the M matrixes
		calc_m(m, a, b, n, sum1, sum2);
		// remake the result matrix
		for (i = 0; i < n / 2; i++)
			for (j = 0; j < n / 2; j++) {
				sum1[i][j] = m[0][i][j] + m[3][i][j];
				sum1[i][j] += m[6][i][j] - m[4][i][j];
				sum1[i][j] = sum_modulo(sum1[i][j], 0);
			}
		sum_matrix(sum2, m[2], m[4], n / 2, NO_SBTRCT);
		put_top_back(rslt, sum1, sum2, n);
		sum_matrix(sum1, m[1], m[3], n / 2, NO_SBTRCT);
		for (i = 0; i < n / 2; i++)
			for (j = 0; j < n / 2; j++) {
				sum2[i][j] = m[0][i][j] - m[1][i][j];
				sum2[i][j] += m[2][i][j] + m[5][i][j];
				sum2[i][j] = sum_modulo(sum2[i][j], 0);
			}
		put_bottom_back(rslt, sum1, sum2, n);
		// free auxiliaries
		free_aux(m, a, b, n);
		free_simple_matrix(sum1, n / 2);
		free_simple_matrix(sum2, n / 2);
	}
}

// initializations for the srassen algorithm
// gets the two matrixes that need to be multiplied,
// puts the result matrix at the end of the list
void strassen_setup(matrix *first, matrix **last)
{
	matrix *rslt, *mat1 = first, *mat2 = first;
	int index1, index2, nbr = 0;
	scanf("%d", &index1);
	scanf("%d", &index2);

	for (nbr = 0; mat1 && nbr < index1; nbr++, mat1 = mat1->next)
		;
	for (nbr = 0; mat2 && nbr < index2; nbr++, mat2 = mat2->next)
		;
	if (!mat1 || !mat2 || index1 < 0 || index2 < 0) {
		printf("No matrix with the given index\n");
	} else if (mat1->n != mat1->m || mat2->m != mat1->m) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		rslt = (matrix *)malloc(sizeof(matrix));
		rslt->m = mat1->m;
		rslt->n = mat2->n;
		rslt->mat = alloc_matrix(rslt->m, rslt->m);
		if (!rslt->mat) {
			fprintf(stderr, "Result could not be loaded\n");
			return;
		}
		strassen(rslt->mat, mat1->mat, mat2->mat, mat1->m);
		if (!rslt->mat) {
			fprintf(stderr, "Result could not be loaded\n");
			return;
		}
		// makes the sum for the elements
		rslt->sum = 0;
		for (int i = 0; i < rslt->m; i++) {
			for (int j = 0; j < rslt->n; j++)
				rslt->sum = sum_modulo(rslt->sum, rslt->mat[i][j]);
		}
		rslt->next = NULL;
		rslt->prev = *last;
		(*last)->next = rslt;
		*last = rslt;
	}
}
