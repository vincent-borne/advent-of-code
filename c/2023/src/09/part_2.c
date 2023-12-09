#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "./src/09/input.txt"

typedef struct {
    int original_size;
    int original[1000];
    int generated_line_size;
    int generated_size[1000];
    int generated[1000][1000];
} Report;

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

void print_report_array(Report* arr, int reports_size) {
    for (int i = 0; i < reports_size; i++) {
        for (int j = 0; j < arr[i].original_size; j++) {
            printf("%d ", arr[i].original[j]);
        }

        printf("\n");
        for (int k = 0; k < arr[i].generated_line_size; k++) {
            for (int j = 0; j < arr[i].generated_size[k]; j++) {
                printf("%d ", arr[i].generated[k][j]);
            }

            printf("\n");
        }

        printf("\n");
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

    int size = 1000;
    int reports_size = 0;
    Report* reports = malloc(size * sizeof(Report));

    while ((read = getline(&line, &len, f)) != -1) {
        strtok(line, "\n");

        int split_path_size;
        char* split_path[size];
        split(line, " ", split_path, &split_path_size);

        Report report;

        for (int i = 0; i < split_path_size; i++) {
            int num = atoi(split_path[i]);

            report.original[i] = num;
            report.original_size = i;
        }

        report.original_size++;
        report.generated_line_size = 0;
        for (int i = 0; i < size; i++) {
            report.generated_size[i] = 0;
        }

        reports[reports_size] = report;
        reports_size++;
    }

    int result = 0;

    for (int i = 0; i < reports_size; i++) {
        Report* report = &reports[i];

        if (report->generated_line_size == 0) {
            for (int j = 0; j < report->original_size - 1; j++) {
                int dif = report->original[j + 1] - report->original[j];
                report->generated[report->generated_line_size][report->generated_size[report->generated_line_size]] = dif;
                report->generated_size[report->generated_line_size]++;
            }

            report->generated_line_size++;
        }

        while (true) {
            int* last_line = report->generated[report->generated_line_size - 1];
            int last_line_size = report->generated_size[report->generated_line_size - 1];

            bool is_done = true;

            for (int j = 0; j < last_line_size; j++) {
                if (last_line[j] != 0) {
                    is_done = false;
                    break;
                }
            }

            if (is_done) {
                break;
            }

            for (int k = 0; k < last_line_size - 1; k++) {
                int dif = last_line[k + 1] - last_line[k];
                report->generated[report->generated_line_size][report->generated_size[report->generated_line_size]] = dif;
                report->generated_size[report->generated_line_size]++;
            }

            report->generated_line_size++;
        }

        report->generated[report->generated_line_size - 1][report->generated_size[report->generated_line_size - 1]] = 0;
        report->generated_size[report->generated_line_size - 1]++;

        for (int j = report->generated_line_size - 1; j > 0; j--) {
            int* p_line = report->generated[j - 1];
            int* line = report->generated[j];

            p_line[0] = p_line[0] - line[0];
        }

        report->original[0] = report->original[0] - report->generated[0][0];
        result += report->original[0];
        report->original_size++;
    }

    print_report_array(reports, reports_size);

    printf("\nPart 2: %d\n", result);

    fclose(f);

    return 0;
}