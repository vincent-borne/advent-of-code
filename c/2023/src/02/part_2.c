#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/02/input.txt"

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

void print_char_array(char** arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", arr[i]);
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

    int result = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_game_size;
        char* split_game[2];
        split(line, ":", split_game, &split_game_size);

        int split_rounds_size;
        char* split_rounds[20];
        split(split_game[1], ";", split_rounds, &split_rounds_size);

        int r = 0, g = 0, b = 0;

        for (int i = 0; i < split_rounds_size; i++) {
            int split_colors_size;
            char* split_colors[20];
            split(split_rounds[i], ",", split_colors, &split_colors_size);

            for (int j = 0; j < split_colors_size; j++) {
                int split_values_size;
                char* split_values[20];
                split(split_colors[j], " ", split_values, &split_values_size);

                int value = atoi(split_values[0]);

                if (strcmp(split_values[1], "red") == 0 && value > r) {
                    r = value;
                }

                if (strcmp(split_values[1], "green") == 0 && value > g) {
                    g = value;
                }

                if (strcmp(split_values[1], "blue") == 0 && value > b) {
                    b = value;
                }
            }
        }

        result += r * g * b;
    }

    printf("\nPart 2: %d\n", result);

    fclose(f);

    return 0;
}