// ex3.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    struct rusage usage;

    for(int i = 0; i < 10; i++) {
        // Allocate 10 MB of memory
        char* memory = malloc(10 * 1024 * 1024);
        if(!memory) {
            perror("Failed to allocate memory");
            return 1;
        }

        // Fill it with zeros
        memset(memory, 0, 10 * 1024 * 1024);

        // Print memory usage with getrusage() function
        if(getrusage(RUSAGE_SELF, &usage) == 0) {
            printf("Memory usage: %ld KB\n", usage.ru_maxrss);
        } else {
            perror("getrusage failed");
            free(memory);
            return 1;
        }

        // Sleep for 1 second
        sleep(1);
    }

    return 0;
}

