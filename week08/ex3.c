#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    struct rusage usage;
    for (int i = 0; i < 10; ++i) {
        // Allocate 10 MB of memory
        char *memory = malloc(10 * 1024 * 1024 * sizeof(char));
        if (memory == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Fill the allocated memory with zeros
        memset(memory, 0, 10 * 1024 * 1024);

        // Get the current memory usage
        if (getrusage(RUSAGE_SELF, &usage) != 0) {
            perror("getrusage");
            free(memory);
            exit(EXIT_FAILURE);
        }

        // Print memory usage
        printf("Memory usage: %ld kilobytes\n", usage.ru_maxrss);

        // Sleep for 1 second
        sleep(1);
    }
    return 0;
}
