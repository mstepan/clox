#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

/*
 * Define all supported Operations below
 */
typedef enum {

    // 2 bytes: OP_CONST <constant pool index>
    OP_CONST,

    // 1 byte: OP_RETURN
    OP_RETURN,
} OpCode;

/*
 * Chunk of executable bytecode.
 */
typedef struct {
    int count;
    int capacity;

    // array for op codes
    uint8_t *code;

    // array for line information
    int *lines;

    // array for constants values
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

#endif // CLOX_CHUNK_H
