#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct{
    int at, bt,idx, rt, wt, ct, tat,quantum,burst; 
} ProcessData;

int quantum ; 
int arrival_times[PS_MAX];
int process_queue[2048] ;
int queue_front = -1, queue_rear = -1;


void add_to_queue(int process_idx) {
    if (queue_front == -1) {
        queue_front = 0;
    }
    queue_rear++;
    process_queue[queue_rear] = process_idx;
}

int remove_from_queue() {
    if (queue_front == -1) {
        fprintf(stderr, "The process queue is empty.\n");
        return -1;
    }
    int process_idx = process_queue[queue_front];
    if (queue_front == queue_rear) {
        queue_front = queue_rear = -1;
    } else {
        queue_front++;
    }
    return process_idx;
}

// the idx of the running process
int running_process = -1; // -1 means no running processes
// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

void read_file(FILE* file) {
    // TODO: extract the data of processes from the {file}
    // and store them in the array {data}
    // initialize ps array to zeros (the process is terminated or not created yet)

    int idx, at, bt, quantum;
    char header[64];
    data_size = 0;
    if (fgets(header, sizeof(header), file) == NULL) {
        return;
    }
    while (fscanf(file, "%d %d %d", &idx, &at, &bt) == 3) {
        data[data_size].idx = idx;
        data[data_size].at = at;
        data[data_size].bt = bt;
        data[data_size].rt = bt;
        data[data_size].wt = 0;
        data[data_size].ct = 0;
        data[data_size].tat = 0;
        data[data_size].quantum = quantum;
        data[data_size].burst = bt;
        ps[data_size] = 0;
        data_size++;
    }
	memset(process_queue, -1, sizeof(process_queue));
    fclose(file);
}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
	    // TODO: send signal SIGCONT to the worker process if it is not in one of the states
		// (1.not created yet or 2.terminated)
    for(int i = 0 ; i < data_size ; i++) {
        if(ps[i] == process) {
            if (ps[i] != 0) {
                // Send SIGCONT signal to the process
                kill(process, SIGCONT);
                break;
            }
        }
    }


}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
	    // TODO: send signal SIGTSTP to the worker process if it is not in one of the states
		// (1.not created yet or 2.terminated)
    for(int i = 0 ; i < data_size ; i++) {
        if(ps[i] == process) {
            if (ps[i] != 0) {
                // Send SIGCONT signal to the process
                kill(process, SIGTSTP);
                break;
            }
        }
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    // TODO: send signal SIGTERM to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    for(int i = 0 ; i < data_size ; i++) {
        if(ps[i] == process) {
            if (ps[i] != 0) {
                // Send SIGCONT signal to the process
                kill(process, SIGTERM);
                break;
            }
        }
    }

}

void create_process(int new_process) {
  // TODO: stop the running process

    // TODO: fork a new process and add it to ps array

    // TODO: Now the idx of the running process is new_process 

    // TODO: The scheduler process runs the program "./worker {new_process}" 
    // using one of the exec functions like execvp
    if (running_process != -1) {
        suspend(ps[running_process]);
    }

    // Fork a new process
    pid_t proc = fork();

    if (proc == 0) {
        char idx[5];
        snprintf(idx, sizeof(idx), "%d", new_process);

        char* args[] = {"./worker", idx, NULL};
        execvp(args[0], args);

        perror("execvp");
        exit(EXIT_FAILURE);
    }else if(proc>0){
			ps[new_process] = proc;
			running_process = new_process;
		}  else {
        perror("fork");
        exit(EXIT_FAILURE);

    }
}

ProcessData find_next_process() {
   for (int i = 0; i < data_size; ++i) {
        if (arrival_times[i] <= total_time) {
            arrival_times[i] = INT_MAX;
            add_to_queue(i);
        }
    }

    if (running_process != -1 && data[running_process].burst > 0 && data[running_process].quantum > 0) {
        return data[running_process];
    }

    int next_in_line = remove_from_queue();
    if (next_in_line != -1) {
        return data[next_in_line];
    } else {
        int minAT = INT_MAX;
        int location = 0;

        for (int i = 0; i < data_size; i++) {
            if (data[i].burst > 0 && data[i].at < minAT) {
                location = i;
                minAT = data[i].at;
            }
        }

        if (data[location].at > total_time) {
            total_time++;
            return find_next_process();
        }

        return data[location];
    }
}


// reports the metrics and simulation results
void report(){
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i=0; i< data_size; i++){
        printf("process %d: \n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt/data_size;
    float avg_tat = (float)sum_tat/data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){

    for(int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;

    // report simulation results
    report();

    // terminate the scheduler :)
    exit(EXIT_SUCCESS);
}

void schedule_handler(int signum) {
     total_time++;

    if (running_process != -1) {
        data[running_process].burst--;
        data[running_process].quantum--;
        printf("Scheduler: Runtime: %d seconds\n", total_time);
        printf("Scheduler: Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
        if (data[running_process].burst == 0) {
            terminate(ps[running_process]);
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process,data[running_process].burst);
            int status;
            waitpid(ps[running_process],&status,0);
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            running_process = -1;
        }
    }

    ProcessData next_proc = find_next_process();
    check_burst();

    if (running_process != next_proc.idx) {
        if (running_process != -1) {
            suspend(ps[running_process]);
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process,data[running_process].burst);
            add_to_queue(running_process);
        }
        running_process = next_proc.idx;
        if (ps[running_process] == 0) {
            create_process(running_process);
            printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process,data[running_process].burst);
            data[running_process].rt = total_time - data[running_process].at;
            data[running_process].quantum = quantum;
        } else {
            resume(ps[running_process]);
            data[running_process].quantum = quantum;
        }
    }
}

int main(int argc, char *argv[]) {
	printf("Enter the quantum time: ");
    scanf("%d", &quantum);
    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }
    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while(1) ;
}
