#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/05/input.txt"

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

long long get_location(long long seed, int maps_size, int maps_arr_size[7], long long maps[7][100][3]) {
    long long location = seed;
    // printf("Seed = %i\n", seed);

    for (int i = 0; i < maps_size; i++) {
        // printf("\tMap %d\n", i);
        for (int j = 0; j < maps_arr_size[i]; j++) {
            long long destination = maps[i][j][0];
            long long source = maps[i][j][1];
            long long length = maps[i][j][2];
            // printf("\t\tMap[%d][%d] = %lld %lld %lld\n", i, j, destination, source, length);

            if (location >= source && location < (source + length)) {
                long long r = location - source;
                long long rr = destination + r;
                location = rr;
                // printf("\t\tNew seed map[%d][%d] = %d\n", i, j, location);
                break;
            }
        }
    }

    return location;
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
    long long seeds[100];

    int maps_size = -1;
    int maps_arr_size[7];
    for (int i = 0; i < 7; i++) {
        maps_arr_size[i] = 0;
    }
    long long maps[7][100][3];

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

            for (int i = 0; i < split_seeds_size; i++) {
                seeds[seeds_size] = atoll(split_seeds[i]);
                seeds_size++;
            }

            continue;
        }

        if (strstr(line, "map") != NULL) {
            ++maps_size;
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

            maps[maps_size][maps_arr_size[maps_size]][0] = destination;
            maps[maps_size][maps_arr_size[maps_size]][1] = source;
            maps[maps_size][maps_arr_size[maps_size]][2] = length;
            ++maps_arr_size[maps_size];
        }
    }

    long long result = LLONG_MAX;

    for (int k = 0; k < seeds_size - 1; k += 2) {
        long long seed = seeds[k];
        long long range = seeds[k + 1];
        printf("Seed = %lld ; Range = %lld\n", seed, range);

        for (int x = 0; x < range; x++) {
            long long c_seed = seed + x;
            long long location = get_location(c_seed, maps_size, maps_arr_size, maps);

            if (location < result) {
                printf("New result: %lld\n", location);
                result = location;
            }
        }
    }

    printf("\nPart 2: %lld\n", result);

    fclose(f);

    return 0;
}