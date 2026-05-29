#include <stdio.h>
#include "../include/simulator.h"

int main(int argc, char *argv[]) {
    Simulator sim;
    sim_init(&sim);

    if (argc >= 2)
        sim_run_file(&sim, argv[1]);
    else
        sim_run_interactive(&sim);

    sim_free(&sim);
    return 0;
}