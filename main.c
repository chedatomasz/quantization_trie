#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "engine.h"

//Program written to spec available at https://moodle.mimuw.edu.pl/mod/assign/view.php?id=13388
//Author: Tomasz Cheda

int main() {
    setUpDataStructures();
    atexit(cleanUpDataStructures);
    while (nextLineAvailable()) {
        executeNextInstruction(getNextInstruction());
    }
    exit(0);
}