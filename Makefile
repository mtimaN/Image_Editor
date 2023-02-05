# Copyright 2022 Matei Mantu

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor rotate load_select utils apply histogram crop_save

build: $(TARGETS)
	$(CC) $(CFLAGS) utils.o rotate.o apply.o crop_save.o histogram.o load_select.o image_editor.o -lm -o image_editor

image_editor: main.c
	$(CC) $(CFLAGS) -c main.c -o image_editor.o

load_select: load_select.c
	$(CC) $(CFLAGS) -c load_select.c -o load_select.o

histogram: histogram.c
	$(CC) $(CFLAGS) -c histogram.c -o histogram.o

crop_save: crop_save.c
	$(CC) $(CFLAGS) -c crop_save.c -o crop_save.o

apply: apply.c
	$(CC) $(CFLAGS) -c apply.c -o apply.o

utils: utils.c
	$(CC) $(CFLAGS) -c utils.c -o utils.o

rotate: rotate.c
	$(CC) $(CFLAGS) -c rotate.c -o rotate.o

pack:
	zip -FSr 312CA_MantuMatei_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
