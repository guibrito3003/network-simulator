#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "packet.h"

typedef struct DL_Node {
    Packet        data;
    struct DL_Node *prev;
    struct DL_Node *next;
} DL_Node;

typedef struct {
    DL_Node *head;
    DL_Node *tail;
    int      size;
} LinkedList;

void     ll_init(LinkedList *ll);
void     ll_free(LinkedList *ll);
void     ll_append(LinkedList *ll, Packet p);
void     ll_print_forward(const LinkedList *ll);
void     ll_print_backward(const LinkedList *ll);
DL_Node *ll_find_by_id(const LinkedList *ll, int id);
int      ll_is_empty(const LinkedList *ll);

#endif