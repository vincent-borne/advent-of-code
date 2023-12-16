#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/12/test_input.txt"

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

bool is_solution_valid(char* line, int line_size, int arrangments[10], int arrangments_size) {
    int broken_count = 0;
    int found_arrangments_size = 0;
    int found_arrangments[100];

    for (int i = 0; i < line_size; i++) {
        if (line[i] == '#') {
            broken_count++;
        } else if (broken_count > 0) {
            found_arrangments[found_arrangments_size] = broken_count;
            found_arrangments_size++;
            broken_count = 0;
        }
    }

    if (broken_count > 0) {
        found_arrangments[found_arrangments_size] = broken_count;
        found_arrangments_size++;
    }

    if (found_arrangments_size != arrangments_size) {
        return false;
    }

    for (int i = 0; i < found_arrangments_size; i++) {
        if (found_arrangments[i] != arrangments[i]) {
            return false;
        }
    }

    return true;
}

bool should_continue(char* line, int line_size, int arrangments[10], int arrangments_size) {
    int broken_count = 0;
    int found_arrangments_size = 0;
    int found_arrangments[100];

    for (int i = 0; i < line_size; i++) {
        if (line[i] == '#') {
            broken_count++;
        } else if (line[i] == '?') {
            break;
        }

        if (line[i] == '.' && broken_count > 0) {
            found_arrangments[found_arrangments_size] = broken_count;
            found_arrangments_size++;

            broken_count = 0;
        }
    }

    if (found_arrangments_size > arrangments_size) {
        return false;
    }

    for (int i = 0; i < found_arrangments_size; i++) {
        if (found_arrangments[i] != arrangments[i]) {
            return false;
        }
    }

    // for (int i = 0; i < line_size - 1; i++) {
    //     if (line[i]) {
    //     }
    // }

    return true;
}

void create_solutions(char* line, int line_size, int index, int arrangments[10], int arrangments_size, int* result) {
    line[line_size] = '\0';
    // printf("\n%s\n", line);

    if (index >= line_size) {
        if (is_solution_valid(line, line_size, arrangments, arrangments_size)) {
            (*result)++;
        }

        return;
    }

    if (line[index] == '?') {
        line[index] = '.';
        if (should_continue(line, line_size, arrangments, arrangments_size)) {
            create_solutions(line, line_size, index + 1, arrangments, arrangments_size, result);
        }

        line[index] = '#';
        if (should_continue(line, line_size, arrangments, arrangments_size)) {
            create_solutions(line, line_size, index + 1, arrangments, arrangments_size, result);
        }

        line[index] = '?';
    } else {
        create_solutions(line, line_size, index + 1, arrangments, arrangments_size, result);
    }
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

    int lines_size = 0;
    int lines_nb_size[1001];
    char* lines[1001];
    int arrangments_size[1001];
    int arrangments[1001][100];

    for (int i = 0; i < 1001; i++) {
        arrangments_size[i] = 0;
        lines_nb_size[i] = 0;
    }

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_path_size;
        char* split_path[2];
        split(line, " ", split_path, &split_path_size);

        lines[lines_size] = malloc(sizeof(char) * (strlen(split_path[0]) + 1) * 5);
        strcpy(lines[lines_size], split_path[0]);
        strcat(lines[lines_size], "?");
        strcat(lines[lines_size], split_path[0]);
        strcat(lines[lines_size], "?");
        strcat(lines[lines_size], split_path[0]);
        strcat(lines[lines_size], "?");
        strcat(lines[lines_size], split_path[0]);
        strcat(lines[lines_size], "?");
        strcat(lines[lines_size], split_path[0]);
        lines_nb_size[lines_size] = strlen(lines[lines_size]);

        int split_a_size;
        char* split_a[10];
        split(split_path[1], ",", split_a, &split_a_size);

        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < split_a_size; i++) {
                arrangments[lines_size][arrangments_size[lines_size]] = atoi(split_a[i]);
                arrangments_size[lines_size]++;
            }
        }

        lines_size++;
    }

    int result = 0;
    printf("Lines: %d\n", lines_size);
    for (int i = 0; i < lines_size; i++) {
        printf("%d) %s ", i + 1, lines[i]);
        for (int j = 0; j < arrangments_size[i]; j++) {
            printf("%d,", arrangments[i][j]);
        }
        printf(" = ");

        int saved = result;
        create_solutions(lines[i], lines_nb_size[i], 0, arrangments[i], arrangments_size[i], &result);
        printf("%d\n", result - saved);
    }

    printf("\nPart 2: %d\n", result);
    return 0;
}