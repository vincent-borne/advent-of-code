#include <stdio.h>
#include <stdlib.h>

#define INPUT "./src/02/input.txt"

int min(int a, int b) { return a < b ? a : b; }

int main() {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    int result = 0;
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, f)) > 0) {
        int index = 0;
        int size[3];
        int temp_index = 0;
        char temp[10];

        for (ssize_t i = 0; i <= linelen; i++) {
            if (line[i] == 'x' || line[i] == '\n' || i == linelen) {
                temp[temp_index] = '\0';
                temp_index = 0;

                size[index++] = atoi(temp);

                if (index == 3) {
                    break;
                }
            } else {
                temp[temp_index++] = line[i];
            }
        }

        int l = size[0];
        int w = size[1];
        int h = size[2];

        int lw = 2 * (l + w);
        int wh = 2 * (w + h);
        int hl = 2 * (h + l);
        int min_2 = min(lw, min(wh, hl));

        result += min_2 + l * w * h;
    }

    printf("Result: %d\n", result);

    fclose(f);
    return 0;
}