#include <stdio.h>
#include "vm.h"
#include "util/debug.h"
#include "compiler/compiler.h"

// global VM instance
VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {
}

static void push(Value value) {
    (*vm.stackTop) = value;
    vm.stackTop++;
}

static Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

static void negateStackTop() {
    *(vm.stackTop - 1) = -(*(vm.stackTop - 1));
}

static void printStackTrace() {
    printf("\tstack:\t");
    for (Value *cur = vm.stack; cur < vm.stackTop; cur++) {
        printf("[ ");
        printValue(*cur);
        printf(" ]");
    }
    printf("<-- top \n");
}

static InterpretResult run() {
#define READ_BYTE()(*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do {              \
    double second = pop(); \
    double first = pop();  \
    push(first op second); \
    } while( false ) \

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

            case OP_ADD:
                BINARY_OP(+);
                break;
            case OP_SUBTRACT:
                BINARY_OP(-);
                break;
            case OP_MULTIPLY:
                BINARY_OP(*);
                break;
            case OP_DIVIDE:
                BINARY_OP(/);
                break;

            case OP_NEGATE: {
                // negate last value on top of stack
                // it's quicker than just pop(), negate and push()
                negateStackTop();
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

InterpretResult interpret(const char *source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

#ifdef DEBUG_TRACE_EXECUTION
    printf("============ VM runtime tracing START ============\n");
#endif
    InterpretResult result = run();

#ifdef DEBUG_TRACE_EXECUTION
    printf("============ VM runtime tracing END ============\n");
#endif

    freeChunk(&chunk);

    return result;
}



