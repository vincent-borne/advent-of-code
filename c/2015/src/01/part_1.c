#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/01/input.txt"

int main() {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    int result = 0;
    char c;

    while ((c = fgetc(f)) != EOF) {
        if (c == '(') {
            result++;
        } else if (c == ')') {
            result--;
        }
    }

    printf("Result: %d\n", result);

    fclose(f);
    return 0;
}