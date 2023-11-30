#include <stdio.h>
#include <stdlib.h>

#define INPUT "./src/01/input.txt"

int elves[2000];
int elves_size = 0;

int part_1() {
    int max = 0;

    for (int i = 0; i < elves_size; i++) {
        if (elves[i] > max) {
            max = elves[i];
        }
    }

    return max;
}

int compare(const void* a, const void* b) { return *(const int*)b - *(const int*)a; }

int part_2() {
    qsort(elves, elves_size, sizeof(int), compare);

    return elves[0] + elves[1] + elves[2];
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

    elves[elves_size] = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        if (line[0] == '\n') {
            elves_size++;
            elves[elves_size] = 0;
            continue;
        }

        int value = atoi(line);
        elves[elves_size] += value;
    }

    elves_size++;

    printf("Part 1: %d\n", part_1());
    printf("Part 2: %d\n", part_2());

    fclose(f);

    return 0;
}