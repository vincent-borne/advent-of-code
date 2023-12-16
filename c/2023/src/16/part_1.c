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

    Coord start = Coord_new(0, 0, directions[RIGHT], RIGHT);

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
            printf("Already visited: %s\n", key);
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

    printf("\n\n");

    for (int y = 0; y < arrlen(layout); y++) {
        for (size_t x = 0; x < strlen(layout[y]); x++) {
            char key_result[200];
            sprintf(key_result, "%zu,%d|", x, y);

            if (shgeti(result, key_result) != -1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }

    printf("\n\n");
    int result2 = 0;

    for (int y = 0; y < arrlen(layout); y++) {
        for (size_t x = 0; x < strlen(layout[y]); x++) {
            int count = 0;
            char og_c = layout[y][x];
            char c = layout[y][x];

            for (int i = 0; i < 4; i++) {
                char key_result[200];
                sprintf(key_result, "%zu,%d,%d|", x, y, i);

                if (shgeti(visited, key_result) != -1) {
                    count++;

                    if (i == UP) {
                        c = '^';
                    } else if (i == DOWN) {
                        c = 'v';
                    } else if (i == LEFT) {
                        c = '<';
                    } else if (i == RIGHT) {
                        c = '>';
                    }
                }
            }

            // 45,16
            if (count > 0) {
                result2++;
            }

            if (og_c != '|' && og_c != '-' && og_c != '\\' && og_c != '/') {
                if (count > 1) {
                    printf("%d", count);
                } else if (count == 1) {
                    printf("%c", c);
                } else {
                    printf("%c", og_c);
                }
            } else {
                printf("%c", og_c);
            }
        }
        printf("\n");
    }

    printf("\nPart 1: %td, %td, %d\n", shlen(result), shlen(visited), result2);
    return 0;
}