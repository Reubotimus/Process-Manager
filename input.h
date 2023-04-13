#ifndef _INPUT 
#define _INPUT

#include <stdio.h>
#include <stdlib.h>
#include "memory-manager.h"
#include "process-scheduler.h"

typedef struct Parameters_Struct {
    FILE *file_pointer;
    enum Scheduler_Algorithm scheduler_algorithm;
    enum Memory_Strategy memory_strategy;
    int quantum;
} Parameters;

// inputs all new processes with an arival time less than the current time
void parse_new_processes(Queue input_queue, FILE *file_pointer, int time);

// parses the command line arguments, returning a struct with all relevant information
Parameters *get_parameters(int argc, char* argv[]);
#endif