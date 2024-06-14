#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/23/test_input.txt"

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
    int len;
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

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int* row = NULL;

        for (size_t i = 0; i < len; i++) {
            arrput(row, line[i]);
        }

        arrput(layout, row);
    }

    Coord start;
    start.x = 0;
    start.y = 1;
    start.len = 0;

    Coord current;
    Coord* queue = NULL;
    arrput(queue, start);

    struct {
        char* key;
        char value;
    }* visited = NULL;

    sh_new_arena(visited);

    int result = 0;

    while (arrlen(queue) > 0) {
        current = queue[0];
        arrdel(queue, 0);

        char key[200];
        sprintf(key, "%d,%d,%d", current.x, current.y, current.len);

        if (shgeti(visited, key) != -1) {
            continue;
        }

        shput(visited, key, 1);

        for (int i = 0; i < 4; i++) {
            int new_x = current.x + directions[i][0];
            int new_y = current.y + directions[i][1];

            if (is_out_of_bounds(new_x, new_y, layout)) {
                continue;
            }

            if (layout[new_y][new_x] == '#') {
                continue;
            }

            Coord new_coord;
            new_coord.x = new_x;
            new_coord.y = new_y;
            new_coord.len = current.len + 1;

            arrput(queue, new_coord);
        }
    }

    return 0;
}