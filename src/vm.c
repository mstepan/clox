#include "util/common.h"
#include <stdarg.h>
#include "vm.h"
#include "bytecode/value.h"
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

static void printStackTrace() {
    printf("\tstack:\t");
    for (Value *cur = vm.stack; cur < vm.stackTop; cur++) {
        printf("[ ");
        printValue(*cur);
        printf(" ]");
    }
    printf("<-- top \n");
}

static Value peek(int distance) {
    return vm.stackTop[-1 - distance];
}

static void runtimeError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}

static InterpretResult run() {
#define READ_BYTE()(*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(wrapValue, op) \
    do {              \
    if( !IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
        runtimeError("Operand must be numbers");      \
        return INTERPRET_RUNTIME_ERROR;  \
    }\
    double second = AS_NUMBER(pop()); \
    double first = AS_NUMBER(pop());  \
    push(wrapValue(first op second)); \
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
            case OP_TRUE: {
                push(BOOL_VAL(true));
                break;
            }
            case OP_FALSE: {
                push(BOOL_VAL(false));
                break;
            }
            case OP_NIL: {
                push(NIL_VAL());
                break;
            }

            case OP_ADD:
                BINARY_OP(NUMBER_VAL, +);
                break;
            case OP_SUBTRACT:
                BINARY_OP(NUMBER_VAL, -);
                break;
            case OP_MULTIPLY:
                BINARY_OP(NUMBER_VAL, *);
                break;
            case OP_DIVIDE:
                BINARY_OP(NUMBER_VAL, /);
                break;

            case OP_NEGATE: {
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Can't negate NON number value.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
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



