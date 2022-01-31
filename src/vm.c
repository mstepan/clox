#include "vm.h"

VM vm;

void initVM(){
}

void freeVM(){
}
static InterpretResult run(){
    return INTERPRET_OK;
}

InterpretResult interpret(Chunk *chunk){

    vm.chunk = chunk;
    vm.ip = vm.chunk->code;

    return run();
}

