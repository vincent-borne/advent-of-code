#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/04/input.txt"

void split(char* str, char* delim, char** result, int* result_size) {
    *result_size = -1;
    char* ptr = strtok(str, delim);

    while (ptr != NULL) {
        (*result_size)++;
        result[*result_size] = ptr;
        ptr = strtok(NULL, delim);
    }

    (*result_size)++;
}

void print_char_array(char** arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", arr[i]);
    }
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

    int result = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_game_size;
        char* split_game[20];
        split(line, ":", split_game, &split_game_size);

        int split_cards_size;
        char* split_cards[20];
        split(split_game[1], "|", split_cards, &split_cards_size);

        int winners_size = 0;
        char* winners[100];

        int split_numbers_size;
        char* split_numbers[20];
        split(split_cards[0], " ", split_numbers, &split_numbers_size);

        for (int j = 0; j < split_numbers_size; j++) {
            winners[winners_size] = split_numbers[j];
            winners_size++;
        }

        char numbers[100] = "";
        strcat(numbers, split_cards[1]);
        strcat(numbers, " ");

        int nb_of_winners = 0;

        for (int i = 0; i < winners_size; i++) {
            char withSpaces[100] = " ";
            strcat(withSpaces, winners[i]);
            strcat(withSpaces, " ");

            if (strstr(numbers, withSpaces) != NULL) {
                ++nb_of_winners;
            }
        }

        if (nb_of_winners > 0) {
            int card_result = 1;

            for (int i = 1; i < nb_of_winners; i++) {
                card_result *= 2;
            }

            result += card_result;
        }
    }

    printf("\nPart 1: %d\n", result);

    fclose(f);

    return 0;
}