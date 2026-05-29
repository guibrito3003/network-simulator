#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "packet.h"

#define HT_TABLE_SIZE 17

typedef struct HT_Node {
    char         key[MAX_IP_LEN];
    Packet       value;
    int          hit_count;
    struct HT_Node *next;
} HT_Node;

typedef struct {
    HT_Node *buckets[HT_TABLE_SIZE];
    int      total_entries;
    int      collision_count;
} HashTable;

void         ht_init(HashTable *ht);
void         ht_free(HashTable *ht);
unsigned int ht_hash(const char *key);
void         ht_insert(HashTable *ht, const char *key, Packet p);
HT_Node     *ht_search(HashTable *ht, const char *key);
int          ht_remove(HashTable *ht, const char *key);
void         ht_print(const HashTable *ht);

#endif