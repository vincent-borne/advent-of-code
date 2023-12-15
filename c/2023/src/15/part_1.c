#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    int result = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_path_size;
        char* split_path[100000];
        split(line, ",", split_path, &split_path_size);

        for (int i = 0; i < split_path_size; i++) {
            int hash = calculate_hash(split_path[i]);

            result += hash;
        }
    }

    printf("\nPart 1: %d\n", result);
    return 0;
}