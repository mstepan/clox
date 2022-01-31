#include <stdio.h>
#include "vm.h"
#include "util/debug.h"


VM vm;


static void resetStack(){
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
    printf("VM initialized and ready to work\n");
}

void freeVM() {
    printf("VM destroyed\n");
}

static void printStackTrace(){
    printf("\tstack:\t");
    for(Value* cur = vm.stack; cur < vm.stackTop; cur++){
        printf("[ ");
        printValue(*cur);
        printf(" ]");
    }
    printf("<-- top \n");
}

static InterpretResult run() {
#define READ_BYTE()(*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
        printStackTrace();
        int offset = vm.ip - vm.chunk->code;
        disassembleInstruction(vm.chunk, offset);
#endif

        uint8_t instruction;

        // main bytecode decoding loop
        switch (instruction = READ_BYTE()) {
            case OP_CONST: {
                Value constantValue = READ_CONSTANT();
                push(constantValue);
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }

    }

#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult interpret(Chunk *chunk) {

    vm.chunk = chunk;
    vm.ip = vm.chunk->code;

    return run();
}

void push(Value value){
    (*vm.stackTop) = value;
    vm.stackTop++;
}

Value pop(){
    vm.stackTop--;
    return *vm.stackTop;
}

