#ifndef _PROCESS_HEADER

#define _PROCESS_HEADER 
enum Process_State {READY, RUNNING, FINISHED};

typedef struct {
    int arival_time;
    int time_remaining;
    char *name;
    enum Process_State state;
    int memory_required;
    int memory_address;
} Process;
// frees the memory from a process
void free_process(Process process);

// updates proecss after having run it for a quantum, frees memory and process if completed
Process *update_running_process(Process *running_process, int time);

#endif