// Copyright Mantu Matei-Cristian 312CA 2022-2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "image.h"

void skip_comments(FILE(*f));

double ***allocate_colour(int width, int height);

void free_colour(double ***colour, int width, int height);

int read_colour(image_t *image, FILE *f);

double **allocate_gs(int width, int height);

int read_gs(image_t *image, FILE *f);

void free_gs(double **gs, int height);

int load_matrix(image_t *image, char *filename);

int select_area(image_t *image, char *parameters);
