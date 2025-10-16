#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_SIZE 1024

int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "usage: %s n\n",argv[0]);
        return(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    char message[MAX_SIZE];
    char dir[20];

    for (int i = 0; i < n; i++) {
        sprintf(dir, "/tmp/ex1/s%d", i);
        mkfifo(dir, 0666);
    }
    puts("start entering messages:");
    while (fgets(message, MAX_SIZE, stdin)) {
        for (int i = 0; i <n; i++) {
            if (fork() == 0) {
                sprintf(dir, "/tmp/ex1/s%d", i);
                int fd = open(dir, O_WRONLY);
                write(fd, message, MAX_SIZE);
                close(fd);
                exit(EXIT_SUCCESS);
            }
        }
    }
    exit(EXIT_SUCCESS);
}

