#include "memory-manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define MEMORY_SIZE 2048
#define HOLE 1
#define NOT_HOLE 0

// creates a memory segment with the given values
Memory_Segment *create_memory_segment(int is_hole, int start, int end) {
    Memory_Segment *segment = malloc(sizeof(Memory_Segment));
    assert(segment);
    segment->is_hole = is_hole;
    segment->start = start;
    segment->end = end;
    return segment;
}
// creates linked list representing memory
Linked_List *create_memory_list() {
    Linked_List *list = create_list();
    insert_at_head(list, create_memory_segment(HOLE, 0, MEMORY_SIZE));
    return list;
}

// returns the memory segment contained by a node
Memory_Segment *node_segment(Node *node) {
    return (Memory_Segment*)node->data;
}
// returns the size of the memory segment held by a node
int memory_size(Node *node) {
    return node_segment(node)->end - node_segment(node)->start;
}

// helper function for find_best_fit checks if node1 is a better fit than node 2
int better_fit(Node *node1, Node *node2, Process *process) {
    if (memory_size(node1) < process->memory_required) return 0;
    if (memory_size(node2) < process->memory_required) return 1;
    if (!node_segment(node2)->is_hole) return 1;
    if (!node_segment(node1)->is_hole) return 0;
    return memory_size(node1) < memory_size(node2);
}

// returns the node with the best fit for the process
Node *find_best_fit(Linked_List *memory_list, Process *process) {
    Node *current_node = memory_list->head, *best_node = memory_list->head;
    while (current_node != NULL) {

        if (!node_segment(current_node)->is_hole) {
            current_node = current_node->next;
            continue;
        }

        if (better_fit(current_node, best_node, process)) {
            best_node = current_node;
        }
        current_node = current_node->next;
    }
    if (!node_segment(best_node)->is_hole) return NULL;
    if (memory_size(best_node) < process->memory_required) return NULL;
    return best_node;
}

// allocates memory from the given node to the process creates new segment to represent left over space
void allocate_node(Linked_List *list, Node *node, Process *process) {
    Memory_Segment *segment = node_segment(node);

    process->memory_address = segment->start;
    segment->is_hole = NOT_HOLE;
    int old_end = segment->end;
    segment->end = segment->start + process->memory_required;

    if (old_end == segment->end) return;
    
    if (node->next != NULL && node_segment(node->next)->is_hole) {
        node_segment(node->next)->start = segment->end;
        return;
    }

    // we need to create a new segment for the splinter
    Memory_Segment *splinter = create_memory_segment(HOLE, segment->end, old_end);
    Node *new_node = create_node(splinter, node->next);
    node->next = new_node;
    if (new_node->next == NULL) list->foot = new_node;
}

// allocates memory in the linked list to the process
void allocate_memory(Linked_List *memory_list, Queue input_queue, Queue ready_queue, int time) {
    // this will be used to hold all processes that are not able to have memory allocated to them
    Linked_List *new_input_queue_list = create_list(); 
    int n_remaining_processes = 0;
    while (!is_empty_queue(input_queue)) {
        Process *process_to_allocate = dequeue(input_queue);
        Node *best_node = find_best_fit(memory_list, process_to_allocate);
        
        if (best_node == NULL) {
            insert_at_foot(new_input_queue_list, process_to_allocate);
            n_remaining_processes++;
            continue;
        }

        allocate_node(memory_list, best_node, process_to_allocate);
        enqueue(ready_queue, process_to_allocate);
        printf("%d,READY,process_name=%s,assigned_at=%d\n", time, process_to_allocate->name, process_to_allocate->memory_address);

        
    }

    input_queue.list->head = new_input_queue_list->head;
    input_queue.list->foot = new_input_queue_list->foot;
    *input_queue.length = n_remaining_processes;
    free(new_input_queue_list);
}

// merges the memory segments and updates linked list
void merge_holes(Linked_List *memory_list, Node *first_node) {
    node_segment(first_node)->end = node_segment(first_node->next)->end;
    if (memory_list->foot == first_node->next) {
        memory_list->foot = first_node;
        free(first_node->next->data);
        free(first_node->next);
        first_node->next = NULL;
        return;
    }
    Node *temp = first_node->next;
    first_node->next = temp->next;
    free(temp->data);
    free(temp);
}

// deallocates memory associated with process
void deallocate_memory(Linked_List *memory_list, Process *process) {
    // finds memory segment associated with process
    Node *current_node = memory_list->head, *previous_to_segment = NULL, *segment_node = memory_list->head;
    while (current_node->next != NULL) {
        if (node_segment(current_node->next)->start == process->memory_address) {
            previous_to_segment = current_node;
            segment_node = current_node->next;
            break;
        }
        current_node = current_node->next;
    }

    // merges segment with previous segment if it is a hole
    node_segment(segment_node)->is_hole = HOLE;
    if (previous_to_segment != NULL && node_segment(previous_to_segment)->is_hole) {
        merge_holes(memory_list, previous_to_segment);
        segment_node = previous_to_segment;
    }

    // mergers segment with next segment if it is a hole
    if (segment_node->next != NULL && node_segment(segment_node->next)->is_hole) {
        merge_holes(memory_list, segment_node);
    }

    // ensures the linked list has an accurate foot
    if (segment_node->next == NULL) {
        memory_list->foot = segment_node;
    }
}