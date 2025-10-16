#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

void executeCommand(char *command) {
    char *args[MAX_ARGS];
    char *token;
    int argCount = 0;

    token = strtok(command, " \t\n");

    while (token != NULL && argCount < MAX_ARGS - 1) {
        args[argCount++] = token;
        token = strtok(NULL, " \t\n");
    }

    if (argCount == 0) {
        return;
    }

    args[argCount] = NULL;

    pid_t pid = fork();

    if (pid < 0) {
        exit(1);
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            exit(1);
        }
    } else {
        wait(NULL); 
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        input[strlen(input) - 1] = '\0';

        executeCommand(input);
    }

    return 0;
}

