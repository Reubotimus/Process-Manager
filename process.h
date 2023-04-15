#ifndef _PROCESS_HEADER

#define _PROCESS_HEADER 

typedef struct {
    int arival_time;
    int time_remaining;
    char *name;
    int memory_required;
    int memory_address;
} Process;

// frees the memory from a process
void free_process(Process *process);

// creates a process with the given values
Process *create_process(int arival_time, int time_remaining, char *name, int memory_required);

// updates proecss after having run it for a quantum, frees memory and process if completed
Process *update_running_process(Process *running_process, int time);

// prints the values of a process mainly for debugging and testing
void print_process(Process *process);

#endif