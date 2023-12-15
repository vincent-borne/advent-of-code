#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/13/input.txt"

typedef struct {
    char** lines;
    int reflection_rows;
    int reflection_cols;
    int new_reflection_rows;
    int new_reflection_cols;
} Pattern;

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

char permutation_char(char c) {
    if (c == '#') {
        c = '.';
    } else {
        c = '#';
    }

    return c;
}

int find_mirror_rows(Pattern* pattern, bool ignore_old_reflection_rows) {
    int lines_size = arrlen(pattern->lines);
    int result = 0;

    for (int y = 0; y < lines_size - 1; y++) {
        if (ignore_old_reflection_rows && pattern->reflection_rows == y) {
            continue;
        }

        char* row1 = pattern->lines[y];
        char* row2 = pattern->lines[y + 1];

        if (strcmp(row1, row2) == 0) {
            bool is_mirror = true;
            int j = 1;

            for (int ii = y - 1; ii >= 0; ii--) {
                row1 = pattern->lines[ii];

                if (y + 1 + j >= lines_size) {
                    break;
                }

                row2 = pattern->lines[y + 1 + j];

                if (strcmp(row1, row2) != 0) {
                    is_mirror = false;
                    break;
                }

                ++j;
            }

            if (is_mirror) {
                return y;
            }
        }
    }

    if (result == 0) {
        return -1;
    }

    return result;
}

int find_mirror_cols(Pattern* pattern, bool ignore_old_reflection_cols) {
    int lines_len = arrlen(pattern->lines);
    int cols_len = strlen(pattern->lines[0]);
    int result = 0;

    for (int x = 0; x < cols_len - 1; x++) {
        if (ignore_old_reflection_cols && pattern->reflection_cols == x) {
            continue;
        }

        char* col1 = malloc(sizeof(char) * lines_len);
        char* col2 = malloc(sizeof(char) * lines_len);

        for (int y = 0; y < lines_len; y++) {
            col1[y] = pattern->lines[y][x];
            col2[y] = pattern->lines[y][x + 1];
        }

        if (strcmp(col1, col2) == 0) {
            bool is_mirror = true;
            int j = 1;

            for (int ii = x - 1; ii >= 0; ii--) {
                char* col3 = malloc(sizeof(char) * lines_len);
                char* col4 = malloc(sizeof(char) * lines_len);
                bool over_flow = false;

                for (int y = 0; y < lines_len; y++) {
                    col3[y] = pattern->lines[y][ii];

                    if (((unsigned int)x + 1 + j) >= strlen(pattern->lines[y])) {
                        over_flow = true;
                        break;
                    }

                    col4[y] = pattern->lines[y][x + 1 + j];
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
                return x;
            }
        }
    }

    if (result == 0) {
        return -1;
    }

    return result;
}

int find_new_mirror_rows(Pattern* pattern) {
    int new_reflection_rows = -1;
    int lines_size = arrlen(pattern->lines);

    for (int y = 0; y < lines_size - 1; y++) {
        char* row1 = pattern->lines[y];

        for (int yy = y + 1; yy < lines_size; yy++) {
            char* row2 = pattern->lines[yy];

            int nb_diff = 0;
            int diff_index = -1;

            for (size_t x = 0; x < strlen(row1); x++) {
                if (row1[x] != row2[x]) {
                    nb_diff++;
                    diff_index = x;
                }
            }

            if (nb_diff == 1 && diff_index != -1) {
                row2[diff_index] = permutation_char(row2[diff_index]);
                new_reflection_rows = find_mirror_rows(pattern, true);
                row2[diff_index] = permutation_char(row2[diff_index]);

                if (new_reflection_rows == -1) {
                    row1[diff_index] = permutation_char(row1[diff_index]);
                    new_reflection_rows = find_mirror_rows(pattern, true);
                    row1[diff_index] = permutation_char(row1[diff_index]);
                }

                if (new_reflection_rows != -1) {
                    return new_reflection_rows;
                }
            }
        }
    }

    return new_reflection_rows;
}

int find_new_mirror_cols(Pattern* pattern) {
    int new_reflection_cols = -1;
    int lines_len = arrlen(pattern->lines);
    int cols_len = strlen(pattern->lines[0]);

    for (int x = 0; x < cols_len - 1; x++) {
        char* col1 = malloc(sizeof(char) * (lines_len + 1));

        for (int xx = x + 1; xx < cols_len; xx++) {
            char* col2 = malloc(sizeof(char) * (lines_len + 1));

            for (int y = 0; y < lines_len; y++) {
                col1[y] = pattern->lines[y][x];
                col2[y] = pattern->lines[y][xx];
            }

            int nb_diff = 0;
            int diff_index = -1;

            for (size_t x = 0; x < strlen(col1); x++) {
                if (col1[x] != col2[x]) {
                    nb_diff++;
                    diff_index = x;
                }
            }

            if (nb_diff == 1 && diff_index != -1) {
                pattern->lines[diff_index][xx] = permutation_char(pattern->lines[diff_index][xx]);
                new_reflection_cols = find_mirror_cols(pattern, true);
                pattern->lines[diff_index][xx] = permutation_char(pattern->lines[diff_index][xx]);

                if (new_reflection_cols == -1) {
                    pattern->lines[diff_index][x] = permutation_char(pattern->lines[diff_index][x]);
                    new_reflection_cols = find_mirror_cols(pattern, true);
                    pattern->lines[diff_index][x] = permutation_char(pattern->lines[diff_index][x]);
                }

                if (new_reflection_cols != -1) {
                    return new_reflection_cols;
                }
            }
        }
    }

    return new_reflection_cols;
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

    Pattern* patterns = NULL;

    Pattern p;
    p.lines = NULL;
    p.reflection_rows = -1;
    p.reflection_cols = -1;
    p.new_reflection_rows = -1;
    p.new_reflection_cols = -1;

    while ((read = getline(&line, &len, f)) != -1) {
        if (strcmp(line, "\n") == 0) {
            p.reflection_rows = find_mirror_rows(&p, false);
            p.reflection_cols = find_mirror_cols(&p, false);
            p.new_reflection_rows = find_new_mirror_rows(&p);
            p.new_reflection_cols = find_new_mirror_cols(&p);
            arrput(patterns, p);
            p.lines = NULL;
            continue;
        }

        strtok(line, "\n");

        char* str = malloc(sizeof(char) * strlen(line));
        strcpy(str, line);
        arrput(p.lines, str);
    }

    int result = 0;

    for (int i = 0; i < arrlen(patterns); i++) {
        if (patterns[i].new_reflection_rows != -1) {
            result += (patterns[i].new_reflection_rows + 1) * 100;
        }

        if (patterns[i].new_reflection_cols != -1) {
            result += patterns[i].new_reflection_cols + 1;
        }
    }

    printf("\nPart 2: %d\n", result);

    return 0;
}