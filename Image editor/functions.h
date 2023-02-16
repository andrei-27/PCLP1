// Ⓒ Pirlea Andrei 312CAa 2022-2023

#pragma once

typedef struct {
	double r;
	double g;
	double b;
} rgb_t;

typedef struct {
	char type[3];
	int width;
	int height;
	int max_val;
	int x1, x2, y1, y2;
	rgb_t **val;
} image_t;

double clamp(double x, double min_val, double max_val);
void clear_image(image_t *img);
void load_P2(image_t *img, char filename[], int prev);
void load_P3(image_t *img, char filename[], int prev);
void load_P5(image_t *img, char filename[], int prev);
void load_P6(image_t *img, char filename[], int prev);
int load(image_t **img, char filename[]);
void select_area(image_t *img, char *p);
void crop(image_t **img);
void equalize(image_t *img);
void histogram(image_t *img, char *p);
rgb_t apply_filter(image_t *img, int x, int y, int filter);
void filter(image_t **img, char *p);
void rotate_image(image_t **img, int angle, int full_matrix);
void rotate(image_t **img, char *p);
void save(image_t *img, char filename[], int type);
int read_data(image_t **img);
