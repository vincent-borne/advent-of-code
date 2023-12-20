#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/17/input.txt"

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
    int same_direction_count;
    int heat;
    enum Direction prev_direction;
} Coord;

bool is_out_of_bounds(int x, int y, int** layout) { return x < 0 || (unsigned int)x >= arrlen(layout[0]) || y < 0 || y >= arrlen(layout); }

Coord Coord_new(int x, int y, int same_direction_count, int heat, enum Direction prev_direction) {
    Coord c;
    c.x = x;
    c.y = y;
    c.same_direction_count = same_direction_count;
    c.heat = heat;
    c.prev_direction = prev_direction;
    return c;
}

int compare_heat(const void* a, const void* b) {
    Coord a_coord = *(const Coord*)a;
    Coord b_coord = *(const Coord*)b;
    return a_coord.heat - b_coord.heat;
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

    int** layout = NULL;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int* row = NULL;

        for (size_t i = 0; i < len; i++) {
            int heat = line[i] - '0';

            if (heat > 9 || heat < 0) {
                continue;
            }

            arrput(row, heat);
        }

        arrput(layout, row);
    }

    Coord start = Coord_new(0, 0, 0, 0, NONE);
    Coord current;
    Coord* queue = NULL;
    arrput(queue, start);

    struct {
        char* key;
        char value;
    }* visited = NULL;

    sh_new_arena(visited);

    while (arrlen(queue) > 0) {
        qsort(queue, arrlen(queue), sizeof(Coord), compare_heat);

        current = queue[0];
        arrdel(queue, 0);

        char key[200];
        sprintf(key, "%d,%d,%d,%d", current.x, current.y, current.same_direction_count, current.prev_direction);

        if (shgeti(visited, key) != -1) {
            continue;
        }

        shput(visited, key, 1);
        if (current.same_direction_count >= 3 && current.x == arrlen(layout[0]) - 1 && current.y == arrlen(layout) - 1) {
            printf("\nPart 2: %d\n", current.heat);
            return 0;
        }

        for (int i = 0; i < 4; i++) {
            if ((current.prev_direction == UP && i == DOWN) || (current.prev_direction == DOWN && i == UP) || (current.prev_direction == LEFT && i == RIGHT) ||
                (current.prev_direction == RIGHT && i == LEFT)) {
                continue;
            }

            int same_direction_count = current.same_direction_count;

            if ((int)current.prev_direction == i) {
                same_direction_count++;
            } else {
                if (same_direction_count < 3 && current.prev_direction != NONE) {
                    continue;
                }

                same_direction_count = 0;
            }

            if (same_direction_count > 9) {
                continue;
            }

            int new_x = current.x + directions[i][0];
            int new_y = current.y + directions[i][1];

            if (is_out_of_bounds(new_x, new_y, layout)) {
                continue;
            }

            Coord c = Coord_new(new_x, new_y, same_direction_count, current.heat + layout[new_y][new_x], i);
            arrput(queue, c);
        }
    }

    return 0;
}