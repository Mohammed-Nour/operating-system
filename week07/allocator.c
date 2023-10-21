#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MEMORY_SIZE 10000000

unsigned int memory[MEMORY_SIZE];

void initilize_memory() {
    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
}

void clear(unsigned int adrs) {
    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}

void allocate_first_fit(unsigned int adrs, unsigned int size) {
    unsigned int free_count = 0;
    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            free_count++;
            if (free_count == size) {
                for (unsigned int j = i + 1 - size; j <= i; j++) {
                    memory[j] = adrs;
                }
                break;
            }
        } else {
            free_count = 0;
        }
    }
}

void allocate_best_fit(unsigned int adrs, unsigned int size) {
    unsigned int free_count = 0;
    unsigned int min_size = MEMORY_SIZE + 1;
    unsigned int start_index = 0;

    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            free_count++;
        } else {
            if (free_count >= size && free_count < min_size) {
                min_size = free_count;
                start_index = i - free_count;
            }
            free_count = 0;
        }
    }

    if (min_size != MEMORY_SIZE + 1) {
        for (unsigned int j = start_index; j < start_index + size; j++) {
            memory[j] = adrs;
        }
    }
}

void allocate_worst_fit(unsigned int adrs, unsigned int size) {
    unsigned int free_count = 0;
    unsigned int max_size = 0;
    unsigned int start_index = 0;

    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            free_count++;
        } else {
            if (free_count > max_size) {
                max_size = free_count;
                start_index = i - free_count;
            }
            free_count = 0;
        }
    }

    if (max_size >= size) {
        for (unsigned int j = start_index; j < start_index + size; j++) {
            memory[j] = adrs;
        }
    }
}

double execute_queries(const char *filename, void (*allocator)(unsigned int, unsigned int)) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    initilize_memory();
    char line[100];
    unsigned int adrs, size;
    unsigned int num_queries = 0;

    clock_t start = clock();
    while (fgets(line, sizeof(line), fp) && strcmp(line, "end\n") != 0) {
        if (sscanf(line, "allocate %u %u", &adrs, &size) == 2) {
            allocator(adrs, size);
        } else if (sscanf(line, "clear %u", &adrs) == 1) {
            clear(adrs);
        }
        num_queries++;
    }
    clock_t end = clock();

    fclose(fp);

    return (double)num_queries / ((double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    double throughput_first_fit = execute_queries("queries.txt", allocate_first_fit);
    double throughput_best_fit = execute_queries("queries.txt", allocate_best_fit);
    double throughput_worst_fit = execute_queries("queries.txt", allocate_worst_fit);

    FILE *fp = fopen("ex1.txt", "w");
    if (!fp) {
        perror("Failed to open ex1.txt for writing\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Throughput (queries/sec):\n");
    fprintf(fp, "First Fit: %f\n", throughput_first_fit);
    fprintf(fp, "Best Fit: %f\n", throughput_best_fit);
    fprintf(fp, "Worst Fit: %f\n", throughput_worst_fit);
    fclose(fp);

    return 0;
}
