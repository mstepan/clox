#include <stdio.h>

#include "debug.h"

void disassembleChunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(Chunk *chunk, int offset) {

    printf("%04d ", offset);

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_CONST:
            return constantInstruction("OP_CONST", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode detected: %d\n", instruction);
            return offset + 1;
    }
}


int simpleInstruction(const char *opName, int offset) {
    printf("%s\n", opName);
    return offset + 1;
}

int constantInstruction(const char *opName, Chunk *chunk, int offset) {

    uint8_t constIndex = chunk->code[offset + 1];

    printf("%-16s %4d '", opName, constIndex);
    printValue(chunk->constants.values[constIndex]);
    printf("'\n");

    return offset + 2;
}

void printValue(Value value) {
    printf("%g", value);
}