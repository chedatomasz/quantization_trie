#ifndef IPP_QUANT_ENGINE_H
#define IPP_QUANT_ENGINE_H

#include "instruction_interface.h"

extern void executeNextInstruction(Instruction *instruction);

extern void setUpDataStructures();

extern void cleanUpDataStructures();

#endif //IPP_QUANT_ENGINE_H
