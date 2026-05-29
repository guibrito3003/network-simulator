#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "packet.h"

#define PQ_INITIAL_CAPACITY 16

typedef struct {
    Packet *data;
    int     size;
    int     capacity;
} PriorityQueue;

void pq_init(PriorityQueue *pq);
void pq_free(PriorityQueue *pq);
void pq_enqueue(PriorityQueue *pq, Packet p);
int  pq_dequeue(PriorityQueue *pq, Packet *out);
int  pq_peek(const PriorityQueue *pq, Packet *out);
int  pq_is_empty(const PriorityQueue *pq);
void pq_print(const PriorityQueue *pq);

#endif