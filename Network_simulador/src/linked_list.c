#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void ll_init(LinkedList *ll) {
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;
}

void ll_free(LinkedList *ll) {
    DL_Node *cur = ll->head;
    while (cur) {
        DL_Node *next = cur->next;
        free(cur);
        cur = next;
    }
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;
}

void ll_append(LinkedList *ll, Packet p) {
    DL_Node *node = (DL_Node *)malloc(sizeof(DL_Node));
    if (!node) {
        fprintf(stderr, "[ERRO] ll_append: falha ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
    node->data = p;
    node->next = NULL;
    node->prev = ll->tail;

    if (ll->tail)
        ll->tail->next = node;
    else
        ll->head = node;

    ll->tail = node;
    ll->size++;
}

void ll_print_forward(const LinkedList *ll) {
    printf("--- HISTORICO DE PROCESSAMENTO (LISTA ENCADEADA) ---\n");

    if (!ll->head) {
        printf("[INICIO] -> [FIM]\n\n");
        return;
    }

    printf("[INICIO]");
    DL_Node *cur = ll->head;
    while (cur) {
        printf(" -> [ID: %d | IP: %s]", cur->data.id, cur->data.destination);
        cur = cur->next;
    }
    printf(" -> [FIM]\n\n");
}

void ll_print_backward(const LinkedList *ll) {
    printf("--- HISTORICO (mais recente primeiro) ---\n");
    if (!ll->tail) { printf("[FIM] -> [INICIO]\n\n"); return; }

    printf("[FIM]");
    DL_Node *cur = ll->tail;
    while (cur) {
        printf(" -> [ID: %d | IP: %s]", cur->data.id, cur->data.destination);
        cur = cur->prev;
    }
    printf(" -> [INICIO]\n\n");
}

DL_Node *ll_find_by_id(const LinkedList *ll, int id) {
    DL_Node *cur = ll->head;
    while (cur) {
        if (cur->data.id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

int ll_is_empty(const LinkedList *ll) {
    return ll->size == 0;
}
