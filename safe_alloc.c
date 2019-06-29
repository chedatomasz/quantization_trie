#include "safe_alloc.h"

//Safely performs a malloc operation, exiting if an error occurs.
void *safe_malloc(size_t size) {
    void *result = malloc(size);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

//Safely performs a realloc operation, exiting if an error occurs.
void *safe_realloc(void *to_realloc, size_t size) {
    void *result = realloc(to_realloc, size);
    if (result == NULL) {
        free(to_realloc);
        exit(1);
    }
    return result;
}
