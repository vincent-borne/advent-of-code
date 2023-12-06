#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/05/test_input.txt"

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

void print_int_array(long long* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("arr[%i] = %lld\n", i, arr[i]);
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

    int seeds_size = 0;
    long long seeds[100][2];

    int interval_size = 0;
    long long interval[100][3];

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        if (strcmp(line, "") == 0) {
            continue;
        }

        int split_types_size;
        char* split_types[20];
        split(line, ":", split_types, &split_types_size);

        if (strcmp(split_types[0], "seeds") == 0) {
            int split_seeds_size;
            char* split_seeds[20];
            split(split_types[1], " ", split_seeds, &split_seeds_size);

            for (int i = 0; i < split_seeds_size - 1; i += 2) {
                seeds[seeds_size][0] = atoll(split_seeds[i]);
                seeds[seeds_size][1] = atoll(split_seeds[i + 1]);
                seeds_size++;
            }

            continue;
        }

        if (strstr(line, "map") != NULL) {
            for (int i = 0; i < seeds_size; i++) {
                long long seed[2];
                seed[0] = seeds[i][0];
                seed[1] = seeds[i][1];

                for (long long k = seed[0]; k < seed[1]; k++) {
                    for (int j = 0; j < interval_size; j++) {
                        long long destination = interval[j][0];
                        long long source = interval[j][1];
                        long long length = interval[j][2];

                        if (k >= source && k < (source + length)) {
                            long long r = k - source;
                            long long rr = destination + r;
                            seeds[i][0] = rr;
                            seeds[i][1] = 1;
                        }
                    }
                }
            }

            interval_size = 0;
        } else {
            int split_nb_size;
            char* split_nb[20];
            split(line, " ", split_nb, &split_nb_size);

            if (split_nb_size != 3) {
                continue;
            }

            long long destination = atoll(split_nb[0]);
            long long source = atoll(split_nb[1]);
            long long length = atoll(split_nb[2]);

            interval[interval_size][0] = destination;
            interval[interval_size][1] = source;
            interval[interval_size][2] = length;
            interval_size++;
        }
    }

    long long result = LLONG_MAX;

    for (int i = 0; i < seeds_size; i++) {
        if (seeds[i][0] < result) {
            result = seeds[i][0];
        }
    }

    printf("\nPart 1: %lld\n", result);

    fclose(f);

    return 0;
}