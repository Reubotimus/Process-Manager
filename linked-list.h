#ifndef _LINKED_LIST

#define _LINKED_LIST 

typedef struct node_struct {
    void *data;
    struct node_struct *next;
} Node;

typedef struct {
    Node *head;
    Node *foot;
} Linked_List;

// makes an empty list
Linked_List *create_list();

// checks if there are any nodes in the linked list
int is_empty(Linked_List *list);

// inserts data at head
void insert_at_head(Linked_List *list, void *data);

// removes the head of a list
void remove_head(Linked_List *list);

// frees linked list
void free_list(Linked_List *list);

#endif