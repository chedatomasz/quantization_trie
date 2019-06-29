#ifndef IPP_QUANT_SAFE_ALLOC_H
#define IPP_QUANT_SAFE_ALLOC_H

#include <stdlib.h>

//Safely performs a malloc operation, exiting if an error occurs.
extern void *safe_malloc(size_t size);

//Safely performs a realloc operation, exiting if an error occurs.
extern void *safe_realloc(void *to_realloc, size_t size);

#endif //IPP_QUANT_SAFE_ALLOC_H
