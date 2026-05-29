#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"

#define CMD_BUF 512

void sim_init(Simulator *sim) {
    pq_init(&sim->queue);
    ht_init(&sim->cache);
    ll_init(&sim->history);
    sim->processed_count = 0;
    printf("[SISTEMA INICIALIZADO - Tabela Hash de tamanho %d]\n\n", HT_TABLE_SIZE);
}

void sim_free(Simulator *sim) {
    pq_free(&sim->queue);
    ht_free(&sim->cache);
    ll_free(&sim->history);
}

void sim_add_packet(Simulator *sim, Packet p) {
    printf(">> Lendo comando: ADD (Pacote %d - Prioridade: %s)\n",
           p.id,
           p.priority == PRIO_HIGH ? "ALTA" : "Normal");
    printf("Status: Pacote %d inserido na fila.\n\n", p.id);
    pq_enqueue(&sim->queue, p);
}

int sim_process(Simulator *sim) {
    Packet p;
    if (!pq_dequeue(&sim->queue, &p)) {
        printf(">> Lendo comando: PROCESS\n");
        printf("Status: Fila vazia - nenhum pacote para processar.\n\n");
        return 0;
    }

    printf(">> Lendo comando: PROCESS\n");
    printf("Executando: Pacote %d (%s) - Prioridade %s.\n",
           p.id,
           p.content,
           p.priority == PRIO_HIGH ? "ALTA" : "Normal");

    HT_Node *cached = ht_search(&sim->cache, p.destination);
    if (cached) {
        cached->hit_count++;
        cached->value = p;
        printf("Cache: IP %s ENCONTRADO (Acesso rapido!).\n", p.destination);
    } else {
        printf("Cache: IP %s nao encontrado. Mapeando novo destino na Hash...\n", p.destination);
        ht_insert(&sim->cache, p.destination, p);
    }

    ll_append(&sim->history, p);
    sim->processed_count++;
    printf("Historico: Pacote %d adicionado a Lista de Eventos.\n\n", p.id);
    return 1;
}

void sim_show_cache(const Simulator *sim) {
    printf(">> Lendo comando: SHOW_CACHE\n");
    printf("--- ESTADO DA TABELA HASH ---\n");
    for (int i = 0; i < HT_TABLE_SIZE; i++) {
        if (sim->cache.buckets[i] != NULL) {
            HT_Node *cur = sim->cache.buckets[i];
            while (cur) {
                printf("[Indice %d]: IP %s (Pacote %d)\n",
                       i, cur->key, cur->value.id);
                if (cur->next)
                    printf("[Indice %d]: ** COLISAO ** IP %s tambem neste bucket\n",
                           i, cur->next->key);
                cur = cur->next;
            }
        }
    }
    printf("[Demais indices vazios ou com colisoes tratadas]\n\n");
}

void sim_show_history(const Simulator *sim) {
    printf(">> Lendo comando: SHOW_HISTORY\n");
    ll_print_forward(&sim->history);
}

void sim_report(const Simulator *sim) {
    printf("\n--- RELATORIO FINAL DE TRAFEGO ---\n");
    printf("Pacotes processados     : %d\n", sim->processed_count);
    printf("Pacotes ainda na fila   : %d\n", sim->queue.size);
    printf("Entradas no cache       : %d\n", sim->cache.total_entries);
    printf("Colisoes na hash        : %d\n", sim->cache.collision_count);
    printf("Entradas no historico   : %d\n", sim->history.size);
    printf("----------------------------------\n\n");
}

static void sim_parse_line(Simulator *sim, const char *line) {
    if (!line || line[0] == '\0' || line[0] == '#') return;

    char buf[CMD_BUF];
    strncpy(buf, line, CMD_BUF - 1);
    buf[CMD_BUF - 1] = '\0';

    char *nl = strchr(buf, '\n'); if (nl) *nl = '\0';
    char *cr = strchr(buf, '\r'); if (cr) *cr = '\0';
    if (strlen(buf) == 0) return;

    char *cmd = strtok(buf, ";");
    if (!cmd) return;

    if (strcmp(cmd, "ADD") == 0) {
        char *sid  = strtok(NULL, ";");
        char *ip   = strtok(NULL, ";");
        char *spri = strtok(NULL, ";");
        char *cont = strtok(NULL, ";");
        if (!sid || !ip || !spri || !cont) {
            printf(">> [ERRO] Formato invalido para ADD.\n\n");
            return;
        }
        int      id   = atoi(sid);
        Priority prio = (atoi(spri) == 1) ? PRIO_HIGH : PRIO_NORMAL;
        Packet   p    = packet_create(id, ip, prio, cont);
        sim_add_packet(sim, p);

    } else if (strcmp(cmd, "PROCESS") == 0) {
        sim_process(sim);
    } else if (strcmp(cmd, "SHOW_CACHE") == 0) {
        sim_show_cache(sim);
    } else if (strcmp(cmd, "SHOW_HISTORY") == 0) {
        sim_show_history(sim);
    } else if (strcmp(cmd, "REPORT") == 0) {
        sim_report(sim);
    } else {
        printf(">> [AVISO] Comando desconhecido: %s\n\n", cmd);
    }
}

void sim_run_file(Simulator *sim, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "[ERRO] Nao foi possivel abrir '%s'.\n", filename);
        return;
    }
    char line[CMD_BUF];
    while (fgets(line, sizeof(line), fp))
        sim_parse_line(sim, line);
    fclose(fp);
    sim_report(sim);
}

void sim_run_interactive(Simulator *sim) {
    printf("Comandos: ADD;ID;IP;PRIO;CONTEUDO | PROCESS | SHOW_CACHE | SHOW_HISTORY | REPORT | QUIT\n\n");
    char line[CMD_BUF];
    while (1) {
        printf("sim> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        char *nl = strchr(line, '\n'); if (nl) *nl = '\0';
        if (strcmp(line, "QUIT") == 0 || strcmp(line, "EXIT") == 0) {
            sim_report(sim);
            break;
        }
        sim_parse_line(sim, line);
    }
}