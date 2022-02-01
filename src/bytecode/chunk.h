#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "../util/common.h"
#include "value.h"

/*
 * Define all supported Operations below
 */
typedef enum {

    // 2 bytes: OP_CONST <1 byte for index>
    OP_CONST,

    // 4 bytes: OP_CONST <3 bytes for index, stored in little-endian order, least-significant byte first>
    OP_CONST_LONG,

    // 1 byte
    OP_NEGATE,

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

    // array for line number information
    int *lines;

    // array for constants values (a.k.a. constant pool in other VMs)
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

void writeLongConstant(Chunk *chunk, Value value, int line);

#endif // CLOX_CHUNK_H
