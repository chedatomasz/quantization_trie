#ifndef IPP_QUANT_OUTPUT_H
#define IPP_QUANT_OUTPUT_H

#include <stdint.h>

//Prints "ERROR" to stderr.
extern void printError();

//Prints "OK" to stdout.
extern void printOk();

//Prints "YES" to stdout.
extern void printYes();

//Prints "NO" to stdout.
extern void printNo();

//Prints the given uint64_t to stdout.
extern void printEnergy(uint64_t energy);

#endif //IPP_QUANT_OUTPUT_H
