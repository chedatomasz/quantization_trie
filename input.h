#ifndef IPP_QUANT_IO_H
#define IPP_QUANT_IO_H

#include <stdbool.h>
#include "instruction_interface.h"

// Checks if there is another line to be read in stdin.
extern bool nextLineAvailable();

// Reads and executes next instruction from stdin.
extern Instruction *getNextInstruction();

#endif //IPP_QUANT_IO_H
