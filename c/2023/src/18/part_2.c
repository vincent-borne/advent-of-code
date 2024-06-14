#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/18/test_input.txt"

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

int main(void) {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    enum Dir { R, D, L, U };

    typedef struct {
        int x;
        int y;
        int value;
        enum Dir dir;
    } Line;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int min_x = INT_MAX;
    int max_x = INT_MIN;
    int min_y = INT_MAX;
    int max_y = INT_MIN;

    Line* lines = NULL;

    Line start;
    start.value = 1;
    start.dir = R;
    start.x = 0;
    start.y = 0;

    arrput(lines, start);

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_path_size;
        char* split_path[3];
        split(line, " ", split_path, &split_path_size);

        char hex[6];

        for (unsigned long i = 2; i < strlen(split_path[2]) - 2; i++) {
            hex[i - 2] = split_path[2][i];
        }

        hex[strlen(split_path[2]) - 4] = '\0';

        Line last = arrlast(lines);

        Line l;
        l.value = strtol(hex, NULL, 16);
        l.dir = split_path[2][strlen(split_path[2]) - 2] - '0';
        l.x = last.x;
        l.y = last.y;

        if (l.dir == R) {
            l.x += l.value;
        } else if (l.dir == U) {
            l.y -= l.value;
        } else if (l.dir == L) {
            l.x -= l.value;
        } else if (l.dir == D) {
            l.y += l.value;
        }

        if (l.x < min_x) {
            min_x = l.x;
        }

        if (l.x > max_x) {
            max_x = l.x;
        }

        if (l.y < min_y) {
            min_y = l.y;
        }

        if (l.y > max_y) {
            max_y = l.y;
        }

        arrput(lines, l);
    }

    int result = 0;

    for (int i = 0; i < arrlen(lines); i++) {
        Line l = lines[i];
        printf("%d %d, (%d,%d)\n", l.dir, l.value, l.x, l.y);
    }

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int count_x = 0;
            int count_y = 0;

            for (int i = 1; i < arrlen(lines); i++) {
                Line lp = lines[i - 1];
                Line l = lines[i];
                int new_x = lp.x + l.x;
                int new_y = lp.x + l.x;

                if (new_x < x && y == lp.y) {
                    count_x++;
                }

                if (new_y < y && x == lp.x) {
                    count_y++;
                }

                // printf("%d %d, (%d,%d)\n", l.dir, l.value, l.x, l.y);
            }

            if (count_x % 2 != 0 && count_y % 2 != 0) {
                result++;
            }
        }
    }

    printf("Min: %d %d\n", min_x, min_y);
    printf("Max: %d %d\n", max_x, max_y);

    printf("Part 2: %d\n", result);
    return 0;
}