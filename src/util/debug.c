#include <stdio.h>

#include "debug.h"

#include "../bytecode/value.h"

void printValue(Value value) {
    if (IS_NIL(value)) {
        printf("Nil");
    } else if (IS_BOOL(value)) {
        printf("%s", AS_BOOL(value) ? "true" : "false");
    } else if (IS_NUMBER(value)) {
        printf("%g", AS_NUMBER(value));
    } else {
        printf("Unknown type: %d", value.type);
    }
}

static int simpleInstruction(const char *opName, int offset) {
    printf("%s\n", opName);
    return offset + 1;
}

static int constantInstruction(const char *opName, Chunk *chunk, int offset) {

    uint8_t constIndex = chunk->code[offset + 1];

    printf("%-16s %4d '", opName, constIndex);
    printValue(chunk->constants.values[constIndex]);
    printf("'\n");

    return offset + 2;
}

static int constantLongInstruction(const char *opName, Chunk *chunk, int offset) {

    // OP_CONST_LONG store operand in little-endian order
    uint8_t loPart = chunk->code[offset + 1];
    uint8_t midPart = chunk->code[offset + 2];
    uint8_t hiPart = chunk->code[offset + 3];

    int constIndex = (hiPart << 16) | (midPart << 8) | loPart;

    printf("%-16s %4d '", opName, constIndex);
    printValue(chunk->constants.values[constIndex]);
    printf("'\n");

    return offset + 4;
}

int disassembleInstruction(Chunk *chunk, int offset) {

    printf("%04d ", offset);

    // handle line number information
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        // same line, so just show vertical line
        printf("    |");
    } else {
        // display new line number
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_CONST:
            return constantInstruction("OP_CONST", chunk, offset);
        case OP_CONST_LONG:
            return constantLongInstruction("OP_CONST_LONG", chunk, offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode detected: %d\n", instruction);
            return offset + 1;
    }
}

void disassembleChunk(Chunk *chunk, const char *name) {
    printf("============ %s ============\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}


