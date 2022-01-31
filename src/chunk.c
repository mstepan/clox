#include <stdio.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk) {
    chunk->code = NULL;
    chunk->capacity = 0;
    chunk->count = 0;
}

void writeChunk(Chunk *chunk, uint8_t byte) {
    if (chunk->capacity < chunk->count + 1) {
        // resize chunk array here
        const int oldCapacity = chunk->capacity;
        const int newCapacity = GROW_CAPACITY(oldCapacity);

        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, newCapacity);
        chunk->capacity = newCapacity;
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void freeChunk(Chunk *chunk) {
    chunk->code = FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    initChunk(chunk);
}
