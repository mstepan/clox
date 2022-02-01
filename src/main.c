#include <stdio.h>
#include "bytecode/chunk.h"
#include "util/debug.h"
#include "vm.h"

int main(int argc, const char *argv[]) {
    initVM();

    Chunk chunk;

    initChunk(&chunk);

    int constantOffset = addConstant(&chunk, 10);
    writeChunk(&chunk, OP_CONST, 10);
    writeChunk(&chunk, constantOffset, 10);

    constantOffset = addConstant(&chunk, 2);
    writeChunk(&chunk, OP_CONST, 11);
    writeChunk(&chunk, constantOffset, 11);

    writeChunk(&chunk, OP_SUBTRACT, 12);
    

    writeChunk(&chunk, OP_RETURN, 15);

//    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    return 0;
}