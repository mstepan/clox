#include <stdio.h>
#include "chunk.h"
#include "debug.h"

int main(int argc, const char *argv[]) {

    Chunk chunk;

    printf("Chunk of bytecode successfully allocated\n");

    initChunk(&chunk);

    int constantOffset = addConstant(&chunk, 3.14);
    writeChunk(&chunk, OP_CONST, 1);
    writeChunk(&chunk, constantOffset, 1);

    writeChunk(&chunk, OP_RETURN, 1);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    printf("CLox VM completed successfully\n");
    return 0;
}