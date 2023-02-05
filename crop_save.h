// Copyright Mantu Matei-Cristian 312CA 2022-2023
#pragma once
#include <stdio.h>
#include "image.h"
#include <string.h>
#include <math.h>
#include "load_select.h"

int crop(image_t *image);

void save_colour(image_t image, int new_magic, FILE *f);

void save_gs(image_t image, int new_magic, FILE *f);

int save(image_t image, char *parameters);
