#include <stdio.h>
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char *argv[]) {
    initVM();

    Chunk chunk;

    printf("Chunk of bytecode successfully allocated\n");

    initChunk(&chunk);

    int constantOffset = addConstant(&chunk, 3.14);
    writeChunk(&chunk, OP_CONST, 1);
    writeChunk(&chunk, constantOffset, 1);

    writeChunk(&chunk, OP_RETURN, 2);

//    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    printf("CLox VM completed successfully\n");
    return 0;
}