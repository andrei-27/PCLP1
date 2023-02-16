// Ⓒ Pirlea Andrei 312CAa 2022-2023

#define MOD 10007

void error_and_exit(void);

void initialization(int ****a, int **size_m, int **size_n);

void load_matrix(int ****a, int **size_m, int **size_n, int index);

void print_matrix(int ***a, int *size_m, int *size_n, int mx);

void print_dimensions(int *size_m, int *size_n, int mx);

void resize_matrix(int ***a, int *size_m, int *size_n, int mx);

void transpose_matrix(int ***a, int *size_m, int *size_n, int mx);

void free_matrix(int ***a, int *size_m, int *size_n, int *mx);

void clear_all(int ***a, int *size_m, int *size_n, int mx);

void sort_matrix(int ***a, int *size_m, int *size_n, int mx);

void multiply_matrix(int ****a, int **size_m, int **size_n, int *mx);

void pow_matrix(int ***a, int *size_m, int *size_n, int mx);

void strassen_alloc(int ***m1, int ***m2, int ***m3, int ***m4, int dim);

void strassen_free(int ***m1, int ***m2, int ***m3, int ***m4, int dim);

void strassen_add(int **a, int **b, int dim, int **rez);

void strassen_subtract(int **a, int **b, int dim, int **rez);

void strassen(int **a, int **b, int dim, int **rez);

void strassen_multiply_matrix(int ****a, int **size_m, int **size_n, int *mx);

