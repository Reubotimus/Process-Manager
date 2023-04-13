#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libc.h>
#include "process-scheduler.h"
#include "memory-manager.h"
#include "queue.h"
#include "input.h"


// performs one run cycle in the process manager, gets new processes, allocates memory and schedules a process for the next quantum
int run_cycle(Parameters *parameters, int time, Process *running_process, Queue input_queue, Queue ready_queue, Linked_List *memory_list);


int main(int argc, char *argv[]) {
    Parameters *parameters = get_parameters(argc, argv);
    
    // initialises queues and running process, the type of queue is according to efficiency
    Process *running_process = NULL;
    Queue input = create_queue(STANDARD); 
    Queue ready;
    Linked_List *memory_list = create_list();
    if (parameters->scheduler_algorithm == SJF) ready = create_queue(PRIORITY);
    else ready = create_queue(STANDARD);
    int time = 0;

    while (run_cycle(parameters, time, running_process, input, ready, memory_list)) time += parameters->quantum;

    fclose(parameters->file_pointer);
    free_queue(ready);
    free_queue(input);
    free(parameters);
}

int run_cycle(Parameters *parameters, int time, Process *running_process, Queue input_queue, Queue ready_queue, Linked_List *memory_list) {
    running_process = update_running_process(running_process, time);
    parse_new_processes(input_queue, parameters->file_pointer, time);
    if (parameters->memory_strategy == BEST_FIT) allocate_memory(memory_list, ready_queue, time);
    running_process = select_new_process(ready_queue, running_process, time);
    if (running_process == NULL && feof(parameters->file_pointer) && is_empty_queue(ready_queue) && is_empty_queue(input_queue)) return 0;
    sleep(parameters->quantum);
    return 1;
}





