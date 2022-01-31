#include <stdio.h>
#include "vm.h"
#include "debug.h"


VM vm;

void initVM() {
}

void freeVM() {
}

static InterpretResult run() {
#define READ_BYTE()(*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
        int offset = vm.ip - vm.chunk->code;
        disassembleInstruction(vm.chunk, offset);
#endif

        uint8_t instruction;

        // main bytecode decoding loop
        switch (instruction = READ_BYTE()) {
            case OP_CONST: {
                Value constantValue = READ_CONSTANT();
                printValue(constantValue);
                printf("\n");
                break;
            }
            case OP_RETURN: {
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

