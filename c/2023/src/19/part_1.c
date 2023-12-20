#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/19/input.txt"

enum Letters { X, M, A, S, R };

typedef struct {
    enum Letters category;
    char symbol;
    int value;
    char* to_workflow;
} Rule;

typedef struct {
    char* key;
    int value_size;
    Rule value[100];
} Workflow;

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

Workflow* find_workflow(Workflow* workflows, char* key) {
    for (int i = 0; i < arrlen(workflows); i++) {
        if (strcmp(workflows[i].key, key) == 0) {
            return &workflows[i];
        }
    }

    printf("Error: workflow '%s' not found\n", key);

    return NULL;
}

int check_rules(Workflow* workflows, int value_arr[4]) {
    bool is_valid = false;
    int result = 0;

    Workflow* current = find_workflow(workflows, "in");

    while (true) {
        for (int i = 0; i < current->value_size; i++) {
            if (current->value[i].category == R) {
                if (strcmp(current->value[i].to_workflow, "R") == 0) {
                    goto after_loop;
                }

                if (strcmp(current->value[i].to_workflow, "A") == 0) {
                    is_valid = true;
                    goto after_loop;
                }

                current = find_workflow(workflows, current->value[i].to_workflow);
                break;
            }

            int value = value_arr[current->value[i].category];

            if (current->value[i].symbol == '<') {
                if (value < current->value[i].value) {
                    if (strcmp(current->value[i].to_workflow, "R") == 0) {
                        goto after_loop;
                    }

                    if (strcmp(current->value[i].to_workflow, "A") == 0) {
                        is_valid = true;
                        goto after_loop;
                    }

                    current = find_workflow(workflows, current->value[i].to_workflow);
                    break;
                }
            } else if (current->value[i].symbol == '>') {
                if (value > current->value[i].value) {
                    if (strcmp(current->value[i].to_workflow, "R") == 0) {
                        goto after_loop;
                    }

                    if (strcmp(current->value[i].to_workflow, "A") == 0) {
                        is_valid = true;
                        goto after_loop;
                    }

                    current = find_workflow(workflows, current->value[i].to_workflow);
                    break;
                }
            }
        }
    }

after_loop:
    if (is_valid) {
        result += value_arr[0] + value_arr[1] + value_arr[2] + value_arr[3];
    }

    return result;
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

    bool read_rules = true;

    Workflow* workflows = NULL;

    int result = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        if (strcmp(line, "\n") == 0) {
            printf("Read rules\n");
            read_rules = false;
            continue;
        }

        strtok(line, "\n");

        if (!read_rules) {
            int split_value_size;
            char* split_value[4];
            split(line, ",", split_value, &split_value_size);

            int value[4];

            for (int i = 0; i < split_value_size; i++) {
                int split_eq_size;
                char* split_eq[2];
                split(split_value[i], "=", split_eq, &split_eq_size);

                if (split_eq[0][0] == '{') {
                    split_eq[0]++;
                }

                if (split_eq[1][strlen(split_eq[1]) - 1] == '}') {
                    split_eq[1][strlen(split_eq[1]) - 1] = '\0';
                }

                value[i] = atoi(split_eq[1]);
            }

            result += check_rules(workflows, value);

            continue;
        }

        int split_wf_size;
        char* split_wf[2];
        split(line, "{", split_wf, &split_wf_size);

        Workflow workflow;
        workflow.key = malloc(sizeof(char) * strlen(split_wf[0]));
        strcpy(workflow.key, split_wf[0]);

        int split_rule_size;
        char* split_rule[2];
        split(split_wf[1], ",", split_rule, &split_rule_size);

        workflow.value_size = 0;

        for (int i = 0; i < split_rule_size; i++) {
            Rule r;

            if (strstr(split_rule[i], "}") != NULL) {
                split_rule[i][strlen(split_rule[i]) - 1] = '\0';
                r.to_workflow = malloc(sizeof(char) * strlen(split_rule[i]));
                strcpy(r.to_workflow, split_rule[i]);
                r.category = R;
                r.symbol = '\0';
                r.value = -1;
            } else {
                if (split_rule[i][0] == 'x') {
                    r.category = X;
                } else if (split_rule[i][0] == 'm') {
                    r.category = M;
                } else if (split_rule[i][0] == 'a') {
                    r.category = A;
                } else if (split_rule[i][0] == 's') {
                    r.category = S;
                }

                r.symbol = split_rule[i][1];

                char* ptr = &split_rule[i][2];

                int split_to_size;
                char* split_to[2];
                split(ptr, ":", split_to, &split_to_size);

                r.value = atoi(split_to[0]);
                r.to_workflow = malloc(sizeof(char) * strlen(split_to[1]));
                strcpy(r.to_workflow, split_to[1]);
            }

            workflow.value[workflow.value_size] = r;
            workflow.value_size++;
        }

        arrput(workflows, workflow);
    }

    printf("Part 1: %d\n", result);
    return 0;
}