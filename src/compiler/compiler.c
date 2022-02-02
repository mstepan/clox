#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include "../scanner/scanner.h"

static char *substring(const char *baseStr, size_t length) {
    char *tokenValue = (char *) malloc(length + 1);

    for (int i = 0; i < length; i++) {
        tokenValue[i] = baseStr[i];
    }

    tokenValue[length] = '\0';

    return tokenValue;
}

void advance();

void expression();

void consume(TokenType type, const char *errorMsg);

bool compile(const char *source, Chunk *chunk) {
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");
    return true;
}

void advance(){}

void expression(){}

void consume(TokenType type, const char *errorMsg){}