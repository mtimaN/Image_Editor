// Copyright Mantu Matei-Cristian 312CA 2022-2023
#include "utils.h"
#include "load_select.h"
#include <string.h>

int apply(image_t *image, char *parameters)
{
	if (!parameters) {
		printf("Invalid command\n");
		return -1;
	}

	if (image->magic % 3 == 2) {
		printf("Easy, Charlie Chaplin\n");
		return -1;
	} else if (image->magic == 1) {
		printf("No image loaded\n");
		return -1;
	}

	double ker[3][3];
	if (strcmp(parameters, "EDGE") == 0) {
		ker[0][0] = -1; ker[0][1] = -1; ker[0][2] = -1;
		ker[1][0] = -1; ker[1][1] = 8; ker[1][2] = -1;
		ker[2][0] = -1; ker[2][1] = -1; ker[2][2] = -1;
	} else if (strcmp(parameters, "SHARPEN") == 0) {
		ker[0][0] = 0; ker[0][1] = -1; ker[0][2] = 0;
		ker[1][0] = -1; ker[1][1] = 5; ker[1][2] = -1;
		ker[2][0] = 0; ker[2][1] = -1; ker[2][2] = 0;
	} else if (strcmp(parameters, "BLUR") == 0) {
		ker[0][0] = 1.F / 9; ker[0][1] = 1.F / 9; ker[0][2] = 1.F / 9;
		ker[1][0] = 1.F / 9; ker[1][1] = 1.F / 9; ker[1][2] = 1.F / 9;
		ker[2][0] = 1.F / 9; ker[2][1] = 1.F / 9; ker[2][2] = 1.F / 9;
	} else if (strcmp(parameters, "GAUSSIAN_BLUR") == 0) {
		ker[0][0] = 1.F / 16; ker[0][1] = 1.F / 8; ker[0][2] = 1.F / 16;
		ker[1][0] = 1.F / 8; ker[1][1] = 1.F / 4; ker[1][2] = 1.F / 8;
		ker[2][0] = 1.F / 16; ker[2][1] = 1.F / 8; ker[2][2] = 1.F / 16;
	} else {
		printf("APPLY parameter invalid\n");
		return -1;
	}
	int addx = 0, addy = 0, minusy = 0, minusx = 0;
	double ***new_colour = allocate_colour(image->width, image->height);

	// shifting the selection if it includes borders
	if (image->p1.y == 0) {
		image->p1.y++;
		addy = 1;
	}
	if (image->p1.x == 0) {
		image->p1.x++;
		addx = 1;
	}
	if (image->p2.y == image->height) {
		image->p2.y--;
		minusy = 1;
	}
	if (image->p2.x == image->width) {
		image->p2.x--;
		minusx = 1;
	}
	int i, j, k;
	for (i = 0; i < image->height; ++i)
		for (j = 0; j < image->width; ++j)
			for (k = 0; k < 3; ++k)
				new_colour[i][j][k] = image->pic.colour[i][j][k];
	for (i = image->p1.y; i < image->p2.y; ++i)
		for (j = image->p1.x; j < image->p2.x; ++j)
			for (k = 0; k < 3; ++k) {
				int l, m;
				double s = 0;
				for (l = 0; l < 3; ++l) {
					int ni = i + 1 - l;
					for (m = 0; m < 3; ++m)
						s += image->pic.colour[ni][j + 1 - m][k] * ker[l][m];
				}
				new_colour[i][j][k] = clamp(s);
			}
	image->p1.y -= addy;
	image->p1.x -= addx;
	image->p2.y += minusy;
	image->p2.x += minusx;
	free_colour(image->pic.colour, image->width, image->height);
	image->pic.colour = new_colour;
	printf("APPLY %s done\n", parameters);
	return 0;
}
