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
            // printf("%s %s %s\n", node.name, node.destination[0], node.destination[1]);

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

    Node* current_node = NULL;

    for (int i = 0; i < nodes_size; i++) {
        if (strcmp(nodes[i].name, "AAA") == 0) {
            current_node = &nodes[i];
            break;
        }
    }

    int result = 0;
    int direction_i = 0;

    while (strcmp(current_node->name, "ZZZ") != 0) {
        // printf("Going %s -> %s\n", path[direction_i] == 'R' ? "right" : "left", current_node->name);

        if (path[direction_i] == 'R' && current_node->right != NULL) {
            current_node = current_node->right;
        } else if (path[direction_i] == 'L' && current_node->left != NULL) {
            current_node = current_node->left;
        } else {
            printf("Error could not go %c: %s\n", path[direction_i], current_node->name);
            break;
        }

        direction_i = (direction_i + 1) % strlen(path);
        ++result;
    }

    // printf("Arrived to %s\n", current_node->name);

    printf("\nPart 1: %d\n", result);

    fclose(f);

    return 0;
}