#include <stdlib.h>
#include <ctype.h>
#include "engine.h"
#include "output.h"
#include "structures.h"
#include "safe_alloc.h"

#define CONVERSION_ERROR 1
#define CONVERSION_SUCCESSFUL 0

//Executes the DECLARE command using structure-specific functions.
static void declare(History h) {
    trieCreateHistory(h);
    printOk();
    free(h.states);
}

//Executes the REMOVE command using structure-specific functions.
static void remove(History h) {
    trieRemoveHistory(h);
    printOk();
    free(h.states);
}

//Executes the VALID command using structure-specific functions.
static void valid(History h) {
    if (trieValid(h)) {
        printYes();
    }
    else {
        printNo();
    }
    free(h.states);
}

//Executes the ENERGY HISTORY ENERGY command using structure-specific functions.
static void energy2(History h, Energy e) {
    if (trieSetEnergy(h, e)) {
        printOk();
    }
    else {
        printError();
    }
}

//Executes the ENERGY HISTORY command using structure-specific functions.
static void energy1(History h) {
    uint64_t buffer = trieGetEnergy(h);
    if (buffer != 0) {
        printEnergy(buffer);
    }
    else {
        printError();
    }
}

//Executes the EQUAL command using structure-specific functions.
static void equal(History h1, History h2) {
    if (trieEqualStates(h1, h2)) {
        printOk();
    }
    else {
        printError();
    }
}

//Converts the int array at input to a History struct.
//Too short histories are already handled at input by readDigitBlock.
static int convertHistory(int *input, uint64_t input_length, History *output) {
    output->states = safe_malloc(sizeof(int) * input_length);
    for (uint64_t i = 0; i < input_length; i++) {
        if (input[i] >= (int) '0' && input[i] <= (int) '0' + CHILDREN - 1) {
            output->states[i] = input[i] - (int) '0';
        }
        else {
            free(input);
            free(output->states);
            return CONVERSION_ERROR;
        }
    }
    output->length = input_length;
    free(input);
    return CONVERSION_SUCCESSFUL;
}

//Converts the int array at input to an Energy struct.
static int convertEnergy(int *input, uint64_t input_length, Energy *output) {
    output->value = 0;
    for (uint64_t i = 0; i < input_length; i++) {
        if (isdigit(input[i]) &&
            output->value <= (UINT64_MAX - (input[i] - (int) '0')) / 10) {
            output->value *= 10;
            output->value += input[i] - (int) '0';
        }
        else {
            output->value = 0;
            break;
        }
    }
    free(input);
    if (output->value == 0) {
        return CONVERSION_ERROR;
    }
    return CONVERSION_SUCCESSFUL;
}

void executeNextInstruction(Instruction *instruction) {
    if (instruction->validity == EMPTY) {
        free(instruction->arg1);
        free(instruction->arg2);
        free(instruction);
        return;
    }

    if (instruction->validity == INCORRECT) {
        printError();
        free(instruction->arg1);
        free(instruction->arg2);
        free(instruction);
        return;
    }

    History h1;
    h1.states = NULL;

    if (convertHistory(instruction->arg1, instruction->arg1_length, &h1) !=
        CONVERSION_SUCCESSFUL) {
        printError();
        if (instruction->signal == ENERGY2 ||
            instruction->signal ==
            EQUAL) {//Freeing buffered 2nd arg for 2 arg commands
            free(instruction->arg2);
        }
        free(instruction);
        return;
    }

    if (instruction->signal == DECLARE) {
        declare(h1);
        free(instruction);
        return;
    }
    if (instruction->signal == REMOVE) {
        remove(h1);
        free(instruction);
        return;
    }
    if (instruction->signal == VALID) {
        valid(h1);
        free(instruction);
        return;
    }
    if (instruction->signal == ENERGY1) {
        energy1(h1);
        free(instruction);
        return;
    }
    if (instruction->signal == ENERGY2) {
        Energy e;
        if (convertEnergy(instruction->arg2, instruction->arg2_length, &e) !=
            CONVERSION_SUCCESSFUL) {
            printError();
            free(h1.states);
            free(instruction);
            return;
        }
        energy2(h1, e);
        free(instruction);
        return;
    }
    if (instruction->signal == EQUAL) {
        History h2;
        if (convertHistory(instruction->arg2, instruction->arg2_length, &h2) !=
            CONVERSION_SUCCESSFUL) {
            printError();
            free(h1.states);
            free(instruction);
            return;
        }
        equal(h1, h2);
        free(instruction);
        return;
    }

}

void setUpDataStructures() {
    state_root = newState();
}

void cleanUpDataStructures() {
    recursiveCleanup(state_root);
}

