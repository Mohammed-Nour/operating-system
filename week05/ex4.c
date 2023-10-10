#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// primality test ( from ex3 )
bool is_prime ( int n ) ;
// The mutex
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER ;
// Do not modify these v a r i a b l e s directly , use the f u n c t i o n s on the right side .
int k = 0;
int c = 0;
// input from command line
int n = 0;

// get next prime candidate
int get_number_to_check() {
    int ret = k;
    if ( k != n )
        k ++;
    return ret;
}
// increase prime counter
void increment_primes() {
    c++;
}
// start_routine
void * check_primes ( void * arg ) {
    while (1) {
        pthread_mutex_lock(&global_lock);
        int num = get_number_to_check();
        if(num == n) {
            pthread_mutex_unlock(&global_lock);
            return NULL;
        }
        pthread_mutex_unlock(&global_lock);
        if(is_prime(num)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "usage: %s n m\n",argv[0]);
        return(EXIT_FAILURE);
    }
    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    for (int i = 0; i < m; i++) {
        pthread_create( &threads[i], NULL, check_primes, NULL);
    }
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("%d\n", c);
    free(threads);
    exit(EXIT_SUCCESS);
}

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
