#ifndef _PROCESS_HEADER

#define _PROCESS_HEADER 
#include <unistd.h>
#include "queue.h"
typedef struct {
    int arrival_time;
    int service_time;
    int time_remaining;
    char *name;
    int memory_required;
    int memory_address;
    int stdin_fd;
    int stdout_fd;
    int time_finished;
    pid_t pid;
} Process;

#include "linked-list.h"

// frees the memory from a process
void free_process(Process *process);

// creates a process with the given values
Process *create_process(int arrival_time, int time_remaining, char *name, int memory_required);

// checks if process is finished running
Process *check_running_process(Linked_List *memory_list, Queue finished_queue, Process *running_process, int time, int num_processes_left);

// prints the values of a process mainly for debugging and testing
void print_process(Process *process);

// creates instance of process executable with I/O connected to file descriptors
void create_forked_process(Process *process, int time, int verbose);

// suspends process executable and sends it the time it is being suspended
void suspend_process(Process *process, int time);

// resumes process executable and sends it the time
void resume_process(Process *process, int time);

// terminates process executable gets and prints associated hash
void terminate_process(Process *process, int time);

#endif