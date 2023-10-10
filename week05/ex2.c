#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_SIZE 256

struct Thread{
	pthread_t id;
	int i;
	char message[MAX_SIZE];
};


void *threadFunction(void *arg) {
    struct Thread *thread= (struct Thread *)arg;
    
    printf("Thread %d is created\n", thread->i);
    printf("Thread id %ld, prints message: %s\n", thread->id, thread->message);
    printf("Thread %d exit\n",thread->i);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
 if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

     int n = atoi(argv[1]);
    struct Thread threads[n];
    for(int i=0;i<n;i++){
		threads[i].i = i;
        threads[i].id =pthread_self();
		sprintf(threads[i].message, "Hello from thread %d", i);
		pthread_create(&threads[i].id, NULL, threadFunction, (void *)&threads[i]);
    // Wait for the thread to finish
		pthread_join(threads[i].id,NULL);
	}


    return 0;
}
