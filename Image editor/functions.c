// Ⓒ Pirlea Andrei 312CAa 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "functions.h"
#define NMAX 1000

double clamp(double x, double min_val, double max_val)
{
	double rez = x < min_val ? min_val : x;
	return rez > max_val ? max_val : rez;
}

void clear_image(image_t *img)
{
	for (int i = 0 ; i < img->height ; ++i)
		free(img->val[i]);
	free(img->val);
	free(img);
}

void load_P2(image_t *img, char filename[], int prev)
{
	int gray;
	FILE *in = fopen(filename, "rt");
	if (!in)
		return;

	fseek(in, prev, SEEK_SET);
	for (int i = 0 ; i < img->height ; ++i)
		for (int j = 0 ; j < img->width ; ++j) {
			fscanf(in, "%d", &gray);
			img->val[i][j].g = (double)gray;
		}
	fclose(in);
}

void load_P3(image_t *img, char filename[], int prev)
{
	int r, g, b;
	FILE *in = fopen(filename, "rt");
	if (!in)
		return;

	fseek(in, prev, SEEK_SET);
	for (int i = 0 ; i < img->height ; ++i)
		for (int j = 0 ; j < img->width ; ++j) {
			fscanf(in, "%d %d %d", &r, &g, &b);
			img->val[i][j].r = (double)r;
			img->val[i][j].g = (double)g;
			img->val[i][j].b = (double)b;
		}
	fclose(in);
}

void load_P5(image_t *img, char filename[], int prev)
{
	unsigned char gray;
	FILE *in = fopen(filename, "rb");
	if (!in)
		return;

	fseek(in, prev, SEEK_SET);
	for (int i = 0 ; i < img->height ; ++i)
		for (int j = 0 ; j < img->width ; ++j) {
			fread(&gray, sizeof(char), 1, in);
			img->val[i][j].g = (double)gray;
		}
	fclose(in);
}

void load_P6(image_t *img, char filename[], int prev)
{
	unsigned char r, g, b;
	FILE *in = fopen(filename, "rb");
	if (!in)
		return;

	fseek(in, prev, SEEK_SET);
	for (int i = 0 ; i < img->height ; ++i)
		for (int j = 0 ; j < img->width ; ++j) {
			fread(&r, sizeof(char), 1, in);
			fread(&g, sizeof(char), 1, in);
			fread(&b, sizeof(char), 1, in);
			img->val[i][j].r = (double)r;
			img->val[i][j].g = (double)g;
			img->val[i][j].b = (double)b;
		}
	fclose(in);
}

int load(image_t **img, char filename[])
{
	char line[NMAX], c, type[3];
	int cnt = 0, prev = 0, a;
	FILE *in = fopen(filename, "rt");
	if (!in) {
		(*img) = NULL;
		return -1;
	}

	fscanf(in, "%s", type);
	type[2] = '\0';
	if (type[0] != 'P') {
		(*img) = NULL;
		return -1;
	}

	(*img) = (image_t *)malloc(sizeof(image_t));
	strcpy((*img)->type, type);
	fseek(in, 1, SEEK_CUR);
	while (cnt < 3) {
		a = 0;
		c = ' ';
		prev = ftell(in);
		fseek(in, prev, SEEK_SET);
		while (c != '\n' && a < 99) {
			fscanf(in, "%c", &c);
			line[a++] = c;
		}
		line[a - 1] = '\0';
		if (line[0] == '#')
			continue;
		if (cnt == 0) {
			fseek(in, prev, SEEK_SET);
			fscanf(in, "%d %d", &(*img)->width, &(*img)->height);
			(*img)->y2 = (*img)->height;
			(*img)->x2 = (*img)->width;
			(*img)->x1 = 0;
			(*img)->y1 = 0;
		}
		if (cnt == 1) {
			fseek(in, prev, SEEK_SET);
			fscanf(in, "%d", &(*img)->max_val);
		}
		if (cnt == 2)
			fseek(in, prev, SEEK_SET);
		fseek(in, 1, SEEK_CUR);
		++cnt;
	}
	fseek(in, 1, SEEK_CUR);
	(*img)->val = (rgb_t **)malloc((*img)->height * sizeof(rgb_t *));

	if (!(*img)->val)
		return -1;
	for (int i = 0 ; i < (*img)->height ; ++i) {
		(*img)->val[i] = (rgb_t *)malloc((*img)->width * sizeof(rgb_t));
		if (!(*img)->val[i]) {
			for (int j = 0 ; j < i ; j++)
				free((*img)->val[i]);
			free((*img)->val);
			return -1;
		}
	}
	fclose(in);
	if (!strcmp(type, "P2"))
		load_P2((*img), filename, prev);
	if (!strcmp(type, "P3"))
		load_P3((*img), filename, prev);
	if (!strcmp(type, "P5"))
		load_P5((*img), filename, prev);
	if (!strcmp(type, "P6"))
		load_P6((*img), filename, prev);
	return 0;
}

void select_area(image_t *img, char *p)
{
	if (!(img)) {
		printf("No image loaded\n");
		return;
	}
	p = strtok(NULL, " ");
	if (!strcmp(p, "ALL")) {
		printf("Selected ALL\n");
	} else {
		int ok = 1, nr = 0, x1, x2, y1, y2, v[4];
		for (int i = 0 ; i < 4 && p ; ++i) {
			if (strlen(p) == 1) {
				if (!isdigit(p[0]))
					ok = 0;
			} else {
				for (size_t i = 0 ; i < strlen(p) ; ++i) {
					if (i == 0 && isdigit(p[i + 1]) && p[0] == '-')
						continue;
					if (!isdigit(p[i]))
						ok = 0;
				}
			}
			if (ok) {
				v[nr] = atoi(p);
				nr++;
			}
			ok = 1;
			p = strtok(NULL, " ");
		}

		if (nr == 4) {
			x1 = v[0];
			y1 = v[1];
			x2 = v[2];
			y2 = v[3];
			if (p) {
				printf("Invalid command\n");
			} else {
				if (x1 > x2) {
					x1 += x2;
					x2 = x1 - x2;
					x1 -= x2;
				}
				if (y1 > y2) {
					y1 += y2;
					y2 = y1 - y2;
					y1 -= y2;
				}
				if (x1 == x2 || y1 == y2 || y1 < 0 || x1 < 0) {
					printf("Invalid set of coordinates\n");
					return;
				}
				if (x2 > img->width || y2 > img->height) {
					printf("Invalid set of coordinates\n");
					return;
				}
				img->x1 = x1;
				img->x2 = x2;
				img->y1 = y1;
				img->y2 = y2;
				printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
			}
		} else {
			printf("Invalid command\n");
		}
	}
}

void crop(image_t **img)
{
	if ((*img) == NULL) {
		printf("No image loaded\n");
		return;
	}
	image_t *tmp = (image_t *)malloc(sizeof(image_t));
	strcpy(tmp->type, (*img)->type);
	tmp->height = (*img)->y2 - (*img)->y1;
	tmp->width = (*img)->x2 - (*img)->x1;
	tmp->y2 = tmp->height;
	tmp->x2 = tmp->width;
	tmp->y1 = 0;
	tmp->x1 = 0;
	tmp->max_val = (*img)->max_val;

	tmp->val = (rgb_t **)malloc(tmp->height * sizeof(rgb_t *));
	if (!tmp->val)
		return;
	for (int i = 0 ; i < tmp->height ; ++i) {
		tmp->val[i] = (rgb_t *)malloc(tmp->width * sizeof(rgb_t));
		if (!tmp->val[i]) {
			for (int j = 0 ; j < i ; j++)
				free(tmp->val[i]);
			free(tmp->val);
			return;
		}
	}
	for (int i = 0 ; i < tmp->height ; ++i)
		for (int j = 0 ; j < tmp->width ; ++j)
			tmp->val[i][j] = (*img)->val[i + (*img)->y1][j + (*img)->x1];

	clear_image(*img);
	(*img) = tmp;
	printf("Image cropped\n");
}

void equalize(image_t *img)
{
	if (!img) {
		printf("No image loaded\n");
		return;
	}
	if (!strcmp(img->type, "P2") || !strcmp(img->type, "P5")) {
		int *freq = (int *)calloc(img->max_val + 1, sizeof(int));
		if (!freq)
			return;
		int partial_sum = 0;

		for (int i = 0 ; i < img->height ; ++i)
			for (int j = 0 ; j < img->width ; ++j)
				++freq[(int)img->val[i][j].g];

		for (int i = 0 ; i < img->height ; ++i)
			for (int j = 0 ; j < img->width ; ++j) {
				for (int k = 0 ; k <= (int)img->val[i][j].g ; ++k)
					partial_sum += freq[k];
				img->val[i][j].g = 255.0 * partial_sum;
				img->val[i][j].g /= (img->height * img->width);
				img->val[i][j].g = clamp(round(img->val[i][j].g), 0, 255);
				partial_sum = 0;
			}
		free(freq);
		printf("Equalize done\n");
	} else {
		printf("Black and white image needed\n");
	}
}

void histogram(image_t *img, char *p)
{
	if (!img) {
		printf("No image loaded\n");
		return;
	}
	if (p) {
		int ok = 1, nr = 0, x, y;
		p = strtok(NULL, " ");
		if (p) {
			for (size_t i = 0 ; i < strlen(p) ; ++i) {
				if (!isdigit(p[i]))
					ok = 0;
			}
			if (ok) {
				++nr;
				x = atoi(p);
				p = strtok(NULL, " ");
			}
			ok = 1;
		}
		if (p) {
			for (size_t i = 0 ; i < strlen(p) ; ++i) {
				if (!isdigit(p[i]))
					ok = 0;
			}
			if (ok) {
				++nr;
				y = atoi(p);
				p = strtok(NULL, " ");
			}
			ok = 1;
		}
		if (nr != 2 || (img->max_val + 1) % y != 0 || p) {
			printf("Invalid command\n");
			return;
		}

		if (!strcmp(img->type, "P2") || !strcmp(img->type, "P5")) {
			int *freq = (int *)calloc(img->max_val + 1, sizeof(int));
			if (!freq)
				return;
			int partial_sum = 0, step = (img->max_val + 1) / y, max_hist = 0;

			for (int i = img->y1 ; i < img->y2 ; ++i)
				for (int j = img->x1 ; j < img->x2 ; ++j)
					++freq[(int)img->val[i][j].g];

			for (int i = 0 ; i < img->max_val + 1 ; i += step) {
				for (int j = i ; j < i + step ; ++j)
					partial_sum += freq[j];
				if (max_hist < partial_sum)
					max_hist = partial_sum;
				partial_sum = 0;
			}

			for (int i = 0 ; i < img->max_val + 1 ; i += step) {
				for (int j = i ; j < i + step ; ++j)
					partial_sum += freq[j];
				printf("%d\t|\t", partial_sum * x / max_hist);
				for (int j = 0 ; j < partial_sum * x / max_hist ; ++j)
					printf("*");
				printf("\n");
				partial_sum = 0;
			}
			free(freq);
		} else {
			printf("Black and white image needed\n");
		}

	} else {
		printf("Invalid command\n");
	}
}

rgb_t apply_filter(image_t *img, int x, int y, int filter)
{
	rgb_t result;
	result.r = 0.0;
	result.g = 0.0;
	result.b = 0.0;
	if (x <= 0 || x >= img->height - 1 || y <= 0 || y >= img->width - 1) {
		result.r = img->val[x][y].r;
		result.g = img->val[x][y].g;
		result.b = img->val[x][y].b;
		return result;
	}
	if (filter == 0) {
		for (int i = x - 1 ; i <= x + 1 ; ++i)
			for (int j = y - 1 ; j <= y + 1 ; ++j) {
				result.r -= img->val[i][j].r;
				result.g -= img->val[i][j].g;
				result.b -= img->val[i][j].b;
			}
		result.r += 9 * img->val[x][y].r;
		result.g += 9 * img->val[x][y].g;
		result.b += 9 * img->val[x][y].b;
		result.r = clamp(result.r, 0, 255);
		result.g = clamp(result.g, 0, 255);
		result.b = clamp(result.b, 0, 255);
	}
	if (filter == 1) {
		for (int i = x - 1 ; i < x + 2 ; ++i)
			for (int j = y - 1 ; j < y + 2 ; ++j) {
				if (i == x || j == y) {
					result.r -= img->val[i][j].r;
					result.g -= img->val[i][j].g;
					result.b -= img->val[i][j].b;
				}
			}
		result.r += 6 * img->val[x][y].r;
		result.g += 6 * img->val[x][y].g;
		result.b += 6 * img->val[x][y].b;
		result.r = clamp(result.r, 0, 255);
		result.g = clamp(result.g, 0, 255);
		result.b = clamp(result.b, 0, 255);
	}
	if (filter == 2) {
		for (int i = x - 1 ; i < x + 2 ; ++i)
			for (int j = y - 1 ; j < y + 2 ; ++j) {
				result.r += img->val[i][j].r;
				result.g += img->val[i][j].g;
				result.b += img->val[i][j].b;
			}
		result.r /= 9;
		result.g /= 9;
		result.b /= 9;
	}
	if (filter == 3) {
		for (int i = x - 1 ; i < x + 2 ; ++i)
			for (int j = y - 1 ; j < y + 2 ; ++j) {
				if (i == x || j == y) {
					result.r += 2 * img->val[i][j].r;
					result.g += 2 * img->val[i][j].g;
					result.b += 2 * img->val[i][j].b;
				} else {
					result.r += img->val[i][j].r;
					result.g += img->val[i][j].g;
					result.b += img->val[i][j].b;
				}
			}
		result.r += 2 * img->val[x][y].r;
		result.g += 2 * img->val[x][y].g;
		result.b += 2 * img->val[x][y].b;
		result.r /= 16;
		result.g /= 16;
		result.b /= 16;
	}
	return result;
}

void filter(image_t **img, char *p)
{
	p = strtok(NULL, " ");
	if (!(*img)) {
		printf("No image loaded\n");
		return;
	}
	if (!p) {
		printf("Invalid command\n");
		return;
	}
	if (!strcmp((*img)->type, "P2") || !strcmp((*img)->type, "P5")) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	image_t *tmp = (image_t *)malloc(sizeof(image_t));
	strcpy(tmp->type, (*img)->type);
	tmp->height = (*img)->height;
	tmp->width = (*img)->width;
	tmp->y2 = (*img)->y2;
	tmp->x2 = (*img)->x2;
	tmp->y1 = (*img)->y1;
	tmp->x1 = (*img)->x1;
	tmp->max_val = (*img)->max_val;
	tmp->val = (rgb_t **)malloc(tmp->height * sizeof(rgb_t *));
	if (!tmp->val)
		return;
	for (int i = 0 ; i < tmp->height ; ++i) {
		tmp->val[i] = (rgb_t *)malloc(tmp->width * sizeof(rgb_t));
		if (!tmp->val[i]) {
			for (int j = 0 ; j < i ; j++)
				free(tmp->val[i]);
			free(tmp->val);
			return;
		}
	}
	for (int i = 0 ; i < tmp->height ; ++i)
		for (int j = 0 ; j < tmp->width ; ++j)
			tmp->val[i][j] = (*img)->val[i][j];
	if (!strcmp(p, "EDGE")) {
		for (int i = tmp->y1 ; i < tmp->y2 ; ++i)
			for (int j = tmp->x1 ; j < tmp->x2 ; ++j)
				tmp->val[i][j] = apply_filter((*img), i, j, 0);
		printf("APPLY %s done\n", p);
	} else if (!strcmp(p, "SHARPEN")) {
		for (int i = tmp->y1 ; i < tmp->y2 ; ++i)
			for (int j = tmp->x1 ; j < tmp->x2 ; ++j)
				tmp->val[i][j] = apply_filter((*img), i, j, 1);
		printf("APPLY %s done\n", p);
	} else if (!strcmp(p, "BLUR")) {
		for (int i = tmp->y1 ; i < tmp->y2 ; ++i)
			for (int j = tmp->x1 ; j < tmp->x2 ; ++j)
				tmp->val[i][j] = apply_filter((*img), i, j, 2);
		printf("APPLY %s done\n", p);
	} else if (!strcmp(p, "GAUSSIAN_BLUR")) {
		for (int i = tmp->y1 ; i < tmp->y2 ; ++i)
			for (int j = tmp->x1 ; j < tmp->x2 ; ++j)
				tmp->val[i][j] = apply_filter((*img), i, j, 3);
		printf("APPLY %s done\n", p);
	} else {
		printf("APPLY parameter invalid\n");
	}

	clear_image(*img);
	(*img) = tmp;
}

void rotate_image(image_t **img, int angle, int full_matrix)
{
	image_t *tmp = (image_t *)malloc(sizeof(image_t));
	strcpy(tmp->type, (*img)->type);
	tmp->height = (*img)->height;
	tmp->width = (*img)->width;
	tmp->y1 = (*img)->y1;
	tmp->x1 = (*img)->x1;
	tmp->max_val = (*img)->max_val;
	int index_i = tmp->y1, index_j = tmp->x1, aux = (*img)->height;
	if (full_matrix && angle != 180) {
		tmp->height = tmp->width;
		tmp->width = aux;
		tmp->x2 = tmp->width;
		tmp->y2 = tmp->height;
	} else {
		tmp->y2 = (*img)->y2;
		tmp->x2 = (*img)->x2;
	}
	tmp->val = (rgb_t **)malloc(tmp->height * sizeof(rgb_t *));
		if (!tmp->val)
			return;
		for (int i = 0 ; i < tmp->height ; ++i) {
			tmp->val[i] = (rgb_t *)malloc(tmp->width * sizeof(rgb_t));
			if (!tmp->val[i]) {
				for (int j = 0 ; j < i ; j++)
					free(tmp->val[i]);
				free(tmp->val);
				return;
			}
		}
	if (!full_matrix || angle == 180) {
		for (int i = 0 ; i < tmp->height ; ++i)
			for (int j = 0 ; j < tmp->width ; ++j)
				tmp->val[i][j] = (*img)->val[i][j];
		if (angle == 90) {
			for (int j = tmp->x1 ; j < tmp->x2 ; ++j) {
				for (int i = tmp->y2 - 1 ; i >= tmp->y1 ; --i)
					tmp->val[index_i][index_j++] = (*img)->val[i][j];
				index_j = tmp->x1;
				index_i++;
			}
		} else if (angle == 270) {
			for (int j = tmp->x2 - 1 ; j >= tmp->x1 ; --j) {
				for (int i = tmp->y1 ; i < tmp->y2 ; ++i)
					tmp->val[index_i][index_j++] = (*img)->val[i][j];
				index_j = tmp->x1;
				index_i++;
			}
		} else if (angle == 180) {
			for (int i = tmp->y2 - 1 ; i >= tmp->y1 ; --i) {
				for (int j = tmp->x2 - 1 ; j >= tmp->x1 ; --j)
					tmp->val[index_i][index_j++] = (*img)->val[i][j];
				index_j = tmp->x1;
				index_i++;
			}
		}
	} else {
		if (angle == 90) {
			for (int j = 0 ; j < (*img)->x2 ; ++j)
				for (int i = (*img)->y2 - 1 ; i >= 0 ; --i) {
					tmp->val[index_i][index_j++] = (*img)->val[i][j];
					if (index_j == tmp->width) {
						index_j = 0;
						index_i++;
					}
				}
		} else {
			for (int j = (*img)->x2 - 1 ; j >= 0 ; --j)
				for (int i = 0 ; i < (*img)->y2 ; ++i) {
					tmp->val[index_i][index_j++] = (*img)->val[i][j];
					if (index_j == tmp->width) {
						index_j = 0;
						index_i++;
					}
				}
		}
	}
	clear_image(*img);
	(*img) = tmp;
}

void rotate(image_t **img, char *p)
{
	if (!(*img)) {
		printf("No image loaded\n");
		return;
	}
	int ok = 1, angle = 0;
	p = strtok(NULL, " ");
	if (p) {
		if (strlen(p) == 1) {
			if (!isdigit(p[0]))
				ok = 0;
		} else {
			for (size_t i = 0 ; i < strlen(p) ; ++i) {
				if (i == 0 && isdigit(p[i + 1]) && p[0] == '-')
					continue;
				if (!isdigit(p[i]))
					ok = 0;
			}
		}
		if (ok)
			angle = atoi(p);
		p = strtok(NULL, " ");
	} else {
		printf("Invalid command\n");
		return;
	}

	int cop_angle = angle, w = (*img)->width, h = (*img)->height;
	int x1 = (*img)->x1, x2 = (*img)->x2, y1 = (*img)->y1, y2 = (*img)->y2;
	if ((x2 - x1 != y2 - y1) && (x1 != 0 || y1 != 0 || x2 != w || y2 != h)) {
		printf("The selection must be square\n");
		return;
	}
	if (!ok || p || angle % 90) {
		printf("Unsupported rotation angle\n");
		return;
	}
	angle %= 360;
	if (angle < 0)
		angle += 360;
	if (angle != 0) {
		if (x1 == 0 && y1 == 0 && x2 == w && y2 == h)
			rotate_image(img, angle, 1);
		else
			rotate_image(img, angle, 0);
	}
	printf("Rotated %d\n", cop_angle);
}

void save(image_t *img, char filename[], int type)
{
	if (!(img)) {
		printf("No image loaded\n");
		return;
	}
	if (type) {
		FILE *out = fopen(filename, "wt");
		if (!out)
			return;
		if (!strcmp(img->type, "P3") || !strcmp(img->type, "P6")) {
			fprintf(out, "P3\n");
			fprintf(out, "%d %d\n", img->width, img->height);
			fprintf(out, "%d\n", img->max_val);
			for (int i = 0 ; i < img->height ; ++i) {
				for (int j = 0 ; j < img->width ; ++j) {
					fprintf(out, "%d ", (int)img->val[i][j].r);
					fprintf(out, "%d ", (int)img->val[i][j].g);
					fprintf(out, "%d ", (int)img->val[i][j].b);
				}
				fprintf(out, "\n");
			}
		}
		if (!strcmp(img->type, "P2") || !strcmp(img->type, "P5")) {
			fprintf(out, "P2\n");
			fprintf(out, "%d %d\n", img->width, img->height);
			fprintf(out, "%d\n", img->max_val);
			for (int i = 0 ; i < img->height ; ++i) {
				for (int j = 0 ; j < img->width ; ++j)
					fprintf(out, "%d ", (int)img->val[i][j].g);
				fprintf(out, "\n");
			}
		}
		fclose(out);
	} else {
		FILE *fptr = fopen(filename, "wt");
		if (!fptr)
			return;
		if (!strcmp(img->type, "P3") || !strcmp(img->type, "P6")) {
			fprintf(fptr, "P6\n");
			fprintf(fptr, "%d %d\n", img->width, img->height);
			fprintf(fptr, "%d\n", img->max_val);
			int poz = ftell(fptr);
			fclose(fptr);
			FILE *out = fopen(filename, "r+b");
			if (!out)
				return;
			fseek(out, poz, SEEK_SET);
			for (int i = 0 ; i < img->height ; ++i)
				for (int j = 0 ; j < img->width ; ++j) {
					unsigned char r = img->val[i][j].r;
					unsigned char g = img->val[i][j].g;
					unsigned char b = img->val[i][j].b;
					fwrite(&r, sizeof(unsigned char), 1, out);
					fwrite(&g, sizeof(unsigned char), 1, out);
					fwrite(&b, sizeof(unsigned char), 1, out);
				}
			fclose(out);
		}
		if (!strcmp(img->type, "P2") || !strcmp(img->type, "P5")) {
			fprintf(fptr, "P5\n");
			fprintf(fptr, "%d %d\n", img->width, img->height);
			fprintf(fptr, "%d\n", img->max_val);
			int poz = ftell(fptr);
			fclose(fptr);
			FILE *out = fopen(filename, "r+b");
			if (!out)
				return;
			fseek(out, poz, SEEK_SET);
			for (int i = 0 ; i < img->height ; ++i)
				for (int j = 0 ; j < img->width ; ++j) {
					unsigned char g = img->val[i][j].g;
					fwrite(&g, sizeof(unsigned char), 1, out);
				}
			fclose(out);
		}
	}
	printf("Saved %s\n", filename);
}

int read_data(image_t **img)
{
	char line[NMAX], filename[NMAX], *p;
	fgets(line, NMAX, stdin);
	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';
	p = strtok(line, " ");
	if (!strcmp(p, "LOAD")) {
		if ((*img))
			clear_image(*img);
		p = strtok(NULL, " ");
		strcpy(filename, p);
		if (load(img, filename))
			fprintf(stdout, "Failed to load %s\n", filename);
		else
			fprintf(stdout, "Loaded %s\n", filename);
	} else if (!strcmp(p, "EXIT")) {
		(*img) ? clear_image(*img) : printf("No image loaded\n");
		return 1;
	} else if (!strcmp(p, "SELECT")) {
		select_area((*img), p);
	} else if (!strcmp(p, "CROP")) {
		crop(img);
	} else if (!strcmp(p, "EQUALIZE")) {
		equalize((*img));
	} else if (!strcmp(p, "APPLY")) {
		filter(img, p);
	} else if (!strcmp(p, "HISTOGRAM")) {
		histogram((*img), p);
	} else if (!strcmp(p, "ROTATE")) {
		rotate(img, p);
	} else if (!strcmp(p, "SAVE")) {
		p = strtok(NULL, " ");
		strcpy(filename, p);
		p = strtok(NULL, " ");
		p ?	save((*img), filename, 1) : save((*img), filename, 0);
	} else {
		fprintf(stdout, "Invalid command\n");
	}
	return 0;
}
