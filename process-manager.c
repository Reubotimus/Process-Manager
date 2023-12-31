#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "process-scheduler.h"
#include "memory-manager.h"
#include "queue.h"
#include "input.h"
#include "process-utils.h"
#define IMPLEMENTS_REAL_PROCESS

// performs one run cycle in the process manager, gets new processes, allocates memory and schedules a process for the next quantum
int run_cycle(Manager *manager, int time, Queue input_queue, Queue ready_queue, Queue finished_queue, Linked_List *memory_list);

// prints statistics regarding finished processes
void print_statistics(Queue finished_queue, int time);

int main(int argc, char *argv[]) {
    Manager *manager = get_manager(argc, argv);
    
    // initialises queues and running process, the type of queue is according to efficiency
    Queue input = create_queue(STANDARD); 
    Queue finished = create_queue(STANDARD);
    Queue ready = {NULL, 0, NULL};
    Linked_List *memory_list = create_memory_list();
    if (manager->scheduler_algorithm == SJF) ready = create_queue(PRIORITY);
    else ready = create_queue(STANDARD);
    int time = 0;

    // keeps running until there are no processes left to be ran
    while (run_cycle(manager, time, input, ready, finished, memory_list)) time += manager->quantum;

    print_statistics(finished, time);

    // frees all associated memory
    fclose(manager->file_pointer);
    free_queue(ready);
    free_queue(input);
    free_queue(finished);
    free(memory_list->head->data);
    free_list(memory_list);
    free(manager);
}

// performs one run cycle in the process manager, gets new processes, allocates memory and schedules a process for the next quantum
int run_cycle(Manager *manager, int time, Queue input_queue, Queue ready_queue, Queue finished_queue, Linked_List *memory_list) {
    // checks if running process is finished and removes it if so
    manager->running_process = check_running_process(memory_list, finished_queue, manager->running_process, time, (*ready_queue.length) + (*input_queue.length));
    
    // checks if all processes are finished
    if (manager->running_process == NULL && feof(manager->file_pointer) && is_empty_queue(ready_queue) && is_empty_queue(input_queue)) return 0;

    // parses new processes and allocates memory if needed
    if (manager->memory_strategy == INFINITE) {
        parse_new_processes(ready_queue, manager->file_pointer, time);
    } else {
        parse_new_processes(input_queue, manager->file_pointer, time);
        allocate_memory(memory_list, input_queue, ready_queue, time);
    }

    // selects new process and decreases its time remaining by a quantum
    manager->running_process = select_new_process(ready_queue, manager->running_process, time);
    if (manager->running_process != NULL) manager->running_process->time_remaining -= manager->quantum;
    return 1;
}

// prints statistics regarding finished processes
void print_statistics(Queue finished_queue, int time) {
    int sum_turnaround_times = 0, n_process = *finished_queue.length, current_turnaround_time = 0;
    double max_overhead = 0, sum_overhead = 0, current_overhead = 0;
    Process* current_process = NULL;

    for (Node *node = finished_queue.list->head; node != NULL; node = node->next) {
        current_process = (Process*)node->data;
        current_turnaround_time = current_process->time_finished - current_process->arrival_time;
        sum_turnaround_times += current_turnaround_time;

        current_overhead = (double)(current_turnaround_time) / (double)(current_process->service_time);
        sum_overhead += current_overhead;
        if (current_overhead > max_overhead) max_overhead = current_overhead;

        free_process(node->data);
    }
    printf("Turnaround time %d\n", ((sum_turnaround_times - 1) / n_process) + 1);
    printf("Time overhead %.2lf %.2lf\n", round(max_overhead*100) / 100, round(sum_overhead / (double) n_process * 100) / 100);
    printf("Makespan %d\n", time);


}


