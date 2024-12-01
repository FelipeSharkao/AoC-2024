#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EXAMPLE_PATH "example.txt"
#define INPUT_PATH   "input.txt"

void sort(int* arr, size_t len) {
    for (size_t i = 1; i < len; i++) {
        for (size_t j = i; j > 0 && arr[j-1] < arr[j]; j--) {
            int tmp  = arr[j-1];
            arr[j-1] = arr[j];
            arr[j]   = tmp;
        }
    }
}

void load_input(const char* path, int** a_out, int** b_out, size_t* len_out) {
    size_t cap = 0;
    size_t len = 0;
    int* a = NULL;
    int* b = NULL;

    FILE* file = fopen(path, "r");

    char* line = NULL;
    size_t line_len = 0;

    while (true) {
        ssize_t read = getline(&line, &line_len, file);
        if (read <= 0 || line_len == 0) {
            break;
        }

        int a_val;
        int b_val;
        int matched = sscanf(line, "%d   %d\n", &a_val, &b_val);
        if (matched < 2) {
            break;
        }

        if (len == cap) {
            cap = cap > 0 ? cap * 2 : 10;
            a = (int*)realloc(a, cap * sizeof(int));
            b = (int*)realloc(b, cap * sizeof(int));
        }

        a[len] = a_val;
        b[len] = b_val;
        len += 1;
    }

    *a_out   = a;
    *b_out   = b;
    *len_out = len;

    if (line != NULL) {
        free(line);
    }
}

int solve(const char* path) {
    int* a = NULL;
    int* b = NULL;
    size_t len = 0;

    load_input(path, &a, &b, &len);

    sort(a, len);
    sort(b, len);

    int dis = 0;

    for (size_t i = 0; i < len; i++) {
        dis += abs(a[i] - b[i]);
    }

    free(a);
    free(b);

    return dis;
}

int main() {
    int example_res = solve(EXAMPLE_PATH);
    printf("Example: %d\n", example_res);

    int example_res2 = solve(EXAMPLE_PATH);
    printf("Example: %d\n", example_res2);

    int res = solve(INPUT_PATH);
    printf("Result: %d\n", res);
}
