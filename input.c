# include "input.h"

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

