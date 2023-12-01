#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/01/input.txt"

char* numbers[1000];
int numbers_size = 0;

int main(void) {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    int result = 0;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    numbers[numbers_size] = malloc(sizeof(char) * 2);

    while ((read = getline(&line, &len, f)) != -1) {
        for (size_t i = 0; i < len; i++) {
            int value = line[i] - '0';

            if (value > 0 && value < 10) {
                if (strlen(numbers[numbers_size]) == 0) {
                    numbers[numbers_size][0] = line[i];
                } else {
                    numbers[numbers_size][1] = line[i];
                }
            }
        }

        if (strlen(numbers[numbers_size]) == 1) {
            numbers[numbers_size][1] = numbers[numbers_size][0];
        }

        result += atoi(numbers[numbers_size]);

        numbers_size++;
        numbers[numbers_size] = malloc(sizeof(char) * 2);
        line = NULL;
    }

    printf("Part 1: %d\n", result);

    fclose(f);

    return 0;
}