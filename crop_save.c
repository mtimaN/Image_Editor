// Copyright Mantu Matei-Cristian 312CA 2022-2023
#include <stdio.h>
#include "image.h"
#include <string.h>
#include <math.h>
#include "load_select.h"

int crop(image_t *image)
{
	if (image->magic == 1) {
		printf("No image loaded\n");
		return -1;
	}
	printf("Image cropped\n");
	int i, j;
	int new_height = image->p2.y - image->p1.y;
	int new_width = image->p2.x - image->p1.x;
	if (image->magic % 3 == 2) {
		// making a copy
		double **new_gs = allocate_gs(new_width, new_height);
		for (i = 0; i < new_height; ++i)
			for (j = 0; j < new_width; ++j) {
				int old_i = i + image->p1.y;
				int old_j = j + image->p1.x;
				new_gs[i][j] = image->pic.gs[old_i][old_j];
			}

		// freeing up the old one
		free_gs(image->pic.gs, image->height);

		// updating size
		image->height = new_height;
		image->width = new_width;
		image->p2.y = image->height;
		image->p1.y = 0;
		image->p2.x = image->width;
		image->p1.x = 0;

		// shallow copy
		image->pic.gs = new_gs;
		return 0;
	} else if (image->magic % 3 == 0) {
		// making a copy
		double ***new_colour = allocate_colour(new_width, new_height);
		for (i = 0; i < new_height; ++i)
			for (j = 0; j < new_width; ++j) {
				int old_i = i + image->p1.y;
				int old_j = j + image->p1.x;
				for (int k = 0; k < 3; ++k)
					new_colour[i][j][k] = image->pic.colour[old_i][old_j][k];
			}

		// freeing up the old one
		free_colour(image->pic.colour, image->width, image->height);

		// updating size
		image->height = new_height;
		image->width = new_width;
		image->p2.y = image->height;
		image->p1.y = 0;
		image->p2.x = image->width;
		image->p1.x = 0;

		// shallow copy
		image->pic.colour = new_colour;
		return 0;
	}
	return -1;
}

void save_colour(image_t image, int new_magic, FILE *f)
{
	if (new_magic == 6) {
		// binary file
		unsigned char binary;
		int i, j, k;
		for (i = 0; i < image.height; ++i)
			for (j = 0; j < image.width; ++j)
				for (k = 0; k < 3; ++k) {
					binary = round(image.pic.colour[i][j][k]);
					fputc(binary, f);
				}
	} else {
		// ascii file
		int i, j, k;
		for (i = 0; i < image.height; ++i) {
			for (j = 0; j < image.width; ++j)
				for (k = 0; k < 3; ++k)
					fprintf(f, "%0.lf ", round(image.pic.colour[i][j][k]));
			fprintf(f, "\n");
		}
	}
}

void save_gs(image_t image, int new_magic, FILE *f)
{
	if (new_magic == 5) {
		// binary file
		unsigned char binary;
		int i, j;
		for (i = 0; i < image.height; ++i)
			for (j = 0; j < image.width; ++j) {
				binary = round(image.pic.gs[i][j]);
				fputc(binary, f);
			}
	} else {
		// ascii file
		int i, j;
		for (i = 0; i < image.height; ++i) {
			for (j = 0; j < image.width; ++j)
				fprintf(f, "%0.lf ", image.pic.gs[i][j]);
			fprintf(f, "\n");
		}
	}
}

int save(image_t image, char *parameters)
{
	if (image.magic == 1) {
		printf("No image loaded\n");
		return -1;
	}

	char *filename = strtok(parameters, "\n ");
	char *ascii = strtok(NULL, "\n ");
	FILE *new = fopen(filename, "wt");

	if (!new) {
		printf("Failed to open %s\n", filename);
		return -1;
	}

	int new_magic = image.magic % 3;
	if (!new_magic)
		new_magic = 3;
	if (!ascii)
		new_magic += 3;
	fprintf(new, "P%d\n", new_magic);
	fprintf(new, "%d %d\n%d\n", image.width, image.height, image.max_value);
	if (new_magic % 3 == 0)
		save_colour(image, new_magic, new);
	else
		save_gs(image, new_magic, new);
	printf("Saved %s\n", filename);
	fclose(new);
	return 0;
}
