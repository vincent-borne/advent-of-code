#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/13/test_input.txt"

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

    int patterns_size = 0;
    int pattern_lines_size[1000];
    char* patterns[1000][100];

    for (int i = 0; i < 1000; i++) {
        pattern_lines_size[i] = 0;
    }

    while ((read = getline(&line, &len, f)) != -1) {
        if (strcmp(line, "\n") == 0) {
            patterns_size++;
            continue;
        }

        strtok(line, "\n");

        patterns[patterns_size][pattern_lines_size[patterns_size]] = malloc(sizeof(char) * strlen(line));
        strcpy(patterns[patterns_size][pattern_lines_size[patterns_size]], line);
        pattern_lines_size[patterns_size]++;
    }

    patterns_size++;

    // for (int i = 0; i < patterns_size; i++) {
    //     printf("\nPattern %d:\n", i + 1);

    //     for (int j = 0; j < pattern_lines_size[i]; j++) {
    //         printf("%s\n", patterns[i][j]);
    //     }
    // }

    int result = 0;

    for (int i = 0; i < patterns_size; i++) {
        printf("Pattern %d\n", i + 1);
        char** pattern = patterns[i];
        for (int j = 0; j < pattern_lines_size[i]; j++) {
            printf("%s\n", patterns[i][j]);
        }

        for (int y = 0; y < pattern_lines_size[i] - 1; y++) {
            char* row1 = pattern[y];
            char* row2 = pattern[y + 1];

            if (strcmp(row1, row2) == 0) {
                bool is_mirror = true;
                int j = 1;

                for (int ii = y - 1; ii >= 0; ii--) {
                    row1 = pattern[ii];

                    if (y + 1 + j >= pattern_lines_size[i]) {
                        break;
                    }

                    row2 = pattern[y + 1 + j];

                    if (strcmp(row1, row2) != 0) {
                        is_mirror = false;
                        break;
                    }

                    ++j;
                }

                if (is_mirror) {
                    result += (y + 1) * 100;
                }
            }
        }

        for (size_t x = 0; x < strlen(pattern[0]) - 1; x++) {
            char* col1 = malloc(sizeof(char) * pattern_lines_size[i]);
            char* col2 = malloc(sizeof(char) * pattern_lines_size[i]);

            for (int y = 0; y < pattern_lines_size[i]; y++) {
                col1[y] = pattern[y][x];
                col2[y] = pattern[y][x + 1];
            }

            if (strcmp(col1, col2) == 0) {
                bool is_mirror = true;
                int j = 1;

                for (int ii = x - 1; ii >= 0; ii--) {
                    char* col3 = malloc(sizeof(char) * pattern_lines_size[i]);
                    char* col4 = malloc(sizeof(char) * pattern_lines_size[i]);
                    bool over_flow = false;

                    for (int y = 0; y < pattern_lines_size[i]; y++) {
                        col3[y] = pattern[y][ii];

                        if (x + 1 + j >= strlen(pattern[y])) {
                            over_flow = true;
                            break;
                        }

                        col4[y] = pattern[y][x + 1 + j];
                    }

                    ++j;

                    if (over_flow) {
                        break;
                    }

                    if (strcmp(col3, col4) != 0) {
                        is_mirror = false;
                        break;
                    }
                }

                if (is_mirror) {
                    result += x + 1;
                }
            }
        }
    }

    printf("\nPart 1: %d\n", result);
    return 0;
}