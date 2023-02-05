// Copyright Mantu Matei-Cristian 312CA 2022-2023
#pragma once
#define MAX_LENGTH 512

typedef struct {
	int x, y;
} point_t;

typedef struct {
	int magic;
	int width, height;
	int max_value;
	union {
		double ***colour;
		double **gs;
	} pic;
	point_t p1, p2;
} image_t;
