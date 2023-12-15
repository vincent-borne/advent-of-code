#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/15/input.txt"

void split(char* str, char* delim, char** result, int* result_size) {
    *result_size = -1;
    char* ptr = strtok(str, delim);

    while (ptr != NULL) {
        (*result_size)++;
        result[*result_size] = ptr;
        ptr = strtok(NULL, delim);
    }

    (*result_size)++;
}

int calculate_hash(char* str) {
    int hash = 0;

    for (size_t i = 0; i < strlen(str); i++) {
        hash += str[i];  // Add ASCII code to current_value
        hash *= 17;
        hash %= 256;
    }

    return hash;
}

int main(void) {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    typedef struct {
        char** labels;
        int* lenses;
    } Box;

    int size = 256;
    Box boxes[size];

    for (int i = 0; i < size; i++) {
        Box b;
        b.labels = NULL;
        b.lenses = NULL;
        boxes[i] = b;
    }

    int result = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_path_size;
        char* split_path[100000];
        split(line, ",", split_path, &split_path_size);

        printf("Size: %d\n", split_path_size);
        for (int i = 0; i < split_path_size; i++) {
            if (strstr(split_path[i], "=") != NULL) {
                printf("Step: %s\n", split_path[i]);
                int split_equal_size;
                char* split_equal[2];
                split(split_path[i], "=", split_equal, &split_equal_size);

                int box_index = calculate_hash(split_equal[0]);
                Box* b = &boxes[box_index];

                bool found_label = false;

                for (int j = 0; j < arrlen(b->labels); j++) {
                    if (strcmp(b->labels[j], split_equal[0]) == 0) {
                        found_label = true;
                        b->lenses[j] = atoi(split_equal[1]);
                        break;
                    }
                }

                if (!found_label) {
                    arrput(b->labels, split_equal[0]);
                    arrput(b->lenses, atoi(split_equal[1]));
                }
            } else {
                split_path[i][strlen(split_path[i]) - 1] = '\0';
                int box_index = calculate_hash(split_path[i]);
                Box* b = &boxes[box_index];

                for (int j = 0; j < arrlen(b->labels); j++) {
                    if (strcmp(b->labels[j], split_path[i]) == 0) {
                        arrdel(b->labels, j);
                        arrdel(b->lenses, j);
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (arrlen(boxes[i].labels) > 0) {
                printf("\nBox %d: ", i);
                for (int j = 0; j < arrlen(boxes[i].labels); j++) {
                    int box_number = i + 1;
                    int slot = j + 1;
                    result += box_number * slot * boxes[i].lenses[j];
                    printf(" [%s %d]", boxes[i].labels[j], boxes[i].lenses[j]);
                }
            }
        }
    }

    printf("\nPart 2: %d\n", result);
    return 0;
}