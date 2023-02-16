// Ⓒ Pirlea Andrei 312CAa 2022-2023

#include <stdio.h>
#include <stdlib.h>
#define MOD 10007

/// @brief exits the program
/// @param
void error_and_exit(void)
{
	printf("Error at allocating memory\n");
	exit(-1);
}

/// @brief Creates the first matrix and the arrays for dimensions
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
void initialization(int ****a, int **size_m, int **size_n)
{
	int m, n;
	scanf("%d%d", &m, &n);

	// allocate memory for the first matrix
	(*a) = (int ***)malloc(sizeof(int **));
	if (!(*a)) {
		free((*a));
		error_and_exit();
	}

	(*a)[0] = (int **)malloc(m * sizeof(int *));
	if (!(*a)[0]) {
		free((*a)[0]);
		error_and_exit();
	}

	for (int i = 0 ; i < m ; ++i) {
		(*a)[0][i] = (int *)malloc(n * sizeof(int));
		if (!(*a)[0][i]) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free((*a)[0][cnt]);
			free((*a)[0]);
			free((*a));
			error_and_exit();
		}
	}

	// read the matrix
	for (int i = 0 ; i < m ; ++i)
		for (int j = 0 ; j < n ; ++j)
			scanf("%d", &(*a)[0][i][j]);

	// allocate memory for the arrays that hold the dimmensions of the matrices
	(*size_m) = (int *)malloc(sizeof(int));
	if (!(*size_m)) {
		free((*size_m));
		error_and_exit();
	}
	(*size_m)[0] = m;

	(*size_n) = (int *)malloc(sizeof(int));
	if (!(*size_n)) {
		free((*size_n));
		error_and_exit();
	}
	(*size_n)[0] = n;
}

/// @brief Allocate more space and load another matrix
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param index the index of the last matrix
void load_matrix(int ****a, int **size_m, int **size_n, int index)
{
	int m, n, new_size = index + 1;
	scanf("%d%d", &m, &n);

	// allocate memory for the new matrix
	int ***tmp = (int ***)realloc((*a), new_size * sizeof(int **));
	if (!tmp) {
		free(tmp);
		error_and_exit();
	}

	tmp[index] = (int **)malloc(m * sizeof(int *));
	if (!tmp[index]) {
		for (int cnt = 0 ; cnt < index ; ++cnt)
			free(tmp[cnt]);
		free(tmp);
		error_and_exit();
	}

	for (int i = 0 ; i < m ; ++i) {
		tmp[index][i] = (int *)malloc(n * sizeof(int));
		if (!tmp[index][i]) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free(tmp[index][cnt]);
			for (int cnt = 0 ; cnt < index ; ++cnt)
				free(tmp[cnt]);
			free(tmp);
			exit(-1);
		}
	}
	(*a) = tmp;

	// read the matrix
	for (int i = 0 ; i < m ; ++i)
		for (int j = 0 ; j < n ; ++j)
			scanf("%d", &(*a)[index][i][j]);

	// allocate memory for the arrays that hold the dimmensions of the matrices
	int *tmp_size_m = (int *)realloc((*size_m), new_size * sizeof(int));
	if (!tmp_size_m) {
		free(tmp_size_m);
		error_and_exit();
	}
	(*size_m) = tmp_size_m;
	(*size_m)[index] = m;

	int *tmp_size_n = (int *)realloc((*size_n), new_size * sizeof(int));
	if (!tmp_size_n) {
		free(tmp_size_n);
		error_and_exit();
	}
	(*size_n) = tmp_size_n;
	(*size_n)[index] = n;
}

/// @brief Print at STDOUT the matrix with the given index
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void print_matrix(int ***a, int *size_m, int *size_n, int mx)
{
	int index;
	scanf("%d", &index);

	if (index > mx || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	for (int i = 0 ; i < size_m[index] ; ++i) {
		for (int j = 0 ; j < size_n[index] ; ++j)
			printf("%d ", a[index][i][j]);
		printf("\n");
	}
}

/// @brief Print at STDOUT the dimensions of the matrix with the given index
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void print_dimensions(int *size_m, int *size_n, int mx)
{
	int index;
	scanf("%d", &index);

	if (index > mx || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	printf("%d %d\n", size_m[index], size_n[index]);
}

/// @brief Resize the matrix with the given index
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void resize_matrix(int ***a, int *size_m, int *size_n, int mx)
{
	int index, l, c, *lines, *columns;
	scanf("%d", &index);

	// read the nr. of lines then allocate the needed memory into an array
	scanf("%d", &l);
	lines = (int *)malloc(l * sizeof(int));
	for (int i = 0 ; i < l ; ++i)
		scanf("%d", &lines[i]);

	// read the nr. of columns then allocate the needed memory into an array
	scanf("%d", &c);
	columns = (int *)malloc(c * sizeof(int));
	for (int i = 0 ; i < c ; ++i)
		scanf("%d", &columns[i]);

	if (index > mx || index < 0) {
		printf("No matrix with the given index\n");
		free(lines);
		free(columns);
		return;
	}

	// allocate memory for the new matrix
	int **tmp = (int **)malloc(l * sizeof(int *));
	if (!tmp) {
		free(tmp);
		error_and_exit();
	}
	for (int i = 0 ; i < l ; ++i) {
		tmp[i] = (int *)malloc(c * sizeof(int));
		if (!tmp) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free(tmp[cnt]);
			free(tmp);
			error_and_exit();
		}
	}

	// create the new matrix
	int tmp_i = 0, tmp_j = 0;
	for (int i = 0 ; i < l ; ++i) {
		for (int j = 0 ; j < c ; ++j)
			tmp[tmp_i][tmp_j++] = a[index][lines[i]][columns[j]];
		tmp_i++;
		tmp_j = 0;
	}

	// clear the old matrix
	for (int i = 0 ; i < size_m[index] ; ++i)
		free(a[index][i]);
	free(a[index]);

	// put the resized matrix at the old index
	a[index] = tmp;
	size_m[index] = l;
	size_n[index] = c;

	// clear the arrays used
	free(lines);
	free(columns);
}

/// @brief Transpose the matrix with the given index
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void transpose_matrix(int ***a, int *size_m, int *size_n, int mx)
{
	int index;
	scanf("%d", &index);

	if (index > mx || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	// allocate a new matrix to construct the transpose
	int **tmp = (int **)malloc(size_n[index] * sizeof(int *));
	if (!tmp) {
		free(tmp);
		error_and_exit();
	}

	for (int i = 0 ; i < size_n[index] ; ++i) {
		tmp[i] = (int *)malloc(size_m[index] * sizeof(int));
		if (!tmp) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free(tmp[cnt]);
			free(tmp);
			error_and_exit();
		}
	}

	// transpose the matrix
	for (int i = 0 ; i < size_m[index] ; ++i)
		for (int j = 0 ; j < size_n[index] ; ++j)
			tmp[j][i] = a[index][i][j];

	// free matrix from index to be able to alocate the transposed matrix
	for (int i = 0 ; i < size_m[index] ; ++i)
		free(a[index][i]);
	free(a[index]);

	a[index] = tmp;

	// swap nr. of lines with nr. of colls of the transposed matrix
	size_m[index] += size_n[index];
	size_n[index] = size_m[index] - size_n[index];
	size_m[index] -= size_n[index];
}

/// @brief Free the matrix with the given index
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void free_matrix(int ***a, int *size_m, int *size_n, int *mx)
{
	int index;
	scanf("%d", &index);
	if (index > (*mx) || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	// free the matrix
	for (int i = 0 ; i < size_m[index] ; ++i)
		free(a[index][i]);
	free(a[index]);

	/*
	since the matrix at a[index] is clearead we shift all the matrices with
	indices greater than the cleared matrix one position to the left, including
	the arrays that hold the dimensions
	*/
	for (int i = index ; i < (*mx) ; ++i) {
		a[i] = a[i + 1];
		size_m[i] = size_m[i + 1];
		size_n[i] = size_n[i + 1];
	}

	if ((*mx) == 0 && index == 0) {
		free(a);
		free(size_m);
		free(size_n);
	}

	--(*mx);
}

/// @brief Free all the matrices
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void clear_all(int ***a, int *size_m, int *size_n, int mx)
{
	// we iterate through the 3D matrix to clear it
	if (mx >= 0) {
		while (mx >= 0) {
			for (int i = 0 ; i < size_m[mx] ; ++i)
				free(a[mx][i]);
			free(a[mx]);
			--mx;
		}

		free(a);
		free(size_m);
		free(size_n);
	}
}

/// @brief Sort the matrices based on the sum of the elements
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void sort_matrix(int ***a, int *size_m, int *size_n, int mx)
{
	mx++;
	// making an array that holds the sum of the elements of a matrix MOD 10007
	int *sum_array = (int *)calloc(mx, sizeof(int));
	if (!sum_array) {
		free(sum_array);
		error_and_exit();
	}

	for (int i = 0 ; i < mx ; ++i) {
		for (int j = 0 ; j < size_m[i] ; ++j)
			for (int k = 0 ; k < size_n[i] ; ++k)
				sum_array[i] = (sum_array[i] + a[i][j][k]) % MOD;
		if (sum_array[i] < 0)
			sum_array[i] += MOD;
	}

	/*
	sorting the matrices and the arrays that hold the dimensions based on
	the sum of the elements
	*/
	for (int i = 0 ; i < mx - 1 ; ++i)
		for (int j = i + 1 ; j < mx ; ++j)
			if (sum_array[i] > sum_array[j]) {
				int aux = sum_array[i];
				sum_array[i] = sum_array[j];
				sum_array[j] = aux;
				int **p = a[i];
				a[i] = a[j];
				a[j] = p;
				aux = size_m[i];
				size_m[i] = size_m[j];
				size_m[j] = aux;
				aux = size_n[i];
				size_n[i] = size_n[j];
				size_n[j] = aux;
			}
	free(sum_array);
}

/*
We generate a new matrix to calculate the result and do mod 10007 at every
element in order to avoid the overflow. When we fill in the new allocated
space (that was allocated using tmp) we add 10007 if the number is smaller
than 0. Also we increase the size of the arrays that hold the dimensions.
*/
/// @brief Multiply 2 matrices
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void multiply_matrix(int ****a, int **size_m, int **size_n, int *mx)
{
	int matrix1, matrix2;
	scanf("%d%d", &matrix1, &matrix2);
	if (matrix1 > (*mx) || matrix2 > (*mx) || matrix1 < 0 || matrix2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*size_n)[matrix1] != (*size_m)[matrix2]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	int **mat = (int **)calloc((*size_m)[matrix1], sizeof(int *));
	if (!mat) {
		free(mat);
		error_and_exit();
	}
	for (int i = 0 ; i < (*size_m)[matrix1] ; ++i) {
		mat[i] = (int *)calloc((*size_n)[matrix2], sizeof(int));
		if (!mat) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free(mat[cnt]);
			free(mat);
			error_and_exit();
		}
	}
	for (int i = 0 ; i < (*size_m)[matrix1] ; ++i)
		for (int j = 0 ; j < (*size_n)[matrix2] ; ++j)
			for (int k = 0 ; k < (*size_n)[matrix1] ; ++k) {
				mat[i][j] += (*a)[matrix1][i][k] * (*a)[matrix2][k][j];
				mat[i][j] %= MOD;
			}
	int new_size = (*mx) + 2, m = (*size_m)[matrix1], n = (*size_n)[matrix2];
	int index = (*mx) + 1;
	int ***tmp = (int ***)realloc((*a), new_size * sizeof(int **));
	if (!tmp) {
		free(tmp);
		error_and_exit();
	}
	tmp[index] = (int **)malloc(m * sizeof(int *));
	if (!tmp[index]) {
		for (int cnt = 0 ; cnt < index ; ++cnt)
			free(tmp[cnt]);
		free(tmp);
		error_and_exit();
	}
	for (int i = 0 ; i < m ; ++i) {
		tmp[index][i] = (int *)malloc(n * sizeof(int));
		if (!tmp[index][i]) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free(tmp[index][cnt]);
			for (int cnt = 0 ; cnt < index ; ++cnt)
				free(tmp[cnt]);
			free(tmp);
			error_and_exit();
		}
	}
	(*a) = tmp;
	for (int i = 0 ; i < m ; ++i)
		for (int j = 0 ; j < n ; ++j) {
			(*a)[index][i][j] = mat[i][j] % MOD;
			if ((*a)[index][i][j] < 0)
				(*a)[index][i][j] += MOD;
		}
	int *tmp_size_m = (int *)realloc((*size_m), new_size * sizeof(int));
	int *tmp_size_n = (int *)realloc((*size_n), new_size * sizeof(int));
	if (!tmp_size_m || !tmp_size_n) {
		free(tmp_size_m);
		free(tmp_size_n);
		error_and_exit();
	}
	(*size_m) = tmp_size_m;
	(*size_m)[index] = m;
	(*size_n) = tmp_size_n;
	(*size_n)[index] = n;

	for (int i = 0 ; i < (*size_m)[matrix1] ; ++i)
		free(mat[i]);
	free(mat);
	++(*mx);
}

/*
We generate two new matrices to calculate the result and do mod 10007 at every
element in order to avoid the overflow. The recursive algorithm halves the
power at every step and allows us to achieve a time complexity of O(log2(N)).
Because this operation is in-place we put the result matrix in the 3D matrix at
the old index and we add 10007 if the number is smaller than 0.
*/
/// @brief Rise the matrix with the given index to the given power
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void pow_matrix(int ***a, int *size_m, int *size_n, int mx)
{
	int index, power, sum = 0;
	scanf("%d%d", &index, &power);
	if (index > mx || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if (power < 0) {
		printf("Power should be positive\n");
		return;
	}
	if (size_m[index] != size_n[index]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	int **tmp = (int **)malloc(size_n[index] * sizeof(int *));
	int **tmp2 = (int **)calloc(size_n[index], sizeof(int *));
	if (!tmp || !tmp2) {
		free(tmp);
		free(tmp2);
		error_and_exit();
	}
	for (int i = 0 ; i < size_n[index] ; ++i) {
		tmp[i] = (int *)malloc(size_m[index] * sizeof(int));
		tmp2[i] = (int *)calloc(size_m[index], sizeof(int));
		if (!tmp || !tmp2) {
			for (int cnt = 0 ; cnt < i ; ++cnt) {
				free(tmp[cnt]);
				free(tmp2[cnt]);
			}
			free(tmp);
			free(tmp2);
			error_and_exit();
		}
	}
	for (int i = 0 ; i < size_m[index] ; ++i)
		for (int j = 0 ; j < size_n[index] ; ++j)
			tmp[i][j] = a[index][i][j];
	for (int i = 0 ; i < size_m[index] ; ++i)
		tmp2[i][i] = 1;
	while (power > 0) {
		if (power % 2) {
			for (int i = 0 ; i < size_m[index] ; ++i)
				for (int j = 0 ; j < size_m[index] ; ++j) {
					sum = 0;
					for (int k = 0 ; k < size_m[index] ; ++k)
						sum += (tmp2[i][k] * tmp[k][j]) % MOD;
					a[index][i][j] = sum % MOD;
				}

			for (int i = 0 ; i < size_m[index] ; ++i)
				for (int j = 0 ; j < size_m[index] ; ++j)
					tmp2[i][j] = a[index][i][j];
		}
		for (int i = 0 ; i < size_m[index] ; ++i)
			for (int j = 0 ; j < size_m[index] ; ++j) {
				sum = 0;
				for (int k = 0 ; k < size_m[index] ; ++k)
					sum += (tmp[i][k] * tmp[k][j]) % MOD;
				a[index][i][j] = sum % MOD;
			}
		for (int i = 0 ; i < size_m[index] ; ++i)
			for (int j = 0 ; j < size_m[index] ; ++j)
				tmp[i][j] = a[index][i][j];
		power >>= 1;
	}
	for (int i = 0 ; i < size_m[index] ; ++i)
		for (int j = 0 ; j < size_n[index] ; ++j) {
			a[index][i][j] = tmp2[i][j] % MOD;
			if (a[index][i][j] < 0)
				a[index][i][j] += MOD;
		}
	for (int i = 0 ; i < size_m[index] ; ++i) {
		free(tmp[i]);
		free(tmp2[i]);
	}
	free(tmp);
	free(tmp2);
}

/*
We use this function to modularize the code and make it easier to read.
*/
/// @brief Alocate space for 4 matrices
/// @param m1
/// @param m2
/// @param m3
/// @param m4
/// @param dim dimension of matrices
void strassen_alloc(int ***m1, int ***m2, int ***m3, int ***m4, int dim)
{
	(*m1) = (int **)malloc(dim * sizeof(int *));
	if (!(*m1)) {
		free((*m1));
		error_and_exit();
	}
	for (int i = 0 ; i < dim ; ++i) {
		(*m1)[i] = (int *)malloc(dim * sizeof(int));
		if (!(*m1)) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free((*m1)[cnt]);
			free((*m1));
			error_and_exit();
		}
	}

	(*m2) = (int **)malloc(dim * sizeof(int *));
	if (!(*m2)) {
		free((*m2));
		error_and_exit();
	}
	for (int i = 0 ; i < dim ; ++i) {
		(*m2)[i] = (int *)malloc(dim * sizeof(int));
		if (!(*m2)) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free((*m2)[cnt]);
			free((*m2));
			error_and_exit();
		}
	}

	(*m3) = (int **)malloc(dim * sizeof(int *));
	if (!(*m3)) {
		free((*m3));
		error_and_exit();
	}
	for (int i = 0 ; i < dim ; ++i) {
		(*m3)[i] = (int *)malloc(dim * sizeof(int));
		if (!(*m3)) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free((*m3)[cnt]);
			free((*m3));
			error_and_exit();
		}
	}

	(*m4) = (int **)malloc(dim * sizeof(int *));
	if (!(*m4)) {
		free((*m4));
		error_and_exit();
	}
	for (int i = 0 ; i < dim ; ++i) {
		(*m4)[i] = (int *)malloc(dim * sizeof(int));
		if (!(*m4)) {
			for (int cnt = 0 ; cnt < i ; ++cnt)
				free((*m4)[cnt]);
			free((*m4));
			error_and_exit();
		}
	}
}

/*
We use this function to modularize the code and make it easier to read.
*/
/// @brief Free the space ocupied by 4 matrices
/// @param m1
/// @param m2
/// @param m3
/// @param m4
/// @param dim dimension of matrices
void strassen_free(int ***m1, int ***m2, int ***m3, int ***m4, int dim)
{
	for (int i = 0 ; i < dim ; ++i) {
		free((*m1)[i]);
		free((*m2)[i]);
		free((*m3)[i]);
		free((*m4)[i]);
	}
	free((*m1));
	free((*m2));
	free((*m3));
	free((*m4));
}

/*
We use this function to modularize the code and make it easier to read.
*/
/// @brief Addition of 2 matrices
/// @param a 1st matrix
/// @param b 2nd matrix
/// @param dim dimension of matrices
/// @param rez result matrix
void strassen_add(int **a, int **b, int dim, int **rez)
{
	for (int i = 0 ; i < dim ; ++i)
		for (int j = 0 ; j < dim ; ++j)
			rez[i][j] = a[i][j] + b[i][j];
}

/*
We use this function to modularize the code and make it easier to read.
*/
/// @brief Subtraction of 2 matrices
/// @param a 1st matrix
/// @param b 2nd matrix
/// @param dim dimension of matrices
/// @param rez result matrix
void strassen_subtract(int **a, int **b, int dim, int **rez)
{
	for (int i = 0 ; i < dim ; ++i)
		for (int j = 0 ; j < dim ; ++j)
			rez[i][j] = a[i][j] - b[i][j];
}

/*
This function does the math needed in order to get a time complexity of
O(N^log2(7)) instead of O(N^3) when multiplying 2 matrices. It follows the
Strassen algorithm.
*/
/// @brief The recursive part of Strassen algorithm
/// @param a 1st matrix
/// @param b 2nd matrix
/// @param dim dimension of matrices
/// @param rez result matrix
void strassen(int **a, int **b, int dim, int **rez)
{
	if (dim == 1) {
		rez[0][0] = a[0][0] * b[0][0];
	} else {
		// alocam memorie pentru matricele necesare
		int new_n = dim / 2, **m1, **m2, **m3, **m4, **m5, **m6, **m7, **m8;
		int **a11, **a12, **a21, **a22, **b11, **b12, **b21, **b22;
		strassen_alloc(&a11, &a12, &a21, &a22, new_n);
		strassen_alloc(&b11, &b12, &b21, &b22, new_n);
		strassen_alloc(&m1, &m2, &m3, &m4, new_n);
		strassen_alloc(&m5, &m6, &m7, &m8, new_n);

		// construim cele 8 matrice
		for (int i = 0 ; i < new_n ; ++i)
			for (int j = 0 ; j < new_n ; ++j) {
				a11[i][j] = a[i][j];
				a12[i][j] = a[i][j + new_n];
				a21[i][j] = a[i + new_n][j];
				a22[i][j] = a[i + new_n][j + new_n];
				b11[i][j] = b[i][j];
				b12[i][j] = b[i][j + new_n];
				b21[i][j] = b[i + new_n][j];
				b22[i][j] = b[i + new_n][j + new_n];
			}

		// folosim matrice temporare pentru a calcula cele 4 zone
		int **tmp1, **tmp2, **tmp3, **tmp4;
		strassen_alloc(&tmp1, &tmp2, &tmp3, &tmp4, new_n);
		strassen_add(a11, a22, new_n, tmp1);
		strassen_add(b11, b22, new_n, tmp2);
		strassen(tmp1, tmp2, new_n, m1);
		strassen_add(a21, a22, new_n, tmp1);
		strassen(tmp1, b11, new_n, m2);
		strassen_subtract(b12, b22, new_n, tmp1);
		strassen(a11, tmp1, new_n, m3);
		strassen_subtract(b21, b11, new_n, tmp1);
		strassen(a22, tmp1, new_n, m4);
		strassen_add(a11, a12, new_n, tmp1);
		strassen(tmp1, b22, new_n, m5);
		strassen_subtract(a21, a11, new_n, tmp1);
		strassen_add(b11, b12, new_n, tmp2);
		strassen(tmp1, tmp2, new_n, m6);
		strassen_subtract(a12, a22, new_n, tmp1);
		strassen_add(b21, b22, new_n, tmp2);
		strassen(tmp1, tmp2, new_n, m7);

		// aflam cele 4 chenare, refolosind matrice alocate anterior
		strassen_add(m1, m7, new_n, tmp1);
		strassen_subtract(m4, m5, new_n, tmp2);
		strassen_add(tmp1, tmp2, new_n, a11); // rez 11
		strassen_add(m3, m5, new_n, a12); // rez 12
		strassen_add(m2, m4, new_n, a21); // rez 21
		strassen_add(m3, m6, new_n, tmp1);
		strassen_subtract(m1, m2, new_n, tmp2);
		strassen_add(tmp1, tmp2, new_n, a22); // rez 22

		// construim matricea rezultat
		for (int i = 0 ; i < dim ; ++i)
			for (int j = 0 ; j < dim ; ++j) {
				if (i < new_n && j < new_n)
					rez[i][j] = a11[i][j];
				if (i >= new_n && j < new_n)
					rez[i][j] = a21[i - new_n][j];
				if (i < new_n && j >= new_n)
					rez[i][j] = a12[i][j - new_n];
				if (i >= new_n && j >= new_n)
					rez[i][j] = a22[i - new_n][j - new_n];
			}

		// eliberam memoria
		strassen_free(&a11, &a12, &a21, &a22, new_n);
		strassen_free(&b11, &b12, &b21, &b22, new_n);
		strassen_free(&tmp1, &tmp2, &tmp3, &tmp4, new_n);
		strassen_free(&m1, &m2, &m3, &m4, new_n);
		strassen_free(&m5, &m6, &m7, &m8, new_n);
	}
}

/*
We generate 3 new matrices for the 1st, 2nd and the result matrix. After
calling the recursive function called strassen, we increase the size of the 3D
matrix and place the result on the last position. When we fill in the new
allocated space (that was allocated using tmp) we add 10007 if the number is
smaller than 0. Also we increase the size of the arrays that hold the
dimensions.
*/
/// @brief Multiply 2 matrices using Strassen algorithm
/// @param a 3D matrix
/// @param size_m array for nr. of rows
/// @param size_n array for nr. of columns
/// @param mx the index of the last matrix
void strassen_multiply_matrix(int ****a, int **size_m, int **size_n, int *mx)
{
	int matrix1, matrix2, n;
	scanf("%d%d", &matrix1, &matrix2);
	if (matrix1 > (*mx) || matrix2 > (*mx) || matrix1 < 0 || matrix2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	n = (*size_n)[matrix1];
	if ((*size_n)[matrix1] != (*size_m)[matrix2]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	};
	int **rez = (int **)malloc(n * sizeof(int *));
	int **mat1 = (int **)malloc(n * sizeof(int *));
	int **mat2 = (int **)malloc(n * sizeof(int *));
	if (!rez || !mat1 || !mat2) {
		free(rez);
		free(mat1);
		free(mat2);
		error_and_exit();
	}
	for (int i = 0 ; i < n ; ++i) {
		rez[i] = (int *)malloc(n * sizeof(int));
		mat1[i] = (int *)malloc(n * sizeof(int));
		mat2[i] = (int *)malloc(n * sizeof(int));
		if (!rez[i] || !mat1[i] || !mat2[i]) {
			for (int cnt = 0 ; cnt < i ; ++cnt) {
				free(mat1[cnt]);
				free(mat2[cnt]);
				free(rez[cnt]);
			}
			free(rez);
			free(mat1);
			free(mat2);
			error_and_exit();
		}
	}
	for (int i = 0 ; i < n ; ++i)
		for (int j = 0 ; j < n ; ++j) {
			mat1[i][j] = (*a)[matrix1][i][j];
			mat2[i][j] = (*a)[matrix2][i][j];
		}
	int new_size = (*mx) + 2, index = (*mx) + 1;
	int ***tmp = (int ***)realloc((*a), new_size * sizeof(int **));
	if (!tmp) {
		free(tmp);
		error_and_exit();
	}
	(*a) = tmp;
	strassen(mat1, mat2, n, rez);
	for (int i = 0 ; i < n ; ++i)
		for (int j = 0 ; j < n ; ++j) {
			rez[i][j] %= MOD;
			if (rez[i][j] < 0)
				rez[i][j] += MOD;
		}

	for (int i = 0 ; i < n ; ++i) {
		free(mat1[i]);
		free(mat2[i]);
	}
	free(mat1);
	free(mat2);
	(*a)[index] = rez;
	int *tmp_size_m = (int *)realloc((*size_m), new_size * sizeof(int));
	int *tmp_size_n = (int *)realloc((*size_n), new_size * sizeof(int));
	if (!tmp_size_m || !tmp_size_n) {
		free(tmp_size_m);
		free(tmp_size_n);
		error_and_exit();
	}
	(*size_m) = tmp_size_m;
	(*size_m)[index] = n;
	(*size_n) = tmp_size_n;
	(*size_n)[index] = n;

	++(*mx);
}

