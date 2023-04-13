#ifndef _LINKED_LIST

#define _LINKED_LIST 

typedef struct {
    void *data;
    struct node *next;
} Node;

typedef struct {
    Node head;
    Node foot;
} Linked_List;

// makes an empty list
Linked_List *create_list();

// checks if there are any nodes in the linked list
int is_empty(Linked_List *list);

// inserts data at head
void insert_at_head(Linked_List *list, void *data);

// inserts data at foot
void insert_at_foot(Linked_List *list, void *data);

// returns the head of a list
Node get_head(Linked_List *list);

// removes the head of a list
void remove_head(Linked_List *list);

#endif