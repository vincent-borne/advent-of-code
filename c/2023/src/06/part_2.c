#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/06/input_2.txt"

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

    int line_count = 0;

    int time_size = 0;
    long long time[5];
    int distance_size = 0;
    long long distance[5];

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        if (line_count == 0) {
            int split_times_size;
            char* split_times[20];
            split(line, " ", split_times, &split_times_size);

            for (int i = 1; i < split_times_size; i++) {
                time[time_size] = atoll(split_times[i]);
                time_size++;
            }
        } else if (line_count == 1) {
            int split_distances_size;
            char* split_distances[20];
            split(line, " ", split_distances, &split_distances_size);

            for (int i = 1; i < split_distances_size; i++) {
                distance[distance_size] = atoll(split_distances[i]);
                distance_size++;
            }
        }

        line_count++;
    }

    print_int_array(distance, distance_size);
    int nb_size = time_size;
    long long nb[5];

    for (int i = 0; i < nb_size; i++) {
        nb[i] = 0;
    }

    for (int i = 0; i < time_size; i++) {
        for (long long j = 0; j < time[i]; j++) {
            long long speed = j;
            long long time_left = time[i] - j;
            long long new_distance = speed * time_left;

            if (new_distance > distance[i]) {
                nb[i]++;
            }
        }
    }

    long long result = 1;

    for (int i = 0; i < nb_size; i++) {
        result *= nb[i];
    }

    printf("\nPart 2: %lld\n", result);

    fclose(f);

    return 0;
}