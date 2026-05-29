#include <stdio.h>
#include <string.h>
#include "packet.h"

Packet packet_create(int id, const char *dest, Priority prio, const char *content) {
    Packet p;
    p.id       = id;
    p.priority = prio;
    strncpy(p.destination, dest,    MAX_IP_LEN  - 1);
    strncpy(p.content,     content, MAX_CONT_LEN - 1);
    p.destination[MAX_IP_LEN  - 1] = '\0';
    p.content    [MAX_CONT_LEN - 1] = '\0';
    return p;
}

void packet_print(const Packet *p) {
    printf("[ID:%d | Destino:%-16s | Prio:%s | Conteudo:%s]",
           p->id,
           p->destination,
           p->priority == PRIO_HIGH ? "ALTA  " : "NORMAL",
           p->content);
}