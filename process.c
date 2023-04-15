#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// frees the memory from a process
void free_process(Process *process) {
    free(process->name);
    free(process);
}

// creates a process with the given values
Process *create_process(int arival_time, int time_remaining, char *name, int memory_required) {
    Process *new_process = malloc(sizeof(Process));
    assert(new_process);
    new_process->arival_time = arival_time;
    new_process->time_remaining = time_remaining;
    new_process->name = strdup(name);
    new_process->memory_required = memory_required;
    new_process->memory_address = -1;
    return new_process;
}

// updates proecss after having run it for a quantum, frees memory and process if completed
Process *update_running_process(Process *running_process, int time) {
    return running_process;
}

// prints the values of a process mainly for debugging and testing
void print_process(Process *process) {
    printf("Name: %s, Time Remaining: %d\n", process->name, process->time_remaining);
}