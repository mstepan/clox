
#include "util/common.h"
#include "util/file.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static void runFile(const char *path) {
    char *source = readFile(path);
    InterpretResult result = interpret(source);

    if (result == INTERPRET_COMPILE_ERROR)exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)exit(70);
}

int main(int argc, const char *argv[]) {
    initVM();

    if (argc == 1) {
        // running in REPL mode
        repl();
    } else if (argc == 2) {
        // executing some script
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path-to-script]\n");
        exit(64);
    }

    freeVM();

    return 0;
}












