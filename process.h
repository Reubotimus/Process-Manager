#ifndef _PROCESS_HEADER

#define _PROCESS_HEADER 

typedef struct {
    int arival_time;
    int time_remaining;
    char *name;
    int memory_required;
    int memory_address;
} Process;

#include "linked-list.h"

// frees the memory from a process
void free_process(Process *process);

// creates a process with the given values
Process *create_process(int arival_time, int time_remaining, char *name, int memory_required);

// checks if process is finished running
Process *check_running_process(Linked_List *memory_list, Process *running_process, int time, int num_processes_left);

// prints the values of a process mainly for debugging and testing
void print_process(Process *process);

#endif