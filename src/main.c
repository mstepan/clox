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

    for(int i =0; i < 100; i++){
        int value = 100 * (i+1);
        int line = i+2;
        writeLongConstant(&chunk, value, line);
    }

    writeChunk(&chunk, OP_RETURN, 102);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    printf("CLox VM completed successfully\n");
    return 0;
}