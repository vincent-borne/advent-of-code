#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/03/input.txt"

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

    char engine[2000][2000];
    int engine_size_x = 0;
    int engine_size_y = 0;

    struct Number {
        int start_x;
        int end_x;
        int y;
        char value[4];
        int value_size;
    };

    struct Number numbers[2000];
    int numbers_size = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        struct Number number;
        number.start_x = -1;
        number.end_x = -1;
        number.y = -1;

        for (size_t i = 0; i < len; i++) {
            engine[engine_size_y][i] = line[i];

            if (line[i] - '0' >= 0 && line[i] - '0' <= 9) {
                if (number.y == -1 && number.start_x == -1) {
                    number.y = engine_size_y;
                    number.start_x = i;
                }

                number.value[number.value_size] = line[i];
                number.value_size++;
            } else if (atoi(number.value) > 0) {
                number.value[number.value_size] = '\0';
                number.end_x = i - 1;
                numbers[numbers_size] = number;
                numbers_size++;

                number.value[0] = '\0';
                number.value[1] = '\0';
                number.value[2] = '\0';
                number.value[3] = '\0';
                number.value_size = 0;
                number.start_x = -1;
                number.end_x = -1;
                number.y = -1;
            }
        }

        engine_size_y++;
        engine_size_x = len;
    }

    int result = 0;
    int moves[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    int already_found[numbers_size];

    for (int i = 0; i < numbers_size; i++) {
        already_found[i] = 0;
    }

    for (int y = 0; y < engine_size_y; y++) {
        for (int x = 0; x < engine_size_x; x++) {
            char c = engine[y][x];

            if ((c - '0' >= 0 && c - '0' <= 9) || engine[y][x] == '.' || engine[y][x] == '\0') {
                continue;
            }

            for (int i = 0; i < 8; i++) {
                int new_x = x + moves[i][1];
                int new_y = y + moves[i][0];

                for (int j = 0; j < numbers_size; j++) {
                    if (already_found[j] == 0 && new_x >= numbers[j].start_x && new_x <= numbers[j].end_x && new_y == numbers[j].y) {
                        already_found[j] = 1;

                        int new_value = atoi(numbers[j].value);
                        result += new_value;
                    }
                }
            }
        }
    }

    printf("\nPart 1: %d\n", result);

    fclose(f);

    return 0;
}