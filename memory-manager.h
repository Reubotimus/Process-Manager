#ifndef _MEMORY_MANAGER 
#define _MEMORY_MANAGER 
#include "queue.h"
#include "process.h"

enum Memory_Strategy {INFINITE, BEST_FIT};

typedef struct {
    int is_hole;
    int start;
    int end;
} Memory_Segment;

// creates linked list representing memory
Linked_List *create_memory_list();

// allocates memory in the linked list to the process
void allocate_memory(Linked_List *memory_list, Queue input_queue, Queue ready_queue, int time);

// deallocate memory
void deallocate_memory(Linked_List *memory_list, Process *process);
#endif