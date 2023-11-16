#include "readfile.h"
#include <stdlib.h>

int open_file(FILE **file, const char *filename) {
    *file = fopen(filename, "r");
    if (*file == NULL) {
        return 1;
    }
    return 0;
}

int read_int(FILE *file, int *value) {
    // Implementation for reading an integer from the file
    return fscanf(file, "%d", value);
}

int read_string(FILE *file, char *value, int size) {
    // Implementation for reading a string from the file
    return fscanf(file, "%s", value);
}

int read_float(FILE *file, float *value) {
    // Implementation for reading a float from the file
    return fscanf(file, "%f", value);
}

int close_file(FILE *file) {
    if (fclose(file) != 0) {
        return 1;
    }
    return 0;
}

void skip_header(FILE *file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            break;
        }
    }
}


void printData(int *count) {
    FILE *fp;
    char buffer[100];
    *count = 0;

    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.");
        exit(1);
    }
    
    skip_header(fp);

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
        (*count)++;
    }

    fclose(fp);
}


