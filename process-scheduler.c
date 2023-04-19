#include "process-scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#define VERBOSE 0

void print_new_process(Process *process, int time) {
    printf("%d,RUNNING,process_name=%s,remaining_time=%d\n", time, process->name, process->time_remaining);
}
// selects the next process to run from the ready queue
Process *select_new_process(Queue ready_queue, Process *running_process, int time) {
    if (running_process == NULL && is_empty_queue(ready_queue)) return NULL;
    if (running_process == NULL) {
        Process *new_process = dequeue(ready_queue);
        if (new_process->pid == -1) {
            create_forked_process(new_process, time, 0);
        } else {
            resume_process(new_process, time);
        }
        print_new_process(new_process, time);
        return new_process;
    }
    if (ready_queue.type == PRIORITY) {resume_process(running_process, time); return running_process;}
    else {
        if (*ready_queue.length == 0) {
            resume_process(running_process, time);
            return running_process;
        }

        suspend_process(running_process, time);
        enqueue(ready_queue, running_process);
        Process *new_process = dequeue(ready_queue);
        
        if (new_process->pid == -1) {
            create_forked_process(new_process, time, 0);
        } else {
            resume_process(new_process, time);
        }

        print_new_process(new_process, time);
        return new_process;
    }
}