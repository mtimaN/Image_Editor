// Copyright Mantu Matei-Cristian 312CA 2022-2023
#pragma once
#include "utils.h"
#include "load_select.h"
#include "image.h"

void reverse_col_colour(image_t image);

void reverse_rows_colour(image_t image);

void transpose_colour(image_t *image);

void reverse_col_gs(image_t image);

void reverse_rows_gs(image_t image);

void transpose_gs(image_t *image);

int rotate(image_t *image, char *parameters);
