#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#define PAGE_TABLE_PATH "/tmp/ex2/pagetable"

// Page Table Entry structure
typedef struct PTE {
    bool valid;      // The page is in the physical memory (RAM)
    int frame;       // The frame number of the page in the RAM
    bool dirty;      // The page should be written to disk
    int referenced;  // The page is referenced/requested
    unsigned int age;  // Aging counter

} PTE;

// Global variables for signal handling
volatile sig_atomic_t page_loaded = 0;

// Signal handler for SIGCONT
void handle_sigcont(int sig) {
    page_loaded = 1;
}

// Function to calculate and print the hit ratio
void print_hit_ratio(unsigned long long total_requests, unsigned long long hit_count) {
    if (total_requests == 0) {
        printf("No requests processed.\n");
        return;
    }

    double hit_ratio = (double)hit_count / total_requests;
    printf("Hit Ratio: %.2f%% (%llu hits out of %llu total requests)\n", hit_ratio * 100, hit_count, total_requests);
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <num_pages> <reference_string> <pager_pid>\n", argv[0]);
        exit(EXIT_FAILURE) ;
    }

    int num_pages = atoi(argv[1]);
    int pager_pid = atoi(argv[argc - 1]);
    // Variables for benchmarking
    unsigned long long total_requests = 0;
    unsigned long long hit_count = 0;

    // Register the signal handler for SIGCONT
    struct sigaction sa;
    sa.sa_handler = handle_sigcont;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCONT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE) ;
    }

    // Open the page table file
    int fd = open(PAGE_TABLE_PATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE) ;
    }
    ftruncate(fd, num_pages * sizeof(struct PTE));
    // Map the page table file into memory
    PTE *page_table = mmap(NULL, num_pages * sizeof(PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE) ;
    }
    printf("-------------------------\n");
    printf("Initialized page table:\n");
    for (int i = 0; i < num_pages; i++) {
        if(i ==0 )printf("Page table\n");
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid,page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    // Process the reference string
    for (int ref_index = 2; ref_index < argc - 1; ++ref_index) {
        total_requests++; 
        printf("-------------------------\n");
        char mode = argv[ref_index][0];// W for write, R for read
        int page = atoi(&argv[ref_index][1]); // Convert char to int
        printf("%c%d\n",mode,page);
        if(mode =='W')  {
            printf("Write Request for page %d\n", page);
        }else {
            printf("Read Request for page %d\n", page);
        }

        // Check if the page is in RAM
        
        if (!page_table[page].valid) {
            printf("It is not a valid page --> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            printf("MMU resumed by SIGCONT signal from pager\n");
            page_table[page].referenced = getpid(); // Set the referenced field to the PID of the MMU
            kill(pager_pid, SIGUSR1); // Signal the pager process
            while (!page_loaded) {
                pause(); // Sleep indefinitely until SIGCONT is received
            }
            page_loaded = 0; // Reset for the next signal
        }else {
            hit_count++;
             printf("It is a valid page\n");
        }
          

        if (mode == 'W') { 
            printf("It is a write request then set the dirty field\n");
            page_table[page].dirty = true;// Set the dirty field for write access
        }
        for (int i = 0; i < num_pages; i++) {
        if(i ==0 )printf("Page table\n");
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid,page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }
        printf("\n");
     
    }
    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    print_hit_ratio(total_requests, hit_count);

    printf("MMU terminates.\n");
    // Done with all memory accesses
    munmap(page_table, num_pages * sizeof(PTE));
    close(fd);

    // Signal the pager one last time
    kill(pager_pid, SIGUSR1);

    exit(EXIT_SUCCESS);
}
