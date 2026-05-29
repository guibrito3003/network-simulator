#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

unsigned int ht_hash(const char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*key++))
        hash = hash * 31 + c;
    return (unsigned int)(hash % HT_TABLE_SIZE);
}

void ht_init(HashTable *ht) {
    ht->total_entries   = 0;
    ht->collision_count = 0;
    for (int i = 0; i < HT_TABLE_SIZE; i++)
        ht->buckets[i] = NULL;
}

void ht_free(HashTable *ht) {
    for (int i = 0; i < HT_TABLE_SIZE; i++) {
        HT_Node *cur = ht->buckets[i];
        while (cur) {
            HT_Node *next = cur->next;
            free(cur);
            cur = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->total_entries   = 0;
    ht->collision_count = 0;
}

void ht_insert(HashTable *ht, const char *key, Packet p) {
    unsigned int idx = ht_hash(key);
    HT_Node *cur = ht->buckets[idx];

    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            cur->value     = p;
            cur->hit_count++;
            return;
        }
        cur = cur->next;
    }

    if (ht->buckets[idx] != NULL)
        ht->collision_count++;

    HT_Node *node = (HT_Node *)malloc(sizeof(HT_Node));
    if (!node) {
        fprintf(stderr, "[ERRO] ht_insert: falha ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(node->key, key, MAX_IP_LEN - 1);
    node->key[MAX_IP_LEN - 1] = '\0';
    node->value     = p;
    node->hit_count = 1;
    node->next      = ht->buckets[idx];
    ht->buckets[idx] = node;
    ht->total_entries++;
}

HT_Node *ht_search(HashTable *ht, const char *key) {
    unsigned int idx = ht_hash(key);
    HT_Node *cur = ht->buckets[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

int ht_remove(HashTable *ht, const char *key) {
    unsigned int idx = ht_hash(key);
    HT_Node *cur  = ht->buckets[idx];
    HT_Node *prev = NULL;

    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                ht->buckets[idx] = cur->next;
            free(cur);
            ht->total_entries--;
            return 1;
        }
        prev = cur;
        cur  = cur->next;
    }
    return 0;
}

void ht_print(const HashTable *ht) {
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               ESTADO DO CACHE (Tabela Hash)                 ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Tamanho da tabela : %d buckets (primo)                     ║\n", HT_TABLE_SIZE);
    printf("║  Entradas ativas   : %d                                     ║\n", ht->total_entries);
    printf("║  Colisoes detectadas: %d                                    ║\n", ht->collision_count);
    printf("╚══════════════════════════════════════════════════════════════╝\n");

    for (int i = 0; i < HT_TABLE_SIZE; i++) {
        printf("  bucket[%2d]: ", i);
        if (ht->buckets[i] == NULL) {
            printf("(vazio)\n");
        } else {
            HT_Node *cur = ht->buckets[i];
            while (cur) {
                printf("{ IP:%-16s | Hits:%d | Ultimo pacote ID:%d }",
                       cur->key, cur->hit_count, cur->value.id);
                if (cur->next) printf(" --> [COLISAO] ");
                cur = cur->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}