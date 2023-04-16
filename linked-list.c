#include "linked-list.h"
#include <stdlib.h>
#include <assert.h>

// makes an empty list
Linked_List *create_list() {
    Linked_List *linked_list = malloc(sizeof(Linked_List));
    assert(linked_list);
    linked_list->head = NULL;
    linked_list->foot = NULL;
    return linked_list;
}

// checks if there are any nodes in the linked list
int is_empty(Linked_List *list) {
    return list->head == NULL;
}

// creates node for a linked list
Node *create_node(void *data, Node *next) {
    Node *node = malloc(sizeof(Node));
    assert(node);
    node->data = data;
    node->next = next;
    return node;
} 

// inserts data at head
void insert_at_head(Linked_List *list, void *data) {
    if (list->head == NULL) {
        list->head = create_node(data, NULL);
        list->foot = list->head;
    }
    else {
        Node *second_node = list->head;
        list->head = create_node(data, second_node);
    }
}

// inserts data at foot
void insert_at_foot(Linked_List *list, void *data) {
    if (list->head == NULL) {
        list->head = create_node(data, NULL);
        list->foot = list->head;
    }
    else {
        Node *second_last_node = list->foot;
        list->foot = create_node(data, NULL);
        second_last_node->next = list->foot;
    }
}

// removes the head of a list
void remove_head(Linked_List *list) {
    if (list->head == NULL) return;

    if (list->head == list->foot) {
        free(list->head);
        list->head = NULL;
        list->foot = NULL;
        return;
    }

    Node *temp = list->head->next;
    free(list->head);
    list->head = temp;
}

// frees linked list
void free_list(Linked_List *list) {
    Node *current_node = list->head, *next;
    while (current_node != NULL) {
        next = current_node->next;
        free(current_node);
        current_node = next;
    }
    free(list);
}
