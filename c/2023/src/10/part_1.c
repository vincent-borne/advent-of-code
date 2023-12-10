#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/10/input.txt"

int types_size = 6;
enum PipeType { V, H, L, J, S, F };

typedef struct Pipe {
    int x;
    int y;
    int links_size;
    struct Pipe** links;
    enum PipeType type;
    bool is_start;
    char letter;
} Pipe;

bool can_go_right(Pipe pipes[200][200], Pipe* pipe, int x_limit, int y_limit) {
    int new_x = pipe->x + 1;
    int new_y = pipe->y;

    if (new_x >= 0 && new_x < x_limit && new_y >= 0 && new_y < y_limit) {
        Pipe new_pipe = pipes[new_y][new_x];

        if (new_pipe.letter == '.') {
            return false;
        }

        if (pipe->type == V || pipe->type == J || pipe->type == S) {
            return false;
        } else if (new_pipe.type == H || new_pipe.type == J || new_pipe.type == S) {
            return true;
        }
    }

    return false;
}

bool can_go_left(Pipe pipes[200][200], Pipe* pipe, int x_limit, int y_limit) {
    int new_x = pipe->x - 1;
    int new_y = pipe->y;

    if (new_x >= 0 && new_x < x_limit && new_y >= 0 && new_y < y_limit) {
        Pipe new_pipe = pipes[new_y][new_x];

        if (new_pipe.letter == '.') {
            return false;
        }

        if (pipe->type == V || pipe->type == L || pipe->type == F) {
            return false;
        } else if (new_pipe.type == H || new_pipe.type == L || new_pipe.type == F) {
            return true;
        }
    }

    return false;
}

bool can_go_up(Pipe pipes[200][200], Pipe* pipe, int x_limit, int y_limit) {
    int new_x = pipe->x;
    int new_y = pipe->y - 1;

    if (new_x >= 0 && new_x < x_limit && new_y >= 0 && new_y < y_limit) {
        Pipe new_pipe = pipes[new_y][new_x];

        if (new_pipe.letter == '.') {
            return false;
        }

        if (pipe->type == H || pipe->type == S || pipe->type == F) {
            return false;
        } else if (new_pipe.type == V || new_pipe.type == S || new_pipe.type == F) {
            return true;
        }
    }

    return false;
}

bool can_go_down(Pipe pipes[200][200], Pipe* pipe, int x_limit, int y_limit) {
    int new_x = pipe->x;
    int new_y = pipe->y + 1;

    if (new_x >= 0 && new_x < x_limit && new_y >= 0 && new_y < y_limit) {
        Pipe new_pipe = pipes[new_y][new_x];

        if (new_pipe.letter == '.') {
            return false;
        }

        if (pipe->type == H || pipe->type == L || pipe->type == J) {
            return false;
        } else if (new_pipe.type == V || new_pipe.type == L || new_pipe.type == J) {
            return true;
        }
    }

    return false;
}

int main(void) {
    printf("Input file: %s\n", INPUT);

    FILE* f = fopen(INPUT, "r");

    if (f == NULL) {
        perror("Error opening file");
        return -1;
    }

    int lines_size = 0;
    char lines[1000][1000];
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int pipes_x_size = 0;
    int pipes_y_size = 0;
    Pipe pipes[200][200];

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        strcpy(lines[lines_size], line);
        ++lines_size;
    }

    for (int y = 0; y < lines_size; y++) {
        pipes_x_size = 0;

        for (size_t x = 0; x < strlen(lines[y]); x++) {
            Pipe pipe;
            pipe.x = x;
            pipe.y = y;
            pipe.is_start = false;
            pipe.links = malloc(2 * sizeof(Pipe));
            pipe.links_size = 0;
            pipe.letter = lines[y][x];

            if (lines[y][x] == '|') {
                pipe.type = V;
            } else if (lines[y][x] == '-') {
                pipe.type = H;
            } else if (lines[y][x] == 'L') {
                pipe.type = L;
            } else if (lines[y][x] == 'J') {
                pipe.type = J;
            } else if (lines[y][x] == '7') {
                pipe.type = S;
            } else if (lines[y][x] == 'F') {
                pipe.type = F;
            } else if (lines[y][x] == 'S') {
                pipe.is_start = true;
            }

            pipes[y][x] = pipe;
            pipes_x_size++;
        }

        pipes_y_size++;
    }

    int x_start = 0;
    int y_start = 0;

    for (int y = 0; y < pipes_y_size; y++) {
        for (int x = 0; x < pipes_x_size; x++) {
            if (pipes[y][x].is_start) {
                for (int t = 0; t < types_size; t++) {
                    pipes[y][x].type = t;
                    x_start = x;
                    y_start = y;

                    bool is_correct[4] = {false, false, false, false};

                    is_correct[0] = can_go_left(pipes, &pipes[y][x], pipes_x_size, pipes_y_size);
                    is_correct[1] = can_go_right(pipes, &pipes[y][x], pipes_x_size, pipes_y_size);
                    is_correct[2] = can_go_up(pipes, &pipes[y][x], pipes_x_size, pipes_y_size);
                    is_correct[3] = can_go_down(pipes, &pipes[y][x], pipes_x_size, pipes_y_size);

                    int count_true = 0;

                    for (int i = 0; i < 4; i++) {
                        if (is_correct[i]) {
                            count_true++;
                        }
                    }

                    if (count_true == 2) {
                        break;
                    }
                }
            }

            if (can_go_left(pipes, &pipes[y][x], pipes_x_size, pipes_y_size)) {
                pipes[y][x].links[pipes[y][x].links_size] = &pipes[y][x - 1];
                pipes[y][x].links_size++;
            }

            if (can_go_right(pipes, &pipes[y][x], pipes_x_size, pipes_y_size)) {
                pipes[y][x].links[pipes[y][x].links_size] = &pipes[y][x + 1];
                pipes[y][x].links_size++;
            }

            if (can_go_up(pipes, &pipes[y][x], pipes_x_size, pipes_y_size)) {
                pipes[y][x].links[pipes[y][x].links_size] = &pipes[y - 1][x];
                pipes[y][x].links_size++;
            }

            if (can_go_down(pipes, &pipes[y][x], pipes_x_size, pipes_y_size)) {
                pipes[y][x].links[pipes[y][x].links_size] = &pipes[y + 1][x];
                pipes[y][x].links_size++;
            }
        }
    }

    int visited_size = 0;
    char* visited[20000];

    Pipe* current_pipe = &pipes[y_start][x_start];
    Pipe* queue[200];
    queue[0] = current_pipe->links[0];
    queue[1] = current_pipe->links[1];
    visited[visited_size] = malloc(100 * sizeof(char));
    sprintf(visited[visited_size], "%d,%d", current_pipe->x, current_pipe->y);
    visited_size++;

    int queue_size = 2;

    int result = 0;

    while (queue_size > 0) {
        ++result;
        current_pipe = queue[0];
        visited[visited_size] = malloc(100 * sizeof(char));
        sprintf(visited[visited_size], "%d,%d", current_pipe->x, current_pipe->y);
        visited_size++;

        for (int i = 0; i < current_pipe->links_size; i++) {
            char visited_str[100];
            sprintf(visited_str, "%d,%d", current_pipe->links[i]->x, current_pipe->links[i]->y);

            bool should_continue = false;

            for (int v = 0; v < visited_size; v++) {
                if (strcmp(visited_str, visited[v]) == 0) {
                    should_continue = true;
                    break;
                }
            }

            if (should_continue) {
                continue;
            }

            queue[queue_size] = current_pipe->links[i];
            queue_size++;
        }

        for (int i = 0; i < queue_size - 1; i++) {
            queue[i] = queue[i + 1];
        }

        queue_size--;
    }

    result /= 2;
    printf("\nPart 1: %d\n", result);

    fclose(f);

    return 0;
}