#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/14/input.txt"

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

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int map_x_size = 0;
    int map_y_size = 0;
    char map[1000][1000];

    while ((read = getline(&line, &len, f)) != -1) {
        map_x_size = 0;
        strtok(line, "\n");

        for (size_t i = 0; i < strlen(line); i++) {
            if (line[i] != 'O') {
                map[map_y_size][map_x_size] = line[i];
            } else {
                int lowest_y = map_y_size;

                for (int y = map_y_size - 1; y >= 0; y--) {
                    if (map[y][map_x_size] == '.') {
                        if (y < lowest_y) {
                            lowest_y = y;
                        }
                    } else {
                        break;
                    }
                }

                map[map_y_size][map_x_size] = '.';
                map[lowest_y][map_x_size] = 'O';
            }

            ++map_x_size;
        }

        map_y_size++;
    }

    int i = 1;
    int result = 0;

    for (int y = map_y_size - 1; y >= 0; y--) {
        int O_count = 0;

        for (int x = 0; x < map_x_size; x++) {
            if (map[y][x] == 'O') {
                O_count++;
            }
        }

        // printf("O_count: %d, %d\n", O_count, i);
        result += O_count * i;
        ++i;
    }

    printf("\nPart 1: %d\n", result);
    return 0;
}