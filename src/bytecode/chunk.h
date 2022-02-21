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

    // true, false, nil
    OP_TRUE,
    OP_FALSE,
    OP_NIL,

    // logical equality, in-equality
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,

    // 1 byte
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,

    OP_NOT,
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

    /**
     * Array to store main opcodes.
     *
     * Example:
     * 10 + 20
     *
     * code => [OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_RETURN]
     * constants => [10, 20]
     */
    uint8_t *code;

    /**
     * Array for line numbers, stored separately from main code, b/c we only need line information for debug purpose.
     * So, better to do not mix with 'code' for better performance.
     */
     // TODO: taking into account that line array will contain lot's of repeated values,
     //  like: [1, 1, 1, 1, 2, 2, 2, 3, 3 ..] better to use run-length encoding here, like [1:4, 2:3, 3:2]
    int *lines;

    // array for constants values (a.k.a. constant pool in other VMs)
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

int getLineNumber(Chunk *chunk, int offset);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

/**
 * Write OP_CONST_LONG + 3 bytes offset in constant pool.
 * This instruction will be used for chunks that have more than 256 (2^8) constants in pool.
 * Can store up to 2 ^ 24 = 16777216 constant values.
 */
void writeLongConstant(Chunk *chunk, Value value, int line);

#endif // CLOX_CHUNK_H
