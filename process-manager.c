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
int run_cycle(Manager *manager, int time, Queue input_queue, Queue ready_queue, Linked_List *memory_list);


int main(int argc, char *argv[]) {
    Manager *manager = get_manager(argc, argv);
    
    // initialises queues and running process, the type of queue is according to efficiency
    Queue input = create_queue(STANDARD); 
    Queue ready;
    Linked_List *memory_list = create_list();
    if (manager->scheduler_algorithm == SJF) ready = create_queue(PRIORITY);
    else ready = create_queue(STANDARD);
    int time = 0;

    while (run_cycle(manager, time, input, ready, memory_list)) time += manager->quantum;

    fclose(manager->file_pointer);
    free_queue(ready);
    free_queue(input);
    free(manager);
}

int run_cycle(Manager *manager, int time, Queue input_queue, Queue ready_queue, Linked_List *memory_list) {
    manager->running_process = check_running_process(memory_list, manager->running_process, time, (*ready_queue.length) + (*input_queue.length));
    //printf("Checked running processs\n");
    if (manager->running_process == NULL && feof(manager->file_pointer) && is_empty_queue(ready_queue) && is_empty_queue(input_queue)) return 0;
    //printf("Checked if end of manager\n");
    if (manager->memory_strategy == INFINITE) {
        parse_new_processes(ready_queue, manager->file_pointer, time);
    } else {
        parse_new_processes(input_queue, manager->file_pointer, time);
        //printf("parsed new processes\n");
        allocate_memory(memory_list, input_queue, ready_queue, time);
        //printf("allocated new processe size = %d\n", *input_queue.length);
    }

    manager->running_process = select_new_process(ready_queue, manager->running_process, time);
    //sleep(manager->quantum);
    if (manager->running_process != NULL) manager->running_process->time_remaining -= manager->quantum;
    return 1;
}





