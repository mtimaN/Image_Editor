// Copyright Mantu Matei-Cristian 312CA 2022-2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "load_select.h"
#include "image.h"

void skip_comments(FILE *f)
{
	char input[MAX_LENGTH];
	fpos_t pos;
	do {
		fgetpos(f, &pos);
		fgets(input, MAX_LENGTH, f);
	} while (strchr(input, '#'));
	fsetpos(f, &pos);
}

double ***allocate_colour(int width, int height)
{
	double ***colour = (double ***)malloc(height * (sizeof(double **)));
	if (!colour) {
		printf("Error allocating\n");
		return NULL;
	}
	int i, j;
	for (i = 0; i < height; ++i) {
		colour[i] = (double **)malloc(width * (sizeof(double *)));
		if (!colour[i]) {
			for (j = 0; j < i; ++j)
				free(colour[i]);
			free(colour);
			printf("Error allocating\n");
			return NULL;
		}
	}
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j) {
			colour[i][j] = (double *)malloc(3 * sizeof(double));
			if (!colour[i][j]) {
				for (int k = 0; k < j; ++k)
					free(colour[i][j]);
				for (int k1 = 0; k1 < i; ++k1)
					for (int k2 = 0; k2 < width; ++k2)
						free(colour[i]);
				free(colour);
				printf("Error allocating\n");
				return NULL;
			}
		}
	return colour;
}

void free_colour(double ***colour, int width, int height)
{
	int i, j;
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j)
			free(colour[i][j]);
	for (i = 0; i < height; ++i)
		free(colour[i]);
	free(colour);
}

int read_colour(image_t *image, FILE *f)
{
	image->pic.colour = allocate_colour(image->width, image->height);
	if (image->magic == 6) {
		// binary file
		unsigned char binary;
		binary = fgetc(f);
		int i, j, k;
		for (i = 0; i < image->height; ++i)
			for (j = 0; j < image->width; ++j)
				for (k = 0; k < 3; ++k) {
					binary = fgetc(f);
					image->pic.colour[i][j][k] = binary;
				}
	} else {
		// ascii file
		int i, j, k;
		for (i = 0; i < image->height; ++i)
			for (j = 0; j < image->width; ++j)
				for (k = 0; k < 3; ++k)
					fscanf(f, "%lf", &image->pic.colour[i][j][k]);
	}

	fclose(f);
	if (image->pic.colour)
		return 0;
	else
		return -1;
}

double **allocate_gs(int width, int height)
{
	double **gs = (double **)malloc(height * (sizeof(double *)));
	if (!gs) {
		printf("Error allocating\n");
		return NULL;
	}
	int i;
	for (i = 0; i < height; ++i) {
		gs[i] = (double *)malloc(width * (sizeof(double)));
		if (!gs[i]) {
			for (int j = 0; j < i; ++j)
				free(gs[j]);
			printf("Error allocating\n");
			return NULL;
		}
	}
	return gs;
}

int read_gs(image_t *image, FILE *f)
{
	image->pic.gs = allocate_gs(image->width, image->height);

	if (image->magic == 5) {
		// binary file
		unsigned char binary;
		int i, j;
		binary = fgetc(f);
		for (i = 0; i < image->height; ++i)
			for (j = 0; j < image->width; ++j) {
				binary = fgetc(f);
				image->pic.gs[i][j] = binary;
			}
	} else {
		// ascii file
		int i, j;
		for (i = 0; i < image->height; ++i)
			for (j = 0; j < image->width; ++j)
				fscanf(f, "%lf", &image->pic.gs[i][j]);
		}
	fclose(f);
	if (image->pic.gs)
		return 0;
	else
		return -1;
}

void free_gs(double **gs, int height)
{
	int i;
	for (i = 0; i < height; ++i)
		free(gs[i]);
	free(gs);
}

int load_matrix(image_t *image, char *filename)
{
	char line[MAX_LENGTH];
	//scrape old photo
	if (image->magic % 3 == 0)
		free_colour(image->pic.colour, image->width, image->height);
	else if (image->magic % 3 == 2)
		free_gs(image->pic.gs, image->height);

	image->magic = 1;
	FILE *f = fopen(filename, "rt");
	if (!f) {
		printf("Failed to load %s\n", filename);
		return -1;
	}
	fgets(line, MAX_LENGTH, f);
	image->magic = line[1] - '0';
	skip_comments(f);
	fscanf(f, "%d %d", &image->width, &image->height);
	skip_comments(f);
	fscanf(f, "%d", &image->max_value);
	skip_comments(f);

	image->p1.x = 0;
	image->p2.x = image->width;
	image->p1.y = 0;
	image->p2.y = image->height;

	if (image->magic % 3 == 0) {
		// grayscale image
		if (read_colour(image, f) == 0) {
			printf("Loaded %s\n", filename);
			return 0;
		}
		printf("Failed to load %s\n", filename);
		return -1;
	} else if (image->magic % 3 == 2) {
		// coloured image
		if (read_gs(image, f) == 0) {
			printf("Loaded %s\n", filename);
			return 0;
		}
		printf("Failed to load %s\n", filename);
		return -1;
	}
	return -1;
}

int select_area(image_t *image, char *parameters)
{
	if (image->magic == 1) {
		return -1;
		printf("No image loaded\n");
	}
	char *arg = strtok(parameters, " ");
	if (!arg) {
		printf("Invalid command\n");
		return -1;
	}
	if (strcmp(arg, "ALL") == 0) {
		image->p1.x = 0;
		image->p1.y = 0;
		image->p2.x = image->width;
		image->p2.y = image->height;
		printf("Selected ALL\n");
		return 0;
	}
	point_t coords[2];
	int i;
	for (i = 0; i < 2; ++i) {
		if (!arg || isalpha(arg[0])) {
			printf("Invalid command\n");
			return -1;
		}
		coords[i].x = atoi(arg);
		arg = strtok(NULL, " ");
		if (!arg || isalpha(arg[0])) {
			printf("Invalid command\n");
			return -1;
		}
		coords[i].y = atoi(arg);
		arg = strtok(NULL, " ");
		// check whether x is too big
		if (coords[i].x > image->width) {
			printf("Invalid set of coordinates\n");
			return -1;
		}
		// check whether y is too big
		if (coords[i].y > image->height) {
			printf("Invalid set of coordinates\n");
			return -1;
		}
		if (0 > coords[i].x || 0 > coords[i].y) {
			printf("Invalid set of coordinates\n");
			return -1;
		}
	}

	if (coords[0].x == coords[1].x || coords[0].y == coords[1].y) {
		printf("Invalid set of coordinates\n");
		return -1;
	}

	if (coords[0].x > coords[1].x)
		my_swap_int(&coords[0].x, &coords[1].x);
	if (coords[0].y > coords[1].y)
		my_swap_int(&coords[0].y, &coords[1].y);

	image->p1.x = coords[0].x;
	image->p1.y = coords[0].y;
	image->p2.x = coords[1].x;
	image->p2.y = coords[1].y;
	printf("Selected %d %d %d %d\n", image->p1.x, image->p1.y
								, image->p2.x, image->p2.y);
	return 0;
}
