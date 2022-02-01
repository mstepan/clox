#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

static void runFile(const char *string) {
    printf("Running script\n");
}

static void repl() {
    char line[1024];
    for(;;){
        printf("> ");

        if(!fgets(line, sizeof(line), stdin)){
            printf("\n");
            break;
        }
        interpret(line);
    }
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


