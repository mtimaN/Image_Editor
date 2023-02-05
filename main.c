// Copyright Mantu Matei-Cristian 312CA 2022-2023
#include "histogram.h"
#include "image.h"
#include "load_select.h"
#include "utils.h"
#include "crop_save.h"
#include "apply.h"
#include "rotate.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void)
{
	image_t image;
	char input[MAX_LENGTH], *command, *parameters;
	// magic == 1 => no image loaded
	image.magic = 1;
	do {
		fgets(input, MAX_LENGTH, stdin);

		// ignoring empty lines
		while (input[0] == '\n')
			fgets(input, MAX_LENGTH, stdin);

		// parsing the input into command and parameters
		command = strtok(input, "\n ");
		parameters = strtok(NULL, "\n");

		if (strcmp(command, "LOAD") == 0)
			load_matrix(&image, parameters);
		else if (image.magic == 1)
			printf("No image loaded\n");
		else if (strcmp(command, "SELECT") == 0)
			select_area(&image, parameters);
		else if (strcmp(command, "HISTOGRAM") == 0)
			histogram(image, parameters);
		else if (strcmp(command, "EQUALIZE") == 0)
			equalize(&image);
		else if (strcmp(command, "CROP") == 0)
			crop(&image);
		else if (strcmp(command, "SAVE") == 0)
			save(image, parameters);
		else if (strcmp(command, "APPLY") == 0)
			apply(&image, parameters);
		else if (strcmp(command, "ROTATE") == 0)
			rotate(&image, parameters);
		else if (strcmp(command, "EXIT") != 0)
			printf("Invalid command\n");
	} while (strcmp(command, "EXIT") != 0);

	if (image.magic % 3 == 0)
		free_colour(image.pic.colour, image.width, image.height);
	else if (image.magic % 3 == 2)
		free_gs(image.pic.gs, image.height);
	return 0;
}
