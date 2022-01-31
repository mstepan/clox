#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

/*
 * Define all supported Operations below
 */
typedef enum {
    OP_CONST,
    OP_RETURN,
} OpCode;

/*
 * Chunk of executable bytecode.
 */
typedef struct {
    int count;
    int capacity;
    uint8_t *code;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte);

void freeChunk(Chunk *chunk);

#endif // CLOX_CHUNK_H
