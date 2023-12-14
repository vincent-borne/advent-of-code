#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

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

void print_map(char map[1000][1000], int map_x_size, int map_y_size) {
    for (int y = 0; y < map_y_size; y++) {
        for (int x = 0; x < map_x_size; x++) {
            printf("%c", map[y][x]);
        }

        printf("\n");
    }
}

void tilt_north(char map[1000][1000], int map_x_size, int map_y_size) {
    for (int y = 0; y < map_y_size; y++) {
        char* line = map[y];

        for (int x = 0; x < map_x_size; x++) {
            if (line[x] == 'O') {
                int lowest_y = y;

                for (int yy = y - 1; yy >= 0; yy--) {
                    if (map[yy][x] == '.') {
                        if (yy < lowest_y) {
                            lowest_y = yy;
                        }
                    } else {
                        break;
                    }
                }

                map[y][x] = '.';
                map[lowest_y][x] = 'O';
            }
        }
    }
}

void tilt_south(char map[1000][1000], int map_x_size, int map_y_size) {
    for (int y = map_y_size - 1; y >= 0; y--) {
        for (int x = 0; x < map_x_size; x++) {
            if (map[y][x] == 'O') {
                int max_y = y;

                for (int yy = y + 1; yy < map_y_size; yy++) {
                    if (map[yy][x] == '.') {
                        if (yy > max_y) {
                            max_y = yy;
                        }
                    } else {
                        break;
                    }
                }

                map[y][x] = '.';
                map[max_y][x] = 'O';
            }
        }
    }
}

void tilt_west(char map[1000][1000], int map_x_size, int map_y_size) {
    for (int y = 0; y < map_y_size; y++) {
        for (int x = 0; x < map_x_size; x++) {
            if (map[y][x] == 'O') {
                int lowest_x = x;

                for (int xx = x - 1; xx >= 0; xx--) {
                    if (map[y][xx] == '.') {
                        if (xx < lowest_x) {
                            lowest_x = xx;
                        }
                    } else {
                        break;
                    }
                }

                map[y][x] = '.';
                map[y][lowest_x] = 'O';
            }
        }
    }
}

void tilt_east(char map[1000][1000], int map_x_size, int map_y_size) {
    for (int y = 0; y < map_y_size; y++) {
        for (int x = map_x_size - 1; x >= 0; x--) {
            if (map[y][x] == 'O') {
                int max_x = x;

                for (int xx = x + 1; xx < map_x_size; xx++) {
                    if (map[y][xx] == '.') {
                        if (xx > max_x) {
                            max_x = xx;
                        }
                    } else {
                        break;
                    }
                }

                map[y][x] = '.';
                map[y][max_x] = 'O';
            }
        }
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

    int map_x_size = 0;
    int map_y_size = 0;
    char map[1000][1000];

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        map_x_size = 0;

        for (size_t i = 0; i < strlen(line); i++) {
            map[map_y_size][map_x_size] = line[i];
            ++map_x_size;
        }

        map_y_size++;
    }

    char* current_cycle = malloc(sizeof(char) * 1000000);

    int cycles_count = 0;

    struct {
        char* key;
        char value;
    }* hash = NULL;

    int to_found = -1;

    while (true) {
        for (size_t i = 0; i < strlen(current_cycle); i++) {
            current_cycle[i] = '\0';
        }

        if (to_found != -1 && cycles_count >= to_found) {
            break;
        }

        tilt_north(map, map_x_size, map_y_size);
        tilt_west(map, map_x_size, map_y_size);
        tilt_south(map, map_x_size, map_y_size);
        tilt_east(map, map_x_size, map_y_size);

        for (int y = 0; y < map_y_size; y++) {
            for (int x = 0; x < map_x_size; x++) {
                if (map[y][x] == 'O') {
                    char* str = malloc(sizeof(char) * 1000);
                    sprintf(str, "%d,%d|", x, y);
                    strcat(current_cycle, str);
                }
            }
        }

        if (shgeti(hash, current_cycle) != -1 && to_found == -1) {
            int i = shgeti(hash, current_cycle);
            int t = i;
            int every = cycles_count - i;

            while (t < 1000000000) {
                t += every;
            }

            t -= 1000000000;
            int nb_to_not_go_over = every - t;
            to_found = nb_to_not_go_over + cycles_count;
        }

        shput(hash, current_cycle, '1');

        ++cycles_count;
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

        result += O_count * i;
        ++i;
    }

    printf("\nPart 2: %d\n", result);
    return 0;
}