#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#define min(a,b) (((a) < (b)) ? (a) : (b))

bool is_prime ( int n ) 
{
    if ( n <= 1) return false ;
    int i = 2;
    for (; i * i <= n ; i ++) {
        if ( n % i == 0)
            return false ;
    }
    return true ;
}

// Primes counter in [a , b )
int primes_count ( int a , int b )
{
    int ret = 0;
    for ( int i = a ; i < b ; i ++)
        if ( is_prime ( i ) != 0)
            ret ++;
    return ret ;
}

// a r g u m e n t to the s t a r t _ r o u t i n e
// of the thread
typedef struct prime_request {
    int a , b ;
} prime_request;

// s t a r t _ r o u t i n e of the thread
void * prime_counter ( void * arg ) {
    prime_request * req = (prime_request*) arg;
    int * count = malloc(sizeof(int));
    *count = primes_count(req->a, req->b);
    return (( void *) count ) ;
}

void free_memory(void **results, pthread_t *threads, prime_request *requests, int m) {

    for (int i = 0; i < m; i++)
        free(results[i]);
    free(results);
    free(requests);
    free(threads);
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("usage: %s n m\n",argv[0]);
        return(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int seg_size = n / m;

    void **results = malloc(m * sizeof(void *));
    pthread_t *threads = malloc(m * sizeof(pthread_t));
    prime_request *requests = malloc(m * sizeof(prime_request));
    for (int i = 0; i < m; i++) {
        requests[i].a = i*seg_size;
        requests[i].b = min((i+1)*seg_size, n-1);
        pthread_create( &threads[i], NULL, prime_counter, (void *) (&requests[i]));
    }

    for (int i = 0; i < m; i++) {
        int exit_code = pthread_join(threads[i], &results[i]);
        if(exit_code != 0 )
            printf("Thread %lu existed with code: %d",threads[i], exit_code);
    }
    int total_result = 0;
    for (int i = 0; i < m; i++) {
        total_result += *(int *)results[i];
    }
    printf("%d\n", total_result);
    free_memory(results, threads, requests, m);
    exit(EXIT_SUCCESS);
}
