#include "output.h"
#include <stdio.h>
#include <inttypes.h>

void printError() {
    fprintf(stderr, "ERROR\n");
}

void printOk() {
    printf("OK\n");
}

void printYes() {
    printf("YES\n");
}

void printNo() {
    printf("NO\n");
}

void printEnergy(uint64_t energy) {
    printf("%"PRIu64"\n", energy);
}
