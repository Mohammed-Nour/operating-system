#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    unsigned int age;  // Aging counter

};

#define PAGE_TABLE_PATH "/tmp/ex2/pagetable"


int totalPages;
int totalFrames;
struct PTE *pageTable;
char **diskArray;
char **ramArray;
volatile sig_atomic_t pageRequested = 0;
int diskAccesses = 0;
int typeVictim;

void handle_signal(int signal) {
    if (signal == SIGUSR1) {
        pageRequested = 1;
    }
}

int find_free_frame() {
    for (int i = 0; i < totalFrames; ++i) {
        if (ramArray[i][0] == '\0') { // Assuming empty frame is represented by an empty string
            return i;
        }
    }
    return -1;
}

void print_page_table() {
    
    for (int i = 0; i < totalPages; ++i) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced);
    }
    printf("-------------------------\n");
}

void print_ram_array() {
    printf("RAM array\n");
    for (int i = 0; i < totalFrames; ++i) {
        printf("Frame %d ---> %s\n", i, ramArray[i]);
    }
}

void initialize() {
    // Allocate and initialize RAM
    ramArray = malloc(sizeof(char*) * totalFrames);

     // Simulating disk array
    diskArray = malloc(sizeof(char*) * totalPages);

    int fd = open(PAGE_TABLE_PATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE) ;
    }

    if (ftruncate(fd, sizeof(struct PTE) * totalPages) == -1) {
        perror("Error truncating the page table file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Allocate and initialize page table
    pageTable = mmap(NULL, sizeof(struct PTE) * totalPages, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (pageTable == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < totalPages; ++i) {
        pageTable[i] = (struct PTE){.valid = false, .frame = -1, .dirty = false, .referenced = 0};
    }

    for (int i = 0; i < totalPages; ++i) {
        diskArray[i] = malloc(sizeof(char) * 8); // Dummy string to simulate disk content
        sprintf(diskArray[i], "Data%d", i); // Just to simulate
    }
    printf("-------------------------\n");
    printf("Initialized page table:\n");
    print_page_table();
    printf("Initialized RAM:\n");
    for (int i = 0; i < totalFrames; ++i) {
        ramArray[i] = malloc(sizeof(char) * 8);
        strcpy(ramArray[i], "");
    }
    print_ram_array();
    printf("-------------------------\n");
    printf("Initialized disk:\n");
    printf("Disk array:\n");
    for (int i = 0; i < totalPages; ++i) {
        printf("Page %d ---> %s\n", i, diskArray[i]);
        
    }
}
// Random page replacement
int random_replacement(struct PTE* page_table, int num_pages) {
  int victim;
    do {
        victim = rand() % num_pages;
    } while (!page_table[victim].valid);
    return victim;
}

// NFU page replacement
int nfu(struct PTE* page_table, int num_pages) {
    int min_referenced = __INT_MAX__;
    int victim = -1;
    for (int i = 0; i < num_pages; ++i) {
        if (page_table[i].valid && page_table[i].referenced < min_referenced) {
            min_referenced = page_table[i].referenced;
            victim = i;
        }
    }
    return victim; // Return the index of the page with the least referenced count
}

// Aging page replacement
int aging(struct PTE* page_table, int num_pages) {
    int victim = -1;
    unsigned int min_age = ~0U; // Initialize with the maximum unsigned int
    for (int i = 0; i < num_pages; ++i) {
        if (page_table[i].valid) {
            if (page_table[i].referenced) {
                page_table[i].age >>= 1; // Right shift age
                page_table[i].age |= 1U << (sizeof(page_table[i].age) * 8 - 1); // Set the leftmost bit if referenced
            } else {
                page_table[i].age >>= 1; // Continue aging if not referenced
            }

            // Reset the referenced bit for the next cycle
            page_table[i].referenced = 0;

            // Check if this page has the smallest age
            if (page_table[i].age < min_age) {
                min_age = page_table[i].age;
                victim = i;
            }
        }
    }
    // Reset the age of the victim page
    if (victim != -1) {
        page_table[victim].age = 0;
    }
    return victim; // Return the index of the oldest page
}



void terminate() {
    printf("-------------------------\n");
    printf("%d disk accesses in total\n", diskAccesses);
    printf("Pager is terminated\n");

    // Cleanup
    for (int i = 0; i < totalPages; ++i) {
        free(diskArray[i]);
    }
    free(diskArray);

    for (int i = 0; i < totalFrames; ++i) {
        free(ramArray[i]);
    }
    free(ramArray);
    munmap(pageTable, totalPages * sizeof(struct PTE));
    exit(0);
}

void page_fault_handler() {
    bool found = false;
    int pid;
    for (int i = 0; i < totalPages; ++i) {
        if (pageTable[i].referenced) {
            found = true;
            pid =pageTable[i].referenced;
            diskAccesses++;
            printf("-------------------------\n");
            printf("A disk access request from MMU Process (pid=%d)\n", pid);
            printf("Page %d is referenced\n", i);
            int frameIdx = find_free_frame();
            if (frameIdx != -1) {
                printf("We can allocate it to free frame %d\n", frameIdx);
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, frameIdx);
                pageTable[i].valid = true;
                pageTable[i].frame = frameIdx;
                pageTable[i].referenced = 0;
                pageTable[i].dirty = false;
                strcpy(ramArray[frameIdx],diskArray[i]);
            } else {
                printf("We do not have free frames in RAM\n");
                // Randomly select a victim page
               int  victim = -1;
               if(typeVictim ==0) {
                victim = random_replacement(pageTable,totalPages);
               }else if(typeVictim ==1) {
                victim =nfu(pageTable,totalPages);
               }else if(typeVictim == 2) {
                victim =aging(pageTable,totalPages);
               }
                if (victim == -1) {
                    fprintf(stderr, "Error: No available victim page found.\n");
                    terminate(); // Or handle error accordingly
                    exit(EXIT_FAILURE);

                }
               int victimFrame = pageTable[victim].frame;
                printf("Chose a random victim page %d\n", victim);
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, victimFrame);
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, victimFrame);
                if (pageTable[victim].dirty) {
                    strcpy(diskArray[pageTable[victim].frame], ramArray[victimFrame]);
                    diskAccesses++;
                }
                strcpy(ramArray[victimFrame], diskArray[i]);
                 // Load the page into RAM
                pageTable[i].valid = true;
                pageTable[i].frame = victimFrame;
                pageTable[i].dirty = false;
                pageTable[i].referenced = 0;
                // Update the page table for the victim
                pageTable[victim].valid = false;
                pageTable[victim].frame = -1;
                pageTable[victim].dirty = false;
                pageTable[victim].referenced = 0;

            }

            

            print_ram_array();
            printf("disk accesses is %d so far\n", diskAccesses);
            printf("Resume MMU process\n");
            printf("-------------------------\n");
            // Send a SIGCONT to the MMU process (just simulating here)
             kill(pid, SIGCONT);
             break;
        }
    }

    if (!found) {
        terminate();
        return;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <algorithm> <num_frames> <num_pages>\n", argv[0]);
        return 1;
    }

// Parse the page replacement algorithm
    // int (*page_replacement_func)(struct PTE*);
    if (strcmp(argv[1], "random") == 0) {
        // page_replacement_func = random_replacement;
        printf("Selected algorithm: Random Page Replacement\n");
        typeVictim = 0;
    } else if (strcmp(argv[1], "nfu") == 0) {
        // page_replacement_func = nfu;
        printf("Selected algorithm: Not Frequently Used (NFU) Page Replacement\n");
        typeVictim = 1;
    } else if (strcmp(argv[1], "aging") == 0) {
        // page_replacement_func = aging;
        printf("Selected algorithm: Aging Page Replacement\n");
        typeVictim = 2;
    } else {
        fprintf(stderr, "Invalid page replacement algorithm: %s\n", argv[1]);
        typeVictim = -1;
        exit(EXIT_FAILURE) ;
    }


    totalPages = atoi(argv[3]);
    totalFrames = atoi(argv[2]);

    if (totalPages <= 0 || totalFrames <= 0) {
        fprintf(stderr, "Error: Number of pages and frames must be positive\n");
        exit(EXIT_FAILURE);
    }

    if (totalFrames > totalPages) {
        fprintf(stderr, "Error: Cannot have more frames than pages\n");
        exit(EXIT_FAILURE);
    }

    // Register signal handler
    struct sigaction sa;
    sa.sa_handler = &handle_signal;
    sa.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error: sigaction");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL)); // Seed for random number generation

    initialize();
    // Wait for signals
    while (1) {
        pause(); // Wait for any signal

        if (pageRequested) {
            pageRequested = 0; // Reset the signal flag
            page_fault_handler();
        }
    }

    return 0;
}
