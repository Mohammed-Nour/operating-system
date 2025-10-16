#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

int agentPid  =-1;

void sigint_handler(int signum) {
    if (agentPid != -1) {
        kill(agentPid, SIGTERM);
    }
    exit(0);
}

int main()
{
    signal(SIGINT, sigint_handler);
    FILE * agent = fopen("/var/run/agent.pid","r" );
    if(agent == NULL) {
		printf("Error: No agent found.\n");
        exit(EXIT_FAILURE);
    }else {
		fscanf(agent,"%d",&agentPid);
		fclose(agent);
		printf("Agent found.\n");  
    while (1) {
        printf("Choose a command{   \"read\", \"exit\", \"stop\", \"continue\"}\n");
        char command[20];
        scanf("%s",command);

        if (strcmp(command, "read") == 0) {
            kill(agentPid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agentPid, SIGUSR2);
            exit(0);
        } else if (strcmp(command, "stop") == 0) {
            kill(agentPid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agentPid, SIGCONT);
        } else {
            printf("Invalid command. Choose from {\"read\", \"exit\", \"stop\", \"continue\"}.\n");
        }
        printf("%d",agentPid);
    }   
		}
     
    return 0; 
    }
