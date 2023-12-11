#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/11/input.txt"

int main(void) {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    int line_count = 0;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int galaxy_size = 141;
    int galaxy_count_x[galaxy_size];
    int galaxy_count_y[galaxy_size];
    char galaxy[galaxy_size][galaxy_size];

    for (int i = 0; i < galaxy_size; i++) {
        galaxy_count_x[i] = 0;
        galaxy_count_y[i] = 0;
    }

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        for (size_t x = 0; x < len; x++) {
            printf("%c", line[x]);
            if (line[x] == '#') {
                galaxy_count_x[x]++;
                galaxy_count_y[line_count]++;
            }

            galaxy[line_count][x] = line[x];
        }

        line_count++;
    }

    typedef struct {
        int x;
        int y;
        int type;
        int val;
    } Galaxy;

    int galaxies_count = 0;
    Galaxy galaxies[galaxy_size * galaxy_size];

    int x_size = 0;
    int y_size = 0;
    int galaxy_expanded_x_size = 0;
    int galaxy_expanded_y_size = 0;
    Galaxy galaxy_expanded[galaxy_size * 2][galaxy_size * 2];
    char t[galaxy_size * 2][galaxy_size * 2];

    for (int y = 0; y < galaxy_size; y++) {
        x_size = 0;

        for (int x = 0; x < galaxy_size; x++) {
            Galaxy g;
            g.x = x_size;
            g.y = y_size;
            g.type = -1;
            g.val = 0;

            char c = galaxy[y][x];

            if (c == '#') {
                g.type = galaxies_count + 1;
                galaxies[galaxies_count] = g;
                galaxies_count++;
            }

            if (galaxy_count_x[x] == 0) {
                t[y_size][x_size] = '.';
                galaxy_expanded[y_size][x_size] = g;
                x_size++;
                g.x = x_size;
            }

            t[y_size][x_size] = galaxy[y][x];
            galaxy_expanded[y_size][x_size] = g;
            x_size++;
        }

        if (galaxy_count_y[y] == 0) {
            printf("Adding empty line\n");
            printf("galaxy_count_y[%d]: %d\n", y, galaxy_count_y[y]);
            y_size++;

            for (int x = 0; x < x_size; x++) {
                Galaxy g;
                g.y = y_size;
                g.type = -1;
                g.x = x;
                g.val = 0;
                galaxy_expanded[y_size][x] = g;
                t[y_size][x] = '*';
            }
        }

        if (galaxy_expanded_x_size < x_size) {
            galaxy_expanded_x_size = x_size;
        }

        y_size++;
    }

    galaxy_expanded_y_size = y_size;

    for (int y = 0; y < galaxy_expanded_y_size; y++) {
        for (int x = 0; x < galaxy_expanded_x_size; x++) {
            printf("%c", t[y][x]);
        }

        printf("\n");
    }

    printf("Galaxy expanded size: %d,%d\n", galaxy_expanded_x_size, galaxy_expanded_y_size);

    {
        int result = 0;
        int found_pairs_count = 0;
        int found_pairs[100000][2];

        printf("Galaxies count: %d\n", galaxies_count);
        for (int y = 0; y < galaxies_count; y++) {
            Galaxy* g1 = &galaxies[y];

            for (int x = 0; x < galaxies_count; x++) {
                Galaxy* g2 = &galaxies[x];

                if (g1->type == g2->type) {
                    continue;
                }

                bool already_found = false;

                for (int k = 0; k < found_pairs_count; k++) {
                    if ((found_pairs[k][0] == g1->type && found_pairs[k][1] == g2->type) || (found_pairs[k][0] == g2->type && found_pairs[k][1] == g1->type)) {
                        already_found = true;
                        continue;
                    }
                }

                if (already_found) {
                    continue;
                }

                found_pairs[found_pairs_count][0] = g1->type;
                found_pairs[found_pairs_count][1] = g2->type;
                found_pairs_count++;

                int distance = abs(g1->x - g2->x) + abs(g1->y - g2->y);
                // printf("%d,%d = %d\n", g1->type, g2->type, distance);
                result += distance;
            }

            // printf("\n");
        }

        printf("Pairs count: %d\n", found_pairs_count);
        printf("\nPart 1: %d\n", result);
    }
    return 0;
}