#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

static void repl();

static void runFile(const char *path);

static char *readFile(const char *path);


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

/**
 * Read the whole file content into dynamic char array.
 */
static char *readFile(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Can't open file: %s\n", path);
        exit(74);
    }

    // calculate file size by moving cursor till the end
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    // rewind cursor back
    rewind(file);

    char *buf = (char *) malloc(fileSize + 1);
    if (buf == NULL) {
        fprintf(stderr, "Can't allocate enough memory to read file: %s\n", path);
        exit(74);
    }

    size_t readBytes = fread(buf, sizeof(char), fileSize, file);
    if (readBytes < fileSize) {
        fprintf(stderr, "Can't fully read file: %s\n", path);
        exit(74);
    }

    buf[readBytes] = '\0';

    fclose(file);
    return buf;
}






