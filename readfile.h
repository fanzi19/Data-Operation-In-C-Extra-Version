#ifndef READFILE_H
#define READFILE_H

#include <stdio.h>

int open_file(FILE **file, const char *filename);
int read_int(FILE *file, int *value);
int read_string(FILE *file, char *value, int size);
int read_float(FILE *file, float *value);
int close_file(FILE *file);
void skip_header(FILE *file);
void printData(int *count);

#endif

