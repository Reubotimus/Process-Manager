#ifndef _PROCESS_SCHEDULER
#define _PROCESS_SCHEDULER 

#include "process-utils.h"
#include "queue.h"

enum Scheduler_Algorithm {SJF, RR};

// selects the next process to run from the ready queue
Process *select_new_process(Queue ready_queue, Process *running_process, int time);

#endif