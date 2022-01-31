#include <stdlib.h>

#include "memory.h"

/*
 * if newSize == 0 we just free memory,
 * otherwise we allocate or resize memory chunk.
 *
 */
void *reallocate(void *arrayPtr, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(arrayPtr);
        return NULL;
    }

    void *res = realloc(arrayPtr, newSize);

    // reallocation failed, not enough memory for a process, so just fail
    if (res == NULL) {
        exit(1);
    }
    return res;
}
