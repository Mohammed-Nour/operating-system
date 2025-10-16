#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>

#define FILE_SIZE (500 * 1024 * 1024) 
#define MAX_LINE_LENGTH 1024

int main() {
    long pagesize = sysconf(_SC_PAGESIZE);
    long chunk_size = pagesize * 1024;
    
    FILE *out = fopen("text.txt", "w");
    FILE *random = fopen("/dev/random", "r");
    if (!out || !random) {
        perror("Error opening files\n");
        exit(EXIT_FAILURE);
    }

    char c;
    int lineLength = 0;
    int bytesSize = 0;
    while (bytesSize < FILE_SIZE) {
        c = fgetc(random);
        if (isprint(c)) {
            fputc(c, out);
            lineLength++;
            bytesSize++;
            if (lineLength == MAX_LINE_LENGTH) {
                fputc('\n', out);
                bytesSize++;
                lineLength = 0;
            }
        }
    }
    fclose(out);
    fclose(random);

    // Map text.txt in chunks and process
    int fd = open("text.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening text.txt\n");
        exit(EXIT_FAILURE);
    }

    int capitalTotla = 0;
    for (long j = 0; j < FILE_SIZE; j += chunk_size) {
        char *data = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, j);
        if (data == MAP_FAILED) {
            perror("Error mmapping the file\n");
            exit(EXIT_FAILURE);
        }
        for (long i = 0; i < chunk_size && j + i < FILE_SIZE; i++) {
            if (isupper(data[i])) {
                capitalTotla++;
                data[i] = tolower(data[i]);
            }
        }
        munmap(data, chunk_size);
    }

    printf("Total capital letters: %d\n", capitalTotla);
    close(fd);
    return 0;
}

