#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

static int pq_compare(const Packet *a, const Packet *b) {
    if (a->priority != b->priority)
        return a->priority > b->priority;
    return a->id < b->id;
}

static void pq_swap(Packet *a, Packet *b) {
    Packet tmp = *a;
    *a = *b;
    *b = tmp;
}

static void pq_heapify_up(PriorityQueue *pq, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq_compare(&pq->data[i], &pq->data[parent])) {
            pq_swap(&pq->data[i], &pq->data[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

static void pq_heapify_down(PriorityQueue *pq, int i) {
    while (1) {
        int largest = i;
        int left    = 2 * i + 1;
        int right   = 2 * i + 2;

        if (left  < pq->size && pq_compare(&pq->data[left],  &pq->data[largest]))
            largest = left;
        if (right < pq->size && pq_compare(&pq->data[right], &pq->data[largest]))
            largest = right;

        if (largest == i) break;

        pq_swap(&pq->data[i], &pq->data[largest]);
        i = largest;
    }
}

void pq_init(PriorityQueue *pq) {
    pq->size     = 0;
    pq->capacity = PQ_INITIAL_CAPACITY;
    pq->data     = (Packet *)malloc(pq->capacity * sizeof(Packet));
    if (!pq->data) {
        fprintf(stderr, "[ERRO] pq_init: falha ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
}

void pq_free(PriorityQueue *pq) {
    free(pq->data);
    pq->data     = NULL;
    pq->size     = 0;
    pq->capacity = 0;
}

void pq_enqueue(PriorityQueue *pq, Packet p) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->data = (Packet *)realloc(pq->data, pq->capacity * sizeof(Packet));
        if (!pq->data) {
            fprintf(stderr, "[ERRO] pq_enqueue: falha ao realocar memoria.\n");
            exit(EXIT_FAILURE);
        }
    }
    pq->data[pq->size] = p;
    pq->size++;
    pq_heapify_up(pq, pq->size - 1);
}

int pq_dequeue(PriorityQueue *pq, Packet *out) {
    if (pq->size == 0) return 0;

    *out = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    if (pq->size > 0)
        pq_heapify_down(pq, 0);
    return 1;
}

int pq_peek(const PriorityQueue *pq, Packet *out) {
    if (pq->size == 0) return 0;
    *out = pq->data[0];
    return 1;
}

int pq_is_empty(const PriorityQueue *pq) {
    return pq->size == 0;
}

void pq_print(const PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("  (fila vazia)\n");
        return;
    }
    for (int i = 0; i < pq->size; i++) {
        printf("  [%d] ", i);
        packet_print(&pq->data[i]);
        printf("\n");
    }
}