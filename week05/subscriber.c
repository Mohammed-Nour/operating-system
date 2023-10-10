#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_SIZE 1024
#define MAX_SIZE_DIR 20

int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "usage: %s n\n",argv[0]);
        return(EXIT_FAILURE);
    }

    char dir[MAX_SIZE_DIR];
    char message[MAX_SIZE];

    snprintf(dir,MAX_SIZE_DIR, "/tmp/ex1/s%s", argv[1]);
    
    int fd = open(dir, O_RDONLY);
    if(fd == -1) {
        printf("Error %d\n", errno);
        return 1;
    }
    while (1) {
        ssize_t val = read(fd, message, MAX_SIZE);
        if (val > 0) {
            printf("Subscriber %s message is: %s", argv[1], message);
        } else if(val < 0){
            break;
        }
    }
    close(fd);
    exit(EXIT_SUCCESS);
}
