#ifndef PACKET_H
#define PACKET_H

#define MAX_IP_LEN   32
#define MAX_CONT_LEN 128

typedef enum {
    PRIO_NORMAL    = 0,
    PRIO_HIGH      = 1
} Priority;

typedef struct {
    int      id;
    char     destination[MAX_IP_LEN];
    Priority priority;
    char     content[MAX_CONT_LEN];
} Packet;

Packet packet_create(int id, const char *dest, Priority prio, const char *content);
void packet_print(const Packet *p);

#endif