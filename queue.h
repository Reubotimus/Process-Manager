#ifndef _QUEUE
#define _QUEUE 
#include "linked-list.h"

enum Queue_Type {STANDARD, PRIORITY};


typedef struct {
    Linked_List *list;
    enum Queue_Type type;
} Queue;


// creates a prority queue using a min-heap
Queue create_queue(enum Queue_Type type);

// checks if queue is empty
int is_empty_queue(Queue queue);

// frees all allocated memory associated with queue
void free_queue(Queue queue);

// returns the head of the queue and removes it from the queue
void *dequeue(Queue queue);

// inserts element at end of queue
void enqueue(Queue queue, void *element);
#endif