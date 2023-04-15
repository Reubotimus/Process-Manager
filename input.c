#include "input.h"

Parameters *get_parameters(int argc, char* argv[]) {
    Parameters *parameters = malloc(sizeof(Parameters));

    // checks all flags, saving information to parameters
    for (int i = 1; i < argc; i += 2) {
        if (argv[i][0] == '-' && argv[i][1] == 'f') {
            parameters->file_pointer = fopen(argv[i + 1], "r");
        }

        else if (argv[i][0] == '-' && argv[i][1] == 's') {
            if (argv[i + 1][0] == 'S') parameters->scheduler_algorithm = SJF;
            else parameters->scheduler_algorithm = RR;
        }

        else if (argv[i][0] == '-' && argv[i][1] == 'q') {
            parameters->quantum = atoi(argv[i + 1]);
        }

        else if (argv[i][0] == '-' && argv[i][1] == 'm') {
            if (argv[i + 1][0] == 'i') parameters->memory_strategy = INFINITE;
            else parameters->memory_strategy = BEST_FIT;
        }
        
    }
    return parameters;
}

// inputs all new processes with an arival time less than the current time
void parse_new_processes(Queue input_queue, FILE *file_pointer, int time) {
    int arrival_time, time_remaining, memory_required, file_pointer_position;
    char name[256];
    while (!feof(file_pointer)) {
        file_pointer_position = ftell(file_pointer);
        if(fscanf(file_pointer, "%d %s %d %d", &arrival_time, name, &time_remaining, &memory_required) != 4) break;

        if (arrival_time > time) {
            fseek(file_pointer, file_pointer_position, SEEK_SET);
            break;
        }
        Process *p = create_process(arrival_time, time_remaining, name, memory_required);
        enqueue(input_queue, p);
    }
}
