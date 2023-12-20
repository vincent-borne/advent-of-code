#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/18/input.txt"

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

    int size = 2000;
    char layout[size][size];
    int x = 500;
    int y = 500;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            layout[i][j] = '.';
        }
    }

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int last_x = x;
        int last_y = y;

        int split_path_size;
        char* split_path[3];
        split(line, " ", split_path, &split_path_size);

        int length = atoi(split_path[1]);

        if (split_path[0][0] == 'R') {
            for (int i = 0; i < length; i++) {
                x++;
            }
        } else if (split_path[0][0] == 'L') {
            for (int i = 0; i < length; i++) {
                if (x - 1 >= 0) {
                    x--;
                }
            }
        } else if (split_path[0][0] == 'D') {
            for (int i = 0; i < length; i++) {
                y++;
            }
        } else if (split_path[0][0] == 'U') {
            for (int i = 0; i < length; i++) {
                if (y - 1 >= 0) {
                    y--;
                }
            }
        }

        if (last_x < x) {
            for (int xx = last_x; xx <= x; xx++) {
                layout[y][xx] = '#';
            }
        } else if (last_x > x) {
            for (int xx = last_x; xx >= x; xx--) {
                layout[y][xx] = '#';
            }
        }

        if (last_y < y) {
            for (int yy = last_y; yy <= y; yy++) {
                layout[yy][x] = '#';
            }
        } else if (last_y > y) {
            for (int yy = last_y; yy >= y; yy--) {
                layout[yy][x] = '#';
            }
        }
    }

    int result = 0;

    typedef struct {
        int x;
        int y;
    } Coord;

    int directions[4][2] = {
        {0, -1},  // UP
        {0, 1},   // DOWN
        {-1, 0},  // LEFT
        {1, 0}    // RIGHT
    };

    Coord start;
    start.x = 501;
    start.y = 501;

    Coord current;
    Coord* queue = NULL;
    arrput(queue, start);

    struct {
        char* key;
        char value;
    }* visited = NULL;

    sh_new_arena(visited);

    while (arrlen(queue) > 0) {
        current = queue[0];
        arrdel(queue, 0);

        char key[200];
        sprintf(key, "%d,%d", current.x, current.y);

        if (shgeti(visited, key) != -1) {
            continue;
        }

        shput(visited, key, 1);

        for (int i = 0; i < 4; i++) {
            int new_x = current.x + directions[i][0];
            int new_y = current.y + directions[i][1];

            if (layout[new_y][new_x] == '#') {
                continue;
            }

            Coord new_coord;
            new_coord.x = new_x;
            new_coord.y = new_y;
            arrput(queue, new_coord);
        }
    }

    for (int y = 250; y < 570; y++) {
        for (int x = 450; x < 800; x++) {
            if (layout[y][x] == '#') {
                result++;
            }
            printf("\e[0m%c", layout[y][x]);
        }
        printf("\n");
    }

    result += shlen(visited);

    printf("Part 1: %d\n", result);
    return 0;
}