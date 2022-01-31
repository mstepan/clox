#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"

typedef struct {
    // store pointer to a chunk that will be executed
    Chunk *chunk;

    // IP current instruction pointer, sometimes called PC (program counter)
    uint8_t* ip;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();

void freeVM();

InterpretResult interpret(Chunk *chunk);

#endif //CLOX_VM_H
