#include <assert.h>
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

char card_order[13] = {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};

int sort_hands(const void* a, const void* b) {
    Hand* hand_a = (Hand*)a;
    Hand* hand_b = (Hand*)b;

    if (hand_a->kind == hand_b->kind) {
        for (int i = 0; i < 5; i++) {
            if (hand_a->hand[i] == hand_b->hand[i]) {
                continue;
            }

            int index_a = -1;
            int index_b = -1;

            for (int k = 0; k < 13; k++) {
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
    }

    return hand_a->kind - hand_b->kind;
}

enum hand_kind get_kind(char hand[5]) {
    int size = 0;
    char cards[5] = {'\0', '\0', '\0', '\0', '\0'};
    int counters[5] = {0, 0, 0, 0, 0};

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
            for (int i = 0; i < size; i++) {
                if (cards[i] == 'J') {
                    return FIVE_OF_A_KIND;
                }
            }

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
        for (int i = 0; i < size; i++) {
            if (cards[i] == 'J') {
                return FIVE_OF_A_KIND;
            }
        }

        return FULL_HOUSE;
    }

    if (has_three_of_a_kind) {
        for (int i = 0; i < size; i++) {
            if (cards[i] == 'J') {
                return FOUR_OF_A_KIND;
            }
        }

        return THREE_OF_A_KIND;
    }

    if (has_two_pair) {
        for (int i = 0; i < size; i++) {
            // If J is the pair
            if (cards[i] == 'J' && counters[i] == 2) {
                return FOUR_OF_A_KIND;
            } else if (cards[i] == 'J' && counters[i] == 1) {
                return FULL_HOUSE;
            }
        }

        return TWO_PAIR;
    }

    if (has_one_pair) {
        for (int i = 0; i < size; i++) {
            if (cards[i] == 'J') {
                return THREE_OF_A_KIND;
            }
        }

        return ONE_PAIR;
    }

    // Last case, if only one J and high card
    for (int i = 0; i < size; i++) {
        if (cards[i] == 'J') {
            return ONE_PAIR;
        }
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

    printf("\nPart 2: %d\n", result);

    assert(get_kind("A1234") == HIGH_CARD);
    assert(get_kind("J1234") == ONE_PAIR);
    assert(get_kind("1J234") == ONE_PAIR);
    assert(get_kind("12J34") == ONE_PAIR);
    assert(get_kind("123J4") == ONE_PAIR);
    assert(get_kind("1234J") == ONE_PAIR);
    assert(get_kind("11343") == TWO_PAIR);
    assert(get_kind("J1134") == THREE_OF_A_KIND);
    assert(get_kind("113J3") == FULL_HOUSE);
    assert(get_kind("112J3") == THREE_OF_A_KIND);
    assert(get_kind("11113") == FOUR_OF_A_KIND);
    assert(get_kind("111J3") == FOUR_OF_A_KIND);
    assert(get_kind("1JJJ3") == FOUR_OF_A_KIND);
    assert(get_kind("JJ334") == FOUR_OF_A_KIND);
    assert(get_kind("JJ333") == FIVE_OF_A_KIND);
    assert(get_kind("33J33") == FIVE_OF_A_KIND);
    assert(get_kind("JJJJ3") == FIVE_OF_A_KIND);
    assert(get_kind("J3333") == FIVE_OF_A_KIND);

    fclose(f);

    return 0;
}