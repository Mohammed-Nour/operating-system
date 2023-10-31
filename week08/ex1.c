#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <ctype.h>


int main() {
    // Write pid to /tmp/ex1.pid
    FILE* pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file == NULL) {
        perror("Failed to open /tmp/ex1.pid");
        exit(1);
    }
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);

    // Generate a random password
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open /dev/urandom");
        exit(1);
    }
    
    char prefix[] = "pass:";
    char password[14] = "pass:"; // 5 characters for "pass:" and 8 for the random characters
    for (int i = 5; i < 13; i++) {
        char c;
        do {
            read(fd, &c, 1);
        } while (!isprint(c));
        password[i] = c;
    }
    close(fd);
    password[13]='\0';

    // Store the password in memory using mmap
    char* mapped;
    mapped = mmap(NULL, sizeof(password), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    strcpy(mapped, password);

    printf("%s\n",password);


    // Infinite loop
    while (1) {
        sleep(1);
    }

    return 0;
}

