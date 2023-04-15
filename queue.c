#include "queue.h"
#include <stdlib.h>
#include <assert.h>

/* the process header is only used for the less_than function if the data 
type is changed only the less_than function has to be changed
*/ 
#include "process.h"
int less_than(Node *node_1, Node *node_2) {
    return ((Process *)node_1->data)->time_remaining < ((Process *)node_2->data)->time_remaining;
}

// creates a prority queue using a min-heap
Queue create_queue(enum Queue_Type type) {
    Linked_List *list = create_list();
    Queue queue = {list, type};
    return queue;
}

// checks if queue is empty
int is_empty_queue(Queue queue) {
    return is_empty(queue.list);
}

// frees all allocated memory associated with queue
void free_queue(Queue queue) {
    free_list(queue.list);
}


// inserts element at end of queue
void enqueue(Queue queue, void *element) {
    insert_at_foot(queue.list, element);
}

// returns the head of the queue and removes it from the queue
void *dequeue(Queue queue) {
    // ensures the queue is not empty
    assert(!is_empty_queue(queue));

    // if standard queue, removes and returns head of linked list
    if (queue.type == STANDARD) {
        void *data = queue.list->head->data;
        remove_head(queue.list);
        return data;
    }

    // finds highest priority node
    Node *current_node = queue.list->head, *previous_to_min = NULL, *min_node = queue.list->head;
    while (current_node->next != NULL) {
        if (less_than(current_node->next, min_node)) {
            min_node = current_node->next;
            previous_to_min = current_node;
        }
        current_node = current_node->next;
    }

    // removes node from list and returns the data
    void *data = min_node->data;
    if (min_node == queue.list->head) {
        remove_head(queue.list);
    } else if (min_node == queue.list->foot) {
        free(min_node);
        queue.list->foot = previous_to_min;
        queue.list->foot->next = NULL;
    }
    else {
        free(min_node);
        previous_to_min->next = min_node->next;
    }
    return data;
}
