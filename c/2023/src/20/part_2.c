#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define INPUT "./src/19/test_input.txt"

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

    Workflow* workflows = NULL;

    while ((read = getline(&line, &len, f)) != -1) {
        if (strcmp(line, "\n") == 0) {
            break;
        }

        strtok(line, "\n");

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

    typedef struct {
        Workflow* wf;
        int range_x[2];
        int range_m[2];
        int range_a[2];
        int range_s[2];
    } WfSearch;

    WfSearch current;
    WfSearch* queue = NULL;

    WfSearch start;
    start.wf = find_workflow(workflows, "in");
    start.range_x[0] = 1;
    start.range_x[1] = 4000;
    start.range_m[0] = 1;
    start.range_m[1] = 4000;
    start.range_a[0] = 1;
    start.range_a[1] = 4000;
    start.range_s[0] = 1;
    start.range_s[1] = 4000;

    arrput(queue, start);

    WfSearch* result_wf = NULL;

    while (arrlen(queue) > 0) {
        current = queue[0];
        arrdel(queue, 0);

        for (int i = 0; i < current.wf->value_size; i++) {
            Rule* r = &current.wf->value[i];

            if (r->symbol == '<') {
                if (r->category == X) {
                    current.range_x[1] = r->value - 1;
                } else if (r->category == M) {
                    current.range_m[1] = r->value - 1;
                } else if (r->category == A) {
                    current.range_a[1] = r->value - 1;
                } else if (r->category == S) {
                    current.range_s[1] = r->value - 1;
                }
            } else if (r->symbol == '>') {
                if (r->category == X) {
                    current.range_x[0] = r->value + 1;
                } else if (r->category == M) {
                    current.range_m[0] = r->value + 1;
                } else if (r->category == A) {
                    current.range_a[0] = r->value + 1;
                } else if (r->category == S) {
                    current.range_s[0] = r->value + 1;
                }
            }

            if (current.range_x[0] > current.range_x[1] || current.range_m[0] > current.range_m[1] || current.range_a[0] > current.range_a[1] || current.range_s[0] > current.range_s[1]) {
                continue;
            }

            if (r->to_workflow[0] == 'A') {
                arrput(result_wf, current);
            } else if (r->to_workflow[0] != 'R') {
                Workflow* wf = find_workflow(workflows, r->to_workflow);
                WfSearch wf_search;
                wf_search.wf = wf;
                wf_search.range_x[0] = current.range_x[0];
                wf_search.range_x[1] = current.range_x[1];
                wf_search.range_m[0] = current.range_m[0];
                wf_search.range_m[1] = current.range_m[1];
                wf_search.range_a[0] = current.range_a[0];
                wf_search.range_a[1] = current.range_a[1];
                wf_search.range_s[0] = current.range_s[0];
                wf_search.range_s[1] = current.range_s[1];

                arrput(queue, wf_search);
            }
        }
    }

    int min_range_x = INT_MAX;
    int max_range_x = INT_MIN;
    int min_range_m = INT_MAX;
    int max_range_m = INT_MIN;
    int min_range_a = INT_MAX;
    int max_range_a = INT_MIN;
    int min_range_s = INT_MAX;
    int max_range_s = INT_MIN;

    long long result = 0;

    for (int i = 0; i < arrlen(result_wf); i++) {
        printf("Workflow: %s\n", result_wf[i].wf->key);
        // if (result_wf[i].range_x[0] > result_wf[i].range_x[1]) {
        //     continue;
        // }

        // if (result_wf[i].range_m[0] > result_wf[i].range_m[1]) {
        //     continue;
        // }

        // if (result_wf[i].range_a[0] > result_wf[i].range_a[1]) {
        //     continue;
        // }

        // if (result_wf[i].range_s[0] > result_wf[i].range_s[1]) {
        //     continue;
        // }

        if (result_wf[i].range_x[1] < min_range_x) {
            min_range_x = result_wf[i].range_x[1];
        }
        if (result_wf[i].range_x[0] > max_range_x) {
            max_range_x = result_wf[i].range_x[0];
        }

        if (result_wf[i].range_m[1] < min_range_m) {
            min_range_m = result_wf[i].range_m[1];
        }
        if (result_wf[i].range_m[0] > max_range_m) {
            max_range_m = result_wf[i].range_m[0];
        }
        if (result_wf[i].range_a[1] < min_range_a) {
            min_range_a = result_wf[i].range_a[1];
        }
        if (result_wf[i].range_a[0] > max_range_a) {
            max_range_a = result_wf[i].range_a[0];
        }
        if (result_wf[i].range_s[1] < min_range_s) {
            min_range_s = result_wf[i].range_s[1];
        }
        if (result_wf[i].range_s[0] > max_range_s) {
            max_range_s = result_wf[i].range_s[0];
        }

        printf("Range X: %d - %d\n", result_wf[i].range_x[0], result_wf[i].range_x[1]);
        printf("Range M: %d - %d\n", result_wf[i].range_m[0], result_wf[i].range_m[1]);
        printf("Range A: %d - %d\n", result_wf[i].range_a[0], result_wf[i].range_a[1]);
        printf("Range S: %d - %d\n", result_wf[i].range_s[0], result_wf[i].range_s[1]);
        printf("\n");
        long long x = abs(result_wf[i].range_x[1] - result_wf[i].range_x[0]);
        long long m = abs(result_wf[i].range_m[1] - result_wf[i].range_m[0]);
        long long a = abs(result_wf[i].range_a[1] - result_wf[i].range_a[0]);
        long long s = abs(result_wf[i].range_s[1] - result_wf[i].range_s[0]);

        result += (x * m * a * s);
        printf("range_x %d\n", x);
        printf("range_m %d\n", m);
        printf("range_a %d\n", a);
        printf("range_s %d\n", s);
        printf("Result: %lld\n", result);
    }

    printf("Range X: %d - %d\n", min_range_x, max_range_x);
    printf("Range M: %d - %d\n", min_range_m, max_range_m);
    printf("Range A: %d - %d\n", min_range_a, max_range_a);
    printf("Range S: %d - %d\n", min_range_s, max_range_s);

    // 256 000 000 000 000
    // 167 409 079 868 000
    //   2 060 678 831 400
    //       1 118 172 760
    //         653 215 756
    //  15 286 585 174 056
    //       1 449 783 348
    //  20 637 971 073 036
    //  39 115 385 337 432

    // printf("min_range_x: %d\n", abs(min_range_x - max_range_x));
    // printf("min_range_m: %d\n", abs(min_range_m - max_range_m));
    // printf("min_range_a: %d\n", abs(min_range_a - max_range_a));
    // printf("min_range_s: %d\n", abs(min_range_s - max_range_s));
    printf("Part 2: %lld\n", result);
    return 0;
}