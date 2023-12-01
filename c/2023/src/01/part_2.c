#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/01/input.txt"

char* numbers[1000];
int numbers_size = 0;

void add_number(char* str, char val, char* buf, int* buf_size, char* number) {
    if (strstr(buf, str) == NULL) {
        return;
    }

    for (int j = 0; j < *buf_size; j++) {
        buf[j] = '\0';
    }

    *buf_size = 0;

    if (strlen(number) == 0) {
        number[0] = val;
    } else {
        number[1] = val;
    }
}

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

    char buf[100];
    int buf_size = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        for (size_t i = 0; i < len; i++) {
            int value = line[i] - '0';

            if (value > 0 && value < 10) {
                if (strlen(numbers[numbers_size]) == 0) {
                    numbers[numbers_size][0] = line[i];
                } else {
                    numbers[numbers_size][1] = line[i];
                }

                continue;
            }

            buf[buf_size] = line[i];
            buf_size++;

            add_number("one", '1', buf, &buf_size, numbers[numbers_size]);
            add_number("two", '2', buf, &buf_size, numbers[numbers_size]);
            add_number("three", '3', buf, &buf_size, numbers[numbers_size]);
            add_number("four", '4', buf, &buf_size, numbers[numbers_size]);
            add_number("five", '5', buf, &buf_size, numbers[numbers_size]);
            add_number("six", '6', buf, &buf_size, numbers[numbers_size]);
            add_number("seven", '7', buf, &buf_size, numbers[numbers_size]);
            add_number("eight", '8', buf, &buf_size, numbers[numbers_size]);
            add_number("nine", '9', buf, &buf_size, numbers[numbers_size]);

            if (buf_size == 0) {
                buf[buf_size] = line[i];
                buf_size++;
            }
        }

        if (strlen(numbers[numbers_size]) == 1) {
            numbers[numbers_size][1] = numbers[numbers_size][0];
        }

        result += atoi(numbers[numbers_size]);

        numbers_size++;
        numbers[numbers_size] = malloc(sizeof(char) * 2);
        buf_size = 0;
        line = NULL;
    }

    printf("Part 2: %d\n", result);

    fclose(f);

    return 0;
}