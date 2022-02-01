#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "bytecode/chunk.h"
#include "bytecode/value.h"

#define STACK_MAX 256

typedef struct {
    // store pointer to a chunk that will be executed
    Chunk *chunk;

    // IP current instruction pointer, sometimes called PC (program counter)
    uint8_t *ip;

    // execution stack
    Value stack[STACK_MAX];
    Value *stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();

void freeVM();

InterpretResult interpret(const char* source);

void push(Value value);

Value pop();

#endif //CLOX_VM_H
