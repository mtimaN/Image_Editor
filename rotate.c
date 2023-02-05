// Copyright Mantu Matei-Cristian 312CA 2022-2023
#include "utils.h"
#include "load_select.h"
#include "image.h"

void reverse_col_colour(image_t image)
{
	int i, j1, j2, k;
	for (i = image.p1.y ; i < image.p2.y; ++i)
		for (j1 = image.p1.x, j2 = image.p2.x - 1; j1 < j2; ++j1, --j2)
			for (k = 0; k < 3; ++k)
				swap_F(&image.pic.colour[i][j1][k]
					, &image.pic.colour[i][j2][k]);
}

void reverse_rows_colour(image_t image)
{
	int i1, i2, j, k;
	for (i1 = image.p1.y, i2 = image.p2.y - 1; i1 < i2; ++i1, --i2)
		for (j = image.p1.x; j < image.p2.x; ++j)
			for (k = 0; k < 3; ++k)
				swap_F(&image.pic.colour[i1][j][k]
					, &image.pic.colour[i2][j][k]);
}

void transpose_colour(image_t *image)
{
	int i, j, k;
	if (image->p2.y - image->p1.y != image->p2.x - image->p1.x) {
		// copying the elements transposed into another matrix
		double ***new = allocate_colour(image->height, image->width);
		for (i = 0; i < image->height; ++i)
			for (j = 0; j < image->width; ++j)
				for (k = 0; k < 3; ++k)
					new[j][i][k] = image->pic.colour[i][j][k];

		// replacing the old matrix
		free_colour(image->pic.colour, image->width, image->height);
		image->pic.colour = new;

		// updating size
		my_swap_int(&image->height, &image->width);
		my_swap_int(&image->p2.y, &image->p2.x);
		return;
	}
	// transposing selection by swapping along the diagonal
	for (i = 0; i < image->p2.y - image->p1.y; ++i)
		for (j = i + 1; j < image->p2.x - image->p1.x; ++j) {
			int iy = i + image->p1.y;
			int jx = j + image->p1.x;
			int jy = j + image->p1.y;
			int ix = i + image->p1.x;
			for (k = 0; k < 3; ++k)
				swap_F(&image->pic.colour[iy][jx][k]
					, &image->pic.colour[jy][ix][k]);
		}
}

void reverse_col_gs(image_t image)
{
	int i, j1, j2;
	for (i = image.p1.y; i < image.p2.y; ++i)
		for (j1 = image.p1.x, j2 = image.p2.x - 1; j1 < j2; ++j1, --j2)
			swap_F(&image.pic.gs[i][j1], &image.pic.gs[i][j2]);
}

void reverse_rows_gs(image_t image)
{
	int i1, i2, j;
	for (i1 = image.p1.y, i2 = image.p2.y - 1; i1 < i2; ++i1, --i2)
		for (j = image.p1.x; j < image.p2.x; ++j)
			swap_F(&image.pic.gs[i1][j], &image.pic.gs[i2][j]);
}

void transpose_gs(image_t *image)
{
	int i, j;
	if (image->p2.y - image->p1.y != image->p2.x - image->p1.x) {
		// copying the elements transposed into another matrix
		double **new = allocate_gs(image->height, image->width);
		for (i = 0; i < image->height; ++i)
			for (j = 0; j < image->width; ++j)
				new[j][i] = image->pic.gs[i][j];

		// replacing the old matrix
		free_gs(image->pic.gs, image->height);
		image->pic.gs = new;

		// updating size
		my_swap_int(&image->height, &image->width);
		my_swap_int(&image->p2.y, &image->p2.x);
		return;
	}
	for (i = 0; i < image->p2.y - image->p1.y; ++i)
		for (j = i + 1; j < image->p2.x - image->p1.x; ++j) {
			int iy = i + image->p1.y;
			int jx = j + image->p1.x;
			int jy = j + image->p1.y;
			int ix = i + image->p1.x;
			swap_F(&image->pic.gs[iy][jx], &image->pic.gs[jy][ix]);
		}
}

int rotate(image_t *image, char *parameters)
{
	if (image->magic == 1) {
		printf("No image loaded\n");
		return -1;
	}
	if (!parameters) {
		printf("Invalid command\n");
		return -1;
	}
	if (image->p1.x != 0 || image->p1.y != 0)
		if (image->p2.x != image->width || image->p2.y != image->height)
			if (image->p2.y - image->p1.y != image->p2.x - image->p1.x) {
				printf("The selection must be square\n");
				return -1;
			}
	char *arg = strtok(parameters, " ");
	int angle = atoi(arg);
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return -1;
	}
	angle %= 360;
	if (image->magic % 3 == 2) {
		if (angle == 90 || angle == -270) {
			// transpose the matrix
			transpose_gs(image);
			// reverse matrix columns
			reverse_col_gs(*image);
		} else if (angle == 180 || angle == -180) {
			// reverse matrix columns
			reverse_col_gs(*image);
			// reverse matrix rows
			reverse_rows_gs(*image);
		} else if (angle == 270 || angle == -90) {
			// transpose the matrix
			transpose_gs(image);
			// reverse matrix rows
			reverse_rows_gs(*image);
		}
	} else {
		if (angle == 90 || angle == -270) {
			// transpose the matrix
			transpose_colour(image);
			// reverse matrix columns
			reverse_col_colour(*image);
		} else if (angle == 180 || angle == -180) {
			// reverse matrix columns
			reverse_col_colour(*image);
			// reverse matrix rows
			reverse_rows_colour(*image);
		} else if (angle == 270 || angle == -90) {
			// transpose the matrix
			transpose_colour(image);
			// reverse matrix rows
			reverse_rows_colour(*image);
		}
	}
	printf("Rotated %d\n", atoi(arg));
	return 0;
}
