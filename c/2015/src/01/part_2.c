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

    int index = 0;
    int result = 0;
    char c;

    while ((c = fgetc(f)) != EOF) {
        index++;

        if (c == '(') {
            result++;
        } else if (c == ')') {
            result--;
        }

        if (result == -1) {
            printf("Result: %d\n", index);
            break;
        }
    }

    fclose(f);
    return 0;
}