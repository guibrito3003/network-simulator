#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "priority_queue.h"
#include "hash_table.h"
#include "linked_list.h"

typedef struct {
    PriorityQueue queue;
    HashTable     cache;
    LinkedList    history;
    int           processed_count;
} Simulator;

void sim_init(Simulator *sim);
void sim_free(Simulator *sim);
void sim_add_packet(Simulator *sim, Packet p);
int  sim_process(Simulator *sim);
void sim_show_cache(const Simulator *sim);
void sim_show_history(const Simulator *sim);
void sim_run_file(Simulator *sim, const char *filename);
void sim_run_interactive(Simulator *sim);
void sim_report(const Simulator *sim);

#endif