#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/16/input.txt"

enum Direction { UP, DOWN, LEFT, RIGHT };
int directions[4][2] = {
    {0, -1},  // UP
    {0, 1},   // DOWN
    {-1, 0},  // LEFT
    {1, 0}    // RIGHT
};

typedef struct {
    int x;
    int y;
    int direction[2];
    enum Direction e_direction;
} Coord;

bool is_out_of_bounds(int x, int y, char** layout) { return x < 0 || (unsigned int)x >= strlen(layout[0]) || y < 0 || y >= arrlen(layout); }

Coord Coord_new(int x, int y, int direction[2], enum Direction e_direction) {
    Coord c;
    c.x = x;
    c.y = y;
    c.direction[0] = direction[0];
    c.direction[1] = direction[1];
    c.e_direction = e_direction;
    return c;
}

int do_path(char** layout, Coord start) {
    Coord* current;
    Coord* queue = NULL;
    arrput(queue, start);

    struct {
        char* key;
        char value;
    }* visited = NULL;

    sh_new_arena(visited);

    struct {
        char* key;
        char value;
    }* result = NULL;

    sh_new_arena(result);

    while (arrlen(queue) > 0) {
        current = &queue[0];

        char key[200];
        sprintf(key, "%d,%d,%d|", current->x, current->y, current->e_direction);

        if (shgeti(visited, key) != -1) {
            arrdel(queue, 0);
            continue;
        }

        shput(visited, key, 1);

        char key_result[200];
        sprintf(key_result, "%d,%d|", current->x, current->y);
        if (shgeti(result, key_result) == -1) {
            shput(result, key_result, 1);
        }

        int new_x = current->x;
        int new_y = current->y;

        if (layout[new_y][new_x] == '.') {
            new_x += current->direction[0];
            new_y += current->direction[1];

            if (!is_out_of_bounds(new_x, new_y, layout)) {
                Coord new_coord = Coord_new(new_x, new_y, current->direction, current->e_direction);
                arrput(queue, new_coord);
            }
        } else if (layout[new_y][new_x] == '|') {
            if (current->e_direction == UP || current->e_direction == LEFT || current->e_direction == RIGHT) {
                new_y = current->y - 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_up = Coord_new(new_x, new_y, directions[UP], UP);
                    arrput(queue, new_coord_up);
                }
            }

            if (current->e_direction == DOWN || current->e_direction == LEFT || current->e_direction == RIGHT) {
                new_y = current->y + 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_up = Coord_new(new_x, new_y, directions[DOWN], DOWN);
                    arrput(queue, new_coord_up);
                }
            }
        } else if (layout[new_y][new_x] == '-') {
            if (current->e_direction == LEFT || current->e_direction == UP || current->e_direction == DOWN) {
                new_x = current->x - 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_left = Coord_new(new_x, new_y, directions[LEFT], LEFT);
                    arrput(queue, new_coord_left);
                }
            }

            if (current->e_direction == RIGHT || current->e_direction == UP || current->e_direction == DOWN) {
                new_x = current->x + 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_right = Coord_new(new_x, new_y, directions[RIGHT], RIGHT);
                    arrput(queue, new_coord_right);
                }
            }

        } else if (layout[new_y][new_x] == '/') {
            if (current->e_direction == RIGHT) {
                new_y = current->y - 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_up = Coord_new(new_x, new_y, directions[UP], UP);
                    arrput(queue, new_coord_up);
                }
            } else if (current->e_direction == LEFT) {
                new_y = current->y + 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_down = Coord_new(new_x, new_y, directions[DOWN], DOWN);
                    arrput(queue, new_coord_down);
                }
            } else if (current->e_direction == DOWN) {
                new_x = current->x - 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_left = Coord_new(new_x, new_y, directions[LEFT], LEFT);
                    arrput(queue, new_coord_left);
                }
            } else if (current->e_direction == UP) {
                new_x = current->x + 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_right = Coord_new(new_x, new_y, directions[RIGHT], RIGHT);
                    arrput(queue, new_coord_right);
                }
            }
        } else if (layout[new_y][new_x] == '\\') {
            if (current->e_direction == RIGHT) {
                new_y = current->y + 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_down = Coord_new(new_x, new_y, directions[DOWN], DOWN);
                    arrput(queue, new_coord_down);
                }
            } else if (current->e_direction == LEFT) {
                new_y = current->y - 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_up = Coord_new(new_x, new_y, directions[UP], UP);
                    arrput(queue, new_coord_up);
                }
            } else if (current->e_direction == DOWN) {
                new_x = current->x + 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_right = Coord_new(new_x, new_y, directions[RIGHT], RIGHT);
                    arrput(queue, new_coord_right);
                }
            } else if (current->e_direction == UP) {
                new_x = current->x - 1;

                if (!is_out_of_bounds(new_x, new_y, layout)) {
                    Coord new_coord_left = Coord_new(new_x, new_y, directions[LEFT], LEFT);
                    arrput(queue, new_coord_left);
                }
            }
        }

        arrdel(queue, 0);
    }

    return shlen(result);
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

    char** layout = NULL;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");
        char* str = malloc(sizeof(char) * strlen(line));
        strcpy(str, line);
        arrput(layout, str);
    }

    for (int i = 0; i < arrlen(layout); i++) {
        printf("%s\n", layout[i]);
    }

    int result = 0;
    for (int y = 0; y < arrlen(layout); y++) {
        for (size_t x = 0; x < strlen(layout[y]); x++) {
            if ((y > 0 && y < arrlen(layout) - 1) && (x > 0 && x < strlen(layout[y]) - 1)) {
                continue;
            }

            Coord start;
            int titles;

            if (x == 0 && y == 0) {
                start = Coord_new(x, y, directions[RIGHT], RIGHT);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }

                start = Coord_new(x, y, directions[DOWN], DOWN);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (x == strlen(layout[y]) - 1 && y == 0) {
                start = Coord_new(x, y, directions[LEFT], LEFT);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }

                start = Coord_new(x, y, directions[DOWN], DOWN);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (x == 0 && y == arrlen(layout) - 1) {
                start = Coord_new(x, y, directions[RIGHT], RIGHT);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }

                start = Coord_new(x, y, directions[UP], UP);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (x == strlen(layout[y]) - 1 && y == arrlen(layout) - 1) {
                start = Coord_new(x, y, directions[LEFT], LEFT);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }

                start = Coord_new(x, y, directions[UP], UP);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (y == 0) {
                start = Coord_new(x, y, directions[DOWN], DOWN);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (x == 0) {
                start = Coord_new(x, y, directions[RIGHT], RIGHT);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (y == arrlen(layout) - 1) {
                start = Coord_new(x, y, directions[UP], UP);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            } else if (x == strlen(layout[y]) - 1) {
                start = Coord_new(x, y, directions[LEFT], LEFT);
                titles = do_path(layout, start);

                if (titles > result) {
                    result = titles;
                }
            }
        }
    }

    printf("\nPart 2: %d\n", result);
    return 0;
}