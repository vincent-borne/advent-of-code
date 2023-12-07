#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/07/input.txt"

enum hand_kind { HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, FULL_HOUSE, FOUR_OF_A_KIND, FIVE_OF_A_KIND };

typedef struct {
    int value;
    char hand[6];
    enum hand_kind kind;
} Hand;

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

void print_hand_array(Hand* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s %d, kind = %d\n", arr[i].hand, arr[i].value, arr[i].kind);
    }
}

char card_order[5] = {'A', 'K', 'Q', 'J', 'T'};

int sort_hands(const void* a, const void* b) {
    Hand* hand_a = (Hand*)a;
    Hand* hand_b = (Hand*)b;

    if (hand_a->kind == hand_b->kind) {
        for (int i = 0; i < 5; i++) {
            if (hand_a->hand[i] == hand_b->hand[i]) {
                continue;
            }

            if (hand_a->hand[i] >= 'A' && hand_a->hand[i] <= 'Z' && hand_b->hand[i] >= 'A' && hand_b->hand[i] <= 'Z') {
                int index_a = -1;
                int index_b = -1;

                for (int k = 0; k < 5; k++) {
                    if (hand_a->hand[i] == card_order[k]) {
                        index_a = k;
                    }

                    if (hand_b->hand[i] == card_order[k]) {
                        index_b = k;
                    }

                    if (index_a != -1 && index_b != -1) {
                        break;
                    }
                }

                return index_b - index_a;
            }

            return hand_a->hand[i] - hand_b->hand[i];
        }
    }

    return hand_a->kind - hand_b->kind;
}

enum hand_kind get_kind(char* hand) {
    int size = 0;
    char cards[5];
    int counters[5];

    for (size_t i = 0; i < 5; i++) {
        int find_match = false;
        for (int j = 0; j < 5; j++) {
            if (cards[j] == hand[i]) {
                counters[j]++;
                find_match = true;
                break;
            }
        }

        if (!find_match) {
            cards[size] = hand[i];
            counters[size] = 1;
            ++size;
        }
    }

    bool has_three_of_a_kind = false;
    bool has_one_pair = false;
    bool has_two_pair = false;

    for (int i = 0; i < size; i++) {
        if (counters[i] == 5) {
            return FIVE_OF_A_KIND;
        } else if (counters[i] == 4) {
            return FOUR_OF_A_KIND;
        } else if (counters[i] == 3) {
            has_three_of_a_kind = true;
        } else if (counters[i] == 2) {
            if (has_one_pair) {
                has_two_pair = true;
            } else {
                has_one_pair = true;
            }
        }
    }

    if (has_three_of_a_kind && has_one_pair) {
        return FULL_HOUSE;
    }

    if (has_two_pair) {
        return TWO_PAIR;
    }

    if (has_three_of_a_kind) {
        return THREE_OF_A_KIND;
    }

    if (has_one_pair) {
        return ONE_PAIR;
    }

    return HIGH_CARD;
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

    int hands_size = 0;
    Hand hands[1000];

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_hands_size;
        char* split_hands[2];
        split(line, " ", split_hands, &split_hands_size);

        Hand hand;
        strcpy(hand.hand, split_hands[0]);
        hand.value = atoi(split_hands[1]);
        hand.kind = get_kind(split_hands[0]);

        hands[hands_size] = hand;
        hands_size++;
    }

    qsort(hands, hands_size, sizeof(Hand), sort_hands);
    print_hand_array(hands, hands_size);

    int result = 0;

    for (int i = 0; i < hands_size; i++) {
        result += hands[i].value * (i + 1);
    }

    printf("\nPart 1: %d\n", result);

    fclose(f);

    return 0;
}