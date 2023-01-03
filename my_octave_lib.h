// Copyright 2022 Alexandru Alin-Ioan 312CA
#ifndef my_octave_lib
	#define my_octave_lib

// struct for the matrixes
typedef struct struct_matrix {
	int m, n, sum;
	int **mat;
	struct struct_matrix *next, *prev;
} matrix;

// load_matrix functions
int sum_modulo(int sum, int to_add);

void load_matrix(matrix **first, matrix **last);

int **alloc_matrix(int m, int n);

void matrix_init(matrix **crt);

// matrix_operations functions
void print_matrix(matrix *first);

void print_dimensions(matrix *first);

void resize_matrix(matrix *first);

void multiply_matrixes(matrix *first, matrix **last);

void transpose_matrix(matrix *first);

void sort_list(matrix *first);

void exponetiate_matrix(matrix *first);

// strassne functions
void strassen(int **rslt, int **mat1, int **mat2, int n);

void strassen_setup(matrix *first, matrix **last);

// free_memory functions
void free_matrix(matrix **first, matrix **last);

void free_simple_matrix(int **a, int m);

void free_list(matrix **first, matrix **last);

#endif
