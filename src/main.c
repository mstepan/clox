#include <stdio.h>
#include "bytecode/chunk.h"
#include "util/debug.h"
#include "vm.h"

int main(int argc, const char *argv[]) {
    initVM();

    Chunk chunk;

    initChunk(&chunk);

    int constantOffset = addConstant(&chunk, 3.14);
    writeChunk(&chunk, OP_CONST, 1);
    writeChunk(&chunk, constantOffset, 1);

    constantOffset = addConstant(&chunk, 177);
    writeChunk(&chunk, OP_CONST, 2);
    writeChunk(&chunk, constantOffset, 2);

    writeChunk(&chunk, OP_RETURN, 3);

//    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    return 0;
}