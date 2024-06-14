#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/21/input.txt"

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
int directions[4][2] = {
    {0, -1},  // UP
    {0, 1},   // DOWN
    {-1, 0},  // LEFT
    {1, 0}    // RIGHT
};

typedef struct {
    int x;
    int y;
} Coord;

bool is_out_of_bounds(int x, int y, char** layout) { return x < 0 || (unsigned int)x >= arrlen(layout[0]) || y < 0 || y >= arrlen(layout); }

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

    char** layout = NULL;
    Coord start;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        char* row = NULL;

        for (size_t i = 0; i < len; i++) {
            if (line[i] == 'S') {
                start.x = i;
                start.y = arrlen(layout);
            }

            arrput(row, line[i]);
        }

        arrput(layout, row);
    }

    Coord* plots = NULL;
    arrput(plots, start);

    for (int i = 0; i < 64; i++) {
        size_t size = arrlen(plots);

        struct {
            char* key;
            char value;
        }* visited = NULL;

        sh_new_arena(visited);

        for (size_t j = 0; j < size; j++) {
            Coord current = plots[j];

            for (int k = 0; k < 4; k++) {
                int new_x = current.x + directions[k][0];
                int new_y = current.y + directions[k][1];

                if (is_out_of_bounds(new_x, new_y, layout)) {
                    continue;
                }

                if (layout[new_y][new_x] == '#') {
                    continue;
                }

                char key[200];
                sprintf(key, "%d,%d", new_x, new_y);

                if (shgeti(visited, key) != -1) {
                    continue;
                }

                shput(visited, key, 1);

                Coord new_coord;
                new_coord.x = new_x;
                new_coord.y = new_y;

                arrput(plots, new_coord);
            }
        }

        arrdeln(plots, 0, size);
    }

    printf("\nPart 1: %td\n", arrlen(plots));

    return 0;
}