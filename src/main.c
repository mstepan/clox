#include <stdio.h>
#include "chunk.h"
#include "debug.h"

int main(int argc, const char *argv[]) {

    Chunk chunk;

    printf("Chunk of bytecode successfully allocated\n");

    initChunk(&chunk);

    writeChunk(&chunk, OP_CONST);
    writeChunk(&chunk, OP_RETURN);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    printf("CLox VM completed successfully\n");
    return 0;
}