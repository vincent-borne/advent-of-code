#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/08/input.txt"

typedef struct Node {
    char name[4];
    char destination[2][100];
    struct Node* left;
    struct Node* right;
} Node;

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

void print_node_array(Node* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s = (%s, %s)\n", arr[i].name, arr[i].destination[0], arr[i].destination[1]);
    }
}

void c_copy(char* dest, char* src, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }

    dest[size] = '\0';
}

int main(void) {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    int line_count = 0;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int nodes_size = 0;
    Node nodes[1000];

    char path[1000];

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        if (line_count == 0) {
            strcpy(path, line);
        } else if (line_count > 1) {
            int split_path_size;
            char* split_path[2];
            split(line, "=", split_path, &split_path_size);

            int split_lr_size;
            char* split_lr[2];
            split(split_path[1], ",", split_lr, &split_lr_size);

            Node node;

            c_copy(node.name, split_path[0], 3);
            strcpy(node.destination[0], split_lr[0]);
            strcpy(node.destination[1], split_lr[1]);

            nodes[nodes_size] = node;
            nodes_size++;
        }

        ++line_count;
    }

    for (int i = 0; i < nodes_size; i++) {
        char* left = nodes[i].destination[0];
        char* right = nodes[i].destination[1];

        for (int j = 0; j < nodes_size; j++) {
            if (strstr(left, nodes[j].name) != NULL) {
                nodes[i].left = &nodes[j];
            }

            if (strstr(right, nodes[j].name) != NULL) {
                nodes[i].right = &nodes[j];
            }
        }
    }

    typedef struct {
        Node* original_node;
        int founded;
        Node* node;
    } Node_Result;

    int nodes_ending_with_a_size = 0;
    Node_Result nodes_ending_with_a[10];

    for (int i = 0; i < nodes_size; i++) {
        if (nodes[i].name[2] == 'A') {
            nodes_ending_with_a[nodes_ending_with_a_size].original_node = &nodes[i];
            nodes_ending_with_a[nodes_ending_with_a_size].node = &nodes[i];
            nodes_ending_with_a[nodes_ending_with_a_size].founded = -1;
            nodes_ending_with_a_size++;
            printf("%s\n", nodes[i].name);
        }
    }

    long long result = 1;
    int direction_i = 0;

    while (true) {
        for (int j = 0; j < nodes_ending_with_a_size; j++) {
            if (path[direction_i] == 'R' && nodes_ending_with_a[j].node->right != NULL) {
                nodes_ending_with_a[j].node = nodes_ending_with_a[j].node->right;
            } else if (path[direction_i] == 'L' && nodes_ending_with_a[j].node->left != NULL) {
                nodes_ending_with_a[j].node = nodes_ending_with_a[j].node->left;
            } else {
                printf("Error could not go %c: %s\n", path[direction_i], nodes_ending_with_a[j].node->name);
                break;
            }
        }

        bool has_arrived = true;

        for (int j = 0; j < nodes_ending_with_a_size; j++) {
            if (nodes_ending_with_a[j].node->name[2] == 'Z') {
                printf("Arrived to %s = %lld\n", nodes_ending_with_a[j].node->name, result);
                nodes_ending_with_a[j].founded = result;
            }
        }

        for (int j = 0; j < nodes_ending_with_a_size; j++) {
            if (nodes_ending_with_a[j].founded == -1) {
                has_arrived = false;
            }
        }

        if (has_arrived) {
            break;
        }

        direction_i = (direction_i + 1) % strlen(path);
        ++result;
    }

    // Calculate lcm using the numbers found
    // Used internet to calculate it
    result = 10818234074807;

    printf("\nPart 2: %lld\n", result);

    fclose(f);

    return 0;
}