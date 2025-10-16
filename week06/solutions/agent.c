#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
void sigusr1Handler(int signum) {
   FILE *text = fopen("text.txt", "r");
    if (text == NULL) {
        printf("Error with file text.txt.\n");
        exit(1);
    }
 char line[1024];
        while (fgets(line, sizeof(line), text)) {
            printf("%s", line);
        }

    fclose(text);
}

void sigusr2Handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}

int main()
{
    


    FILE * agent = fopen("/var/run/agent.pid","w" );
    if(agent == NULL) {
        perror("Error with file agent.pid ");
        return 1;
    }else  {
	fprintf(agent, "%d", getpid());
     fclose(agent);

	
		}
        signal(SIGUSR1, sigusr1Handler);
    signal(SIGUSR2, sigusr2Handler);
       FILE *text = fopen("text.txt", "r");
    if (text == NULL) {
        printf("Error with file text.txt.\n");
        exit(1);
    }
        char line[1024];
        while (fgets(line, sizeof(line), text)) {
            printf("%s", line);
        }

    fclose(text);  
    while (1) {
        sleep(1); 
    }
    return 0;
}
