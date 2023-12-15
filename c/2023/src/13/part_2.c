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

int find_mirror_rows_ignores_og(char** pattern, int pattern_lines_size, int* og_cols, int* og_rows) {
    int result = 0;

    for (int y = 0; y < pattern_lines_size - 1; y++) {
        char* row1 = pattern[y];
        char* row2 = pattern[y + 1];
        int og1 = og_cols[y];
        int og2 = og_rows[y];
        int real_og = -1;
        if (og1 > 0) {
            real_og = og1 - 1;
        } else if (og2 > 0) {
            real_og = og2 - 1;
        }
        printf("Real og: %d\n", real_og);

        if (strcmp(row1, row2) == 0) {
            bool is_mirror = true;
            int j = 1;

            for (int ii = y - 1; ii >= 0; ii--) {
                row1 = pattern[ii];

                if (y + 1 + j >= pattern_lines_size) {
                    break;
                }

                row2 = pattern[y + 1 + j];

                if (strcmp(row1, row2) != 0) {
                    is_mirror = false;
                    break;
                }

                ++j;
            }

            if (is_mirror && real_og != y) {
                result += (y + 1) * 100;
                break;
            }
        }
    }

    return result;
}

int find_mirror_cols_ignores_og(char** pattern, int pattern_lines_size, int* og_cols, int* og_rows) {
    int result = 0;

    for (size_t x = 0; x < strlen(pattern[0]) - 1; x++) {
        char* col1 = malloc(sizeof(char) * pattern_lines_size);
        char* col2 = malloc(sizeof(char) * pattern_lines_size);
        int og1 = og_cols[x];
        int og2 = og_rows[x];
        int real_og = -1;
        if (og1 > 0) {
            real_og = og1 - 1;
        } else if (og2 > 0) {
            real_og = og2 - 1;
        }

        for (int y = 0; y < pattern_lines_size; y++) {
            col1[y] = pattern[y][x];
            col2[y] = pattern[y][x + 1];
        }

        if (strcmp(col1, col2) == 0) {
            bool is_mirror = true;
            int j = 1;

            for (int ii = x - 1; ii >= 0; ii--) {
                char* col3 = malloc(sizeof(char) * pattern_lines_size);
                char* col4 = malloc(sizeof(char) * pattern_lines_size);
                bool over_flow = false;

                for (int y = 0; y < pattern_lines_size; y++) {
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

            if (is_mirror && real_og != x) {
                result += x + 1;
                break;
            }
        }
    }

    return result;
}

int find_mirror_rows(char** pattern, int pattern_lines_size) {
    int result = 0;

    for (int y = 0; y < pattern_lines_size - 1; y++) {
        char* row1 = pattern[y];
        char* row2 = pattern[y + 1];

        if (strcmp(row1, row2) == 0) {
            bool is_mirror = true;
            int j = 1;

            for (int ii = y - 1; ii >= 0; ii--) {
                row1 = pattern[ii];

                if (y + 1 + j >= pattern_lines_size) {
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
                break;
            }
        }
    }

    return result;
}

int find_mirror_cols(char** pattern, int pattern_lines_size) {
    int result = 0;

    for (size_t x = 0; x < strlen(pattern[0]) - 1; x++) {
        char* col1 = malloc(sizeof(char) * pattern_lines_size);
        char* col2 = malloc(sizeof(char) * pattern_lines_size);

        for (int y = 0; y < pattern_lines_size; y++) {
            col1[y] = pattern[y][x];
            col2[y] = pattern[y][x + 1];
        }

        if (strcmp(col1, col2) == 0) {
            bool is_mirror = true;
            int j = 1;

            for (int ii = x - 1; ii >= 0; ii--) {
                char* col3 = malloc(sizeof(char) * pattern_lines_size);
                char* col4 = malloc(sizeof(char) * pattern_lines_size);
                bool over_flow = false;

                for (int y = 0; y < pattern_lines_size; y++) {
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
                break;
            }
        }
    }

    return result;
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
    int found_patterns[1000];

    for (int i = 0; i < 1000; i++) {
        pattern_lines_size[i] = 0;
        found_patterns[i] = false;
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
    //     for (int j = 0; j < pattern_lines_size[i]; j++) {
    //         printf("%s\n", patterns[i][j]);
    //     }
    // }

    int found_cols[patterns_size];
    int found_rows[patterns_size];

    for (int i = 0; i < patterns_size; i++) {
        found_cols[i] = find_mirror_cols(patterns[i], pattern_lines_size[i]);
        found_rows[i] = (find_mirror_rows(patterns[i], pattern_lines_size[i]) / 100);
        printf("Found cols: %d\n", found_cols[i]);
        printf("Found rows: %d\n", find_mirror_rows(patterns[i], pattern_lines_size[i]));
    }

    // return 0;

    int result = 0;

    for (int i = 0; i < patterns_size; i++) {
    end_loop:
        if (i >= patterns_size) {
            break;
        }
        // if (i != 7) continue;
        printf("Pattern %d\n", i + 1);

        for (int j = 0; j < pattern_lines_size[i] - 1; j++) {
            char* line1 = patterns[i][j];

            for (int jj = j + 1; jj < pattern_lines_size[i]; jj++) {
                char* line2 = patterns[i][jj];
                int diff = -1;
                int nb_diff = 0;

                for (size_t k = 0; k < strlen(line1); k++) {
                    if (line1[k] != line2[k]) {
                        diff = k;
                        nb_diff++;
                    }
                }

                int og_result_cols = find_mirror_cols(patterns[i], pattern_lines_size[i]);
                int og_result_rows = find_mirror_rows(patterns[i], pattern_lines_size[i]);

                if (nb_diff == 1 && diff > -1) {
                    // printf("Line 1: %s\nLine 2: %s\n", line1, line2);
                    int tries = 0;

                    if (line2[diff] == '.') {
                        line2[diff] = '#';
                    } else {
                        line2[diff] = '.';
                    }
                redo: {}
                    // printf("Try: %d\n", tries);
                    int result_cols = find_mirror_cols_ignores_og(patterns[i], pattern_lines_size[i], found_cols, found_rows);
                    int result_rows = find_mirror_rows_ignores_og(patterns[i], pattern_lines_size[i], found_cols, found_rows);

                    int found = false;

                    // printf("Result cols: %d, %d\n", result_cols, og_result_cols);
                    if (result_cols > 0 && result_cols != og_result_cols) {
                        found = true;
                        result += result_cols;
                    }

                    // printf("Result rows: %d, %d\n", result_rows, og_result_rows);
                    if (result_rows > 0 && result_rows != og_result_rows) {
                        result += result_rows;
                        found = true;
                        // }
                        // printf("Result: %d + %d\n", result_rows, result_cols);
                    }

                    if (found) {
                        printf("Found row\n");
                        found_patterns[i] = true;
                        ++i;
                        goto end_loop;
                    } else if (tries == 0) {
                        ++tries;
                        if (line1[diff] == '.') {
                            line1[diff] = '#';
                        } else {
                            line1[diff] = '.';
                        }

                        // Rechange
                        if (line2[diff] == '.') {
                            line2[diff] = '#';
                        } else {
                            line2[diff] = '.';
                        }
                        goto redo;
                    }
                }
            }
        }
    }

    int res_2 = 0;

    for (int i = 0; i < patterns_size; i++) {
        if (found_patterns[i]) {
            res_2 += find_mirror_rows(patterns[i], pattern_lines_size[i]);
            res_2 += find_mirror_cols(patterns[i], pattern_lines_size[i]);
        }
    }

    for (int i = 0; i < patterns_size; i++) {
    end_loop_2:
        if (i >= patterns_size) {
            break;
        }
        if (found_patterns[i]) {
            continue;
        }
        // if (i != 7) continue;
        printf("Col pattern %d\n", i + 1);

        for (size_t k = 0; k < strlen(patterns[i][0]) - 1; k++) {
            // printf("!!! Col %zu\n", k);
            char* col1 = malloc(sizeof(char) * (pattern_lines_size[i] + 1));

            for (size_t jj = k + 1; jj < strlen(patterns[i][0]); jj++) {
                char* col2 = malloc(sizeof(char) * (pattern_lines_size[i] + 1));

                for (int j = 0; j < pattern_lines_size[i]; j++) {
                    col1[j] = patterns[i][j][k];
                    col2[j] = patterns[i][j][jj];
                }

                int diff = -1;
                int nb_diff = 0;

                for (size_t k = 0; k < strlen(col1); k++) {
                    // printf("Col1: %c\nCol2: %c\n", col1[k], col2[k]);
                    if (col1[k] != col2[k]) {
                        diff = k;
                        nb_diff++;
                    }
                }

                // printf("\nDiff: %d\n", nb_diff);
                // printf("Col %zu: %s\nCol %zu: %s\n", k, col1, jj, col2);
                int og_result_cols = find_mirror_cols(patterns[i], pattern_lines_size[i]);
                int og_result_rows = find_mirror_rows(patterns[i], pattern_lines_size[i]);

                if (nb_diff == 1 && diff > -1) {
                    // printf("Col %d: %s\nCol %d: %s\n\n", k, col1, jj, col2);
                    // printf("\n\nCol %zu\n", k + 1);
                    int tries = 0;

                    if (col2[diff] == '.') {
                        col2[diff] = '#';
                        patterns[i][diff][jj] = '#';
                    } else {
                        // printf("Tries: %d\n", tries);
                        // printf("i: %d, dif: %lu, jj: %lu\n", i, diff, jj);
                        col2[diff] = '.';
                        patterns[i][diff][jj] = '.';
                    }

                    do {
                        int result_cols = find_mirror_cols_ignores_og(patterns[i], pattern_lines_size[i], found_cols, found_rows);
                        int result_rows = find_mirror_rows_ignores_og(patterns[i], pattern_lines_size[i], found_cols, found_rows);

                        int found = false;

                        // printf("Result: %d + %d\n", result_rows, result_cols);
                        // printf("Result cols: %d, %d\n", result_cols, og_result_cols);
                        if (result_cols > 0 && result_cols != og_result_cols) {
                            found = true;
                            result += result_cols;
                        }

                        // printf("Result rows: %d, %d\n", result_rows, og_result_rows);
                        if (result_rows > 0 && result_rows != og_result_rows) {
                            result += result_rows;
                            found = true;
                            // }
                            // printf("Result: %d + %d\n", result_rows, result_cols);
                        }

                        // printf("Found: %d\n", found);
                        if (found) {
                            printf("Found col\n");
                            ++i;
                            goto end_loop_2;
                        } else if (tries == 0) {
                            if (col1[diff] == '.') {
                                col1[diff] = '#';
                                patterns[i][diff][k] = '#';
                            } else {
                                col1[diff] = '.';
                                patterns[i][diff][k] = '.';
                            }

                            // printf("2, i: %d, k: %lu, jj: %lu\n", i, diff, jj);
                            if (col2[diff] == '.') {
                                col2[diff] = '#';
                                patterns[i][diff][jj] = '#';
                            } else {
                                col2[diff] = '.';
                                patterns[i][diff][jj] = '.';
                            }
                            // printf("Go to redo_2\n");
                            // goto redo_2;
                        }
                        ++tries;
                        // printf("Tries: %d\n", tries);
                    } while (tries < 2);
                }
            }
            // printf("Col %zu: %s\nCol %zu: %s\n\n", k, col1, k + 1, col2);
        }
    }

    for (int i = 0; i < patterns_size; i++) {
        if (!found_patterns[i]) {
            res_2 += find_mirror_rows(patterns[i], pattern_lines_size[i]);
            res_2 += find_mirror_cols(patterns[i], pattern_lines_size[i]);
        }
    }

    // for (int i = 0; i < patterns_size; i++) {
    //     for (int j = 0; j < pattern_lines_size[i]; j++) {
    //         printf("%s\n", patterns[i][j]);
    //     }
    // }

    // int result = 0;
    // for (int i = 0; i < patterns_size; i++) {
    //     printf("Pattern %d\n", i + 1);
    //     result += find_mirror_rows(patterns[i], pattern_lines_size[i]);
    //     result += find_mirror_cols(patterns[i], pattern_lines_size[i]);
    // }

    printf("\nPart 2: %d\n", result);
    return 0;
}