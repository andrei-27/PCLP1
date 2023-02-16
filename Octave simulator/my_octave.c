// Ⓒ Pirlea Andrei 312CAa 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "functions.c"

int main(void)
{
	int ***a, *size_m, *size_n, index_cnt = -1;
	char operation;

	// the switch case is used to select the wanted operation
	do {
		scanf("%c", &operation);
		switch (operation) {
		case 'L':
			if (index_cnt < 0) {
				initialization(&a, &size_m, &size_n);
				index_cnt = 0;
			} else {
				load_matrix(&a, &size_m, &size_n, ++index_cnt);
			}
			break;
		case 'D':
			print_dimensions(size_m, size_n, index_cnt);
			break;
		case 'P':
			print_matrix(a, size_m, size_n, index_cnt);
			break;
		case 'C':
			resize_matrix(a, size_m, size_n, index_cnt);
			break;
		case 'T':
			transpose_matrix(a, size_m, size_n, index_cnt);
			break;
		case 'F':
			free_matrix(a, size_m, size_n, &index_cnt);
			break;
		case 'O':
			sort_matrix(a, size_m, size_n, index_cnt);
			break;
		case 'M':
			multiply_matrix(&a, &size_m, &size_n, &index_cnt);
			break;
		case 'R':
			pow_matrix(a, size_m, size_n, index_cnt);
			break;
		case 'S':
			strassen_multiply_matrix(&a, &size_m, &size_n, &index_cnt);
			break;
		/*
		we ignore spaces, EOLs and Qs in this switch because the do while exits
		whenever it gets a Q and we print "Unrecognized command" on the defaut
		case
		*/
		case ' ':
			break;
		case '\n':
			break;
		case 'Q':
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
	} while (operation != 'Q');

	/*
	if we reach this point in code we know that we just got an Q so we clear
	everything and finish the program
	*/
	clear_all(a, size_m, size_n, index_cnt);

	return 0;
}

