#include <stdio.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        // resize chunk array here
        const int oldCapacity = chunk->capacity;
        const int newCapacity = GROW_CAPACITY(oldCapacity);

        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, newCapacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, newCapacity);

        chunk->capacity = newCapacity;
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;

    chunk->count++;
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

/*
 * Returns index for just added constant, so that we can find constant by index later.
 */
int addConstant(Chunk *chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}


void writeLongConstant(Chunk *chunk, Value value, int line) {
    if (chunk->capacity < chunk->count + 4) {
        // resize chunk array here
        const int oldCapacity = chunk->capacity;
        const int newCapacity = GROW_CAPACITY(oldCapacity);

        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, newCapacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, newCapacity);

        chunk->capacity = newCapacity;
    }

    int constantIndex = addConstant(chunk, value);

    uint8_t loPart = constantIndex & 0xFF;
    uint8_t midPart = (constantIndex >> 8) & 0xFF;
    uint8_t hiPart = (constantIndex >> 16) & 0xFF;

    chunk->code[chunk->count] = OP_CONST_LONG;

    // store long constant in a little-endian order, less significant byte first
    chunk->code[chunk->count + 1] = loPart;
    chunk->code[chunk->count + 2] = midPart;
    chunk->code[chunk->count + 3] = hiPart;

    chunk->lines[chunk->count] = line;

    chunk->count += 4;
}
