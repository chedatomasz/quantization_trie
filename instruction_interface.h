#ifndef IPP_QUANT_INSTRUCTION_INTERFACE_H
#define IPP_QUANT_INSTRUCTION_INTERFACE_H

#include <stdint.h>

// Validity values:
#define EMPTY -1
#define INCORRECT 0
#define CORRECT 1

// Signal values:
#define NOTB 0 //None of the below:
#define DECLARE 1
#define REMOVE 2
#define VALID 3
#define ENERGY2 4
#define ENERGY1 5
#define ENERGY_UNDEFINED 6
#define EQUAL 7

struct _Instruction {
    int validity;
    int signal;
    int *arg1;
    uint64_t arg1_length;
    uint64_t arg2_length;
    int *arg2;
};
typedef struct _Instruction Instruction;

#endif //IPP_QUANT_INSTRUCTION_INTERFACE_H
