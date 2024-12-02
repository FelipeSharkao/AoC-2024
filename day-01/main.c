#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE_PATH "example.txt"
#define INPUT_PATH   "input.txt"

void sort(int* arr, size_t len) {
    for (size_t i = 1; i < len; i++) {
        for (size_t j = i; j > 0 && arr[j-1] > arr[j]; j--) {
            int tmp  = arr[j-1];
            arr[j-1] = arr[j];
            arr[j]   = tmp;
        }
    }
}

typedef struct {
    int* a;
    int* b;
    size_t len;
} Input;

#define Vec(T) struct { \
    T* arr;             \
    size_t len;         \
    size_t cap;         \
}

#define VEC_EMPTY {NULL, 0, 0}

#define Vec_push(vec, x) ({                                      \
    if (vec.cap == vec.len) {                                    \
        vec.cap = vec.cap > 0 ? vec.cap*2 : 10;                  \
        vec.arr = realloc(vec.arr, vec.cap * sizeof(typeof(x))); \
    }                                                            \
    vec.arr[vec.len] = x;                                        \
    vec.len += 1;                                                \
    vec.len;                                                     \
});

Input load_input(const char* path) {
    Vec(int) a = VEC_EMPTY;
    Vec(int) b = VEC_EMPTY;

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

        Vec_push(a, a_val);
        Vec_push(b, b_val);
    }

    if (line != NULL) {
        free(line);
    }

    sort(a.arr, a.len);
    sort(b.arr, b.len);

    Input r = {a.arr, b.arr, a.len};
    return r;
}

void drop_input(Input input) {
    free(input.a);
    free(input.b);
}

int solve(Input input) {
    int dis = 0;

    for (size_t i = 0; i < input.len; i++) {
        dis += abs(input.a[i] - input.b[i]);
    }

    return dis;
}

typedef struct {
    int value;
    size_t count;
} Stat;

typedef Vec(Stat) Stats;

size_t stat_index(Stats stats, int x) {
    size_t l = 0;
    size_t h = stats.len - 1;
    while (l < h) {
        size_t m = (l + h) / 2;
        if (stats.arr[m].value < x) {
            l = m + 1;
        } else if (stats.arr[m].value > x) {
            h = m - 1;
        } else {
            return m;
        }
    }
    return l;
}

Stat* stat_for(Stats stats, int x) {
    size_t idx = stat_index(stats, x);
    if (stats.arr[idx].value != x) {
        return NULL;
    }
    return &stats.arr[idx];
}

Stats calc_stats(Input input) {
    Stats stats = VEC_EMPTY;
    int last = -1;

    for (size_t i = 0; i < input.len; i++) {
        if (input.b[i] > last) {
            Stat item = {input.b[i], 1};
            Vec_push(stats, item);
            last = input.b[i];
        } else {
            size_t idx = stat_index(stats, input.b[i]);
            stats.arr[idx].count += 1;
        }
    }

    return stats;
}

int solve2(Input input) {
    int tot = 0;

    Stats stats = calc_stats(input);


    for (size_t i = 0; i < input.len; i++) {
        int count = 0;
        Stat* stat = stat_for(stats, input.a[i]);
        if (stat != NULL) {
            count = stat->count;
        }
        tot += input.a[i] * count;
    }

    return tot;
}

int main() {
    Input example = load_input(EXAMPLE_PATH);
    Input input = load_input(INPUT_PATH);

    for (int i = 0; i < 2; i++) {
        int example_res = solve(example);
        printf("Example: %d\n", example_res);

        int example_res2 = solve2(example);
        printf("Example (2nd puzzle): %d\n", example_res2);
    }

    int res = solve(input);
    printf("Result: %d\n", res);

    int res2 = solve2(input);
    printf("Result (2nd puzzle): %d\n", res2);

    drop_input(example);
    drop_input(input);
}
