#include "process-utils.h"
#include "memory-manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 65

// frees the memory from a process
void free_process(Process *process) {
    free(process->name);
    free(process);
}

// creates a process with the given values
Process *create_process(int arrival_time, int time_remaining, char *name, int memory_required) {
    Process *new_process = malloc(sizeof(Process));
    assert(new_process);
    new_process->arrival_time = arrival_time;
    new_process->service_time = time_remaining;
    new_process->time_remaining = time_remaining;
    new_process->name = strdup(name);
    new_process->memory_required = memory_required;
    new_process->memory_address = -1;
    new_process->pid = -1;
    return new_process;
}

// checks if process is finished running
Process *check_running_process(Linked_List *memory_list, Queue finished_queue, Process *running_process, int time, int num_processes_left) {
    if (running_process == NULL) return NULL;

    if (running_process->time_remaining <= 0) {
        printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n", time, running_process->name, num_processes_left);
        terminate_process(running_process, time);
        deallocate_memory(memory_list, running_process);
        running_process->time_finished = time;
        enqueue(finished_queue, running_process);
        return NULL;
    }

    return running_process;
}

// prints the values of a process mainly for debugging and testing
void print_process(Process *process) {
    printf("Name: %s, Time Remaining: %d\n", process->name, process->time_remaining);
}

void create_forked_process(Process *process, int time, int verbose) {
    
    int stdin_pipe[2], stdout_pipe[2];

    if (pipe(stdin_pipe) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(stdout_pipe) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();

    if (pid == 0) {
        if (verbose) {
            fprintf(stderr, "Debug logs enabled.\n");
        }
        
        close(stdin_pipe[1]);  // close write end of stdin pipe
        close(stdout_pipe[0]); // close read end of stdout pipe

        dup2(stdin_pipe[0], STDIN_FILENO);   // redirect stdin to the read end of the stdin pipe
        dup2(stdout_pipe[1], STDOUT_FILENO); // redirect stdout to the write end of the stdout pipe

        close(stdin_pipe[0]);
        close(stdout_pipe[1]);
        char *argv[] = {"./process", process->name, NULL};
        execvp("./process", argv); // replace child process with "process" executable

        // execve() only returns on error
        perror("execve");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    close(stdin_pipe[0]);   // close read end of stdin pipe
    close(stdout_pipe[1]);  // close write end of stdout pipe

    process->pid = pid;
    process->stdin_fd = stdin_pipe[1];
    process->stdout_fd = stdout_pipe[0];

    uint8_t bytes[4] = {
                        ((time >> (8 * 3)) & 0xff),
                        ((time >> (8 * 2)) & 0xff), 
                        ((time >> (8 * 1)) & 0xff),
                        ((time >> (8 * 0)) & 0xff)
                        };

    // Send 32 bit simulation time of when the process starts running to child process
    write(process->stdin_fd, bytes, 4*sizeof(uint8_t));

    // Read least significant byte (last byte) from child process's stdout and verify that it matches the start time
    uint8_t ls_byte;
    read(process->stdout_fd, &ls_byte, sizeof(ls_byte));
    if (ls_byte != bytes[3]) {
        printf("Error: least significant byte mismatch\n");
        exit(EXIT_FAILURE);
    }

}

void suspend_process(Process *process, int time) {
    uint8_t bytes[4] = {
                        ((time >> (8 * 3)) & 0xff),
                        ((time >> (8 * 2)) & 0xff), 
                        ((time >> (8 * 1)) & 0xff),
                        ((time >> (8 * 0)) & 0xff)
                        };

    // Send 32 bit simulation time of when the process starts running to child process
    write(process->stdin_fd, bytes, 4*sizeof(uint8_t));
    
    pid_t child_pid = process->pid;
    if (kill(child_pid, SIGTSTP) == -1) {
        perror("kill failed");
        exit(EXIT_FAILURE);
    }

    int wstatus;
    waitpid(child_pid, &wstatus, WUNTRACED);

    while (!WIFSTOPPED(wstatus)) {
        waitpid(child_pid, &wstatus, WUNTRACED);
    }

    
}

void resume_process(Process *process, int time) {
    uint8_t bytes[4] = {
                        ((time >> (8 * 3)) & 0xff),
                        ((time >> (8 * 2)) & 0xff), 
                        ((time >> (8 * 1)) & 0xff),
                        ((time >> (8 * 0)) & 0xff)
                        };

    // Send 32 bit simulation time of when the process starts running to child process
    write(process->stdin_fd, bytes, 4*sizeof(uint8_t));

    pid_t child_pid = process->pid;
    if (kill(child_pid, SIGCONT) == -1) {
        perror("kill failed");
        exit(EXIT_FAILURE);
    }

    // Read least significant byte (last byte) from child process's stdout and verify that it matches the start time
    uint8_t ls_byte;
    read(process->stdout_fd, &ls_byte, sizeof(ls_byte));
    if (ls_byte != bytes[3]) {
        printf("Error: least significant byte mismatch\n");
        exit(EXIT_FAILURE);
    }
}

void terminate_process(Process *process, int time) {
    uint8_t bytes[4] = {
                        ((time >> (8 * 3)) & 0xff),
                        ((time >> (8 * 2)) & 0xff), 
                        ((time >> (8 * 1)) & 0xff),
                        ((time >> (8 * 0)) & 0xff)
                        };
    // Send 32 bit simulation time of when the process is terminated
    write(process->stdin_fd, bytes, 4*sizeof(uint8_t));

    // Send SIGTERM signal to process
    if (kill(process->pid, SIGTERM) == -1) {
        perror("kill failed");
        exit(EXIT_FAILURE);
    }

    // Read 64-byte string from standard output of process
    char buffer[BUFFER_SIZE];
    int n = read(process->stdout_fd, buffer, BUFFER_SIZE - 1);
    if (n == -1) {
        perror("read from process failed");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("%d,FINISHED-PROCESS,process_name=%s,sha=%s\n", time, process->name, buffer);
}