#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1024

int main() {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Subscriber process
        char subscribeMessage[MAX_SIZE];
        close(pipe_fd[1]); // Close the write end

        ssize_t bytesRead = read(pipe_fd[0], subscribeMessage, MAX_SIZE);
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        subscribeMessage[bytesRead] = '\0'; // Null-terminate the message
        printf("Received: %s\n", subscribeMessage);
        close(pipe_fd[0]);
        return EXIT_SUCCESS;
    } else {
        // Publisher process
        char publishMessage[MAX_SIZE];
        close(pipe_fd[0]); // Close the read end

        printf("Enter a message to publish: ");
        if (fgets(publishMessage, MAX_SIZE, stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove the newline character, if present
        size_t len = strlen(publishMessage);
        if (len > 0 && publishMessage[len - 1] == '\n') {
            publishMessage[len - 1] = '\0';
        }

        ssize_t bytesWritten = write(pipe_fd[1], publishMessage, strlen(publishMessage) + 1);
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd[1]);
        return EXIT_SUCCESS;
    }

    return 0;
}

