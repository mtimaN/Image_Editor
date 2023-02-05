// Copyright Mantu Matei-Cristian 312CA 2022-2023
#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include "image.h"

int histogram(image_t image, char *parameters)
{
	if (!parameters) {
		printf("Invalid command\n");
		return -1;
	}
	char *arg = strtok(parameters, " ");
	int x = atoi(arg);
	arg = strtok(NULL, " ");
	if (!arg) {
		printf("Invalid command\n");
		return -1;
	}
	int y = atoi(arg);
	arg = strtok(NULL, " ");
	if (arg) {
		printf("Invalid command\n");
		return -1;
	}
	// checking validity of parameters
	if (!is_power_of_2(y)) {
		printf("Invalid input\n");
		return -1;
	} else if (image.magic % 3 == 0) {
		printf("Black and white image needed\n");
		return -1;
	} else if (image.magic == 1) {
		printf("No image loaded\n");
		return -1;
	}

	// computing the frequency of each bin
	int *freq = (int *)calloc(y, sizeof(int));
	int mx = 0;
	int i, j, bin = 256 / y;
	for (i = image.p1.y; i < image.p2.y; ++i)
		for (j = image.p1.x; j < image.p2.x; ++j)
			freq[(int)image.pic.gs[i][j] / bin] += x;
	for (i = 0; i < y; ++i)
		if (freq[i] / x > mx)
			mx = freq[i] / x;
	for (i = 0; i < y; ++i)
		freq[i] /= mx;

	// printing the histogram
	for (i = 0; i < y; ++i) {
		printf("%d\t|\t", freq[i]);
		for (j = 0; j < freq[i]; ++j)
			printf("*");
		printf("\n");
	}
	free(freq);
	return 0;
}

int equalize(image_t *image)
{
	if (image->magic % 3 == 0) {
		printf("Black and white image needed\n");
		return -1;
	} else if (image->magic == 1) {
		printf("No image loaded\n");
		return -1;
	}
	int freq[256] = {0};
	int i, j, area = image->height * image->width;

	for (i = 0; i < image->height; ++i)
		for (j = 0; j < image->width; ++j)
			freq[(int)image->pic.gs[i][j]]++;

	for (i = 0; i < image->height; ++i) {
		for (j = 0; j < image->width; ++j) {
			int a = (int)image->pic.gs[i][j];
			int s = 0;
			for (int h = 0; h <= a; ++h)
				s += freq[h];
			image->pic.gs[i][j] = 255.F * s / area;
		}
	}
	image->max_value = 255;
	printf("Equalize done\n");
	return 0;
}
