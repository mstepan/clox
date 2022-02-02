#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include "../scanner/scanner.h"

static char *substring(const char *baseStr, size_t length) {
    char *tokenValue = (char *) malloc(length + 1);

    for(int i =0; i < length; i++){
        tokenValue[i] = baseStr[i];
    }

    tokenValue[length] = '\0';

    return tokenValue;
}

void compile(const char *source) {
    initScanner(source);
    int line = -1;

    // scan and print token till we get EOF
    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%d | ", token.line);
            line = token.line;
        } else {
            printf(" |  ");
        }

        const char *tokenValue = substring(token.start, token.length);

        printf("type: %d, %s\n", token.type, tokenValue);

        free((void*)tokenValue);

        if (token.type == TOKEN_EOF) break;
    }
}