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

static void advance();

static void errorAtCurrent(const char *msg);

static void expression();

static void consume(TokenType type, const char *msg);

static void errorAt(Token *token, const char *msg);

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

// global parser instance
Parser parser;

// current compiling chunk global pointer
Chunk *compilingChunk;

static Chunk *currentChunk() {
    return compilingChunk;
}

/*
 * Write single byte to current compilation Chunk.
 */
static void emitByte(uint8_t byte){
    writeChunk(currentChunk(), byte, parser.previous.line);
}

/*
 * Write 2 bytes to current compilation Chunk.
 */
static void emitBytes(uint8_t byte1, uint8_t byte2){
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn(){
    emitByte(OP_RETURN);
}

static void endCompiler(){
    emitReturn();
}

bool compile(const char *source, Chunk *chunk) {
    initScanner(source);

    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");
    endCompiler();
    return !parser.hadError;
}

static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR)break;
        errorAtCurrent(parser.current.start);
    }
}

static void errorAtCurrent(const char *msg) {
    errorAt(&parser.current, msg);
}

static void error(const char *msg) {
    errorAt(&parser.previous, msg);
}

static void errorAt(Token *token, const char *errorMsg) {
    // do not report any error if we are in panic mode
    if (parser.panicMode)return;

    // set panic mode to true
    parser.panicMode = true;

    fprintf(stderr, "[line %d], Error", token->line);
    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // do nothing here
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", errorMsg);
    parser.hadError = true;
}

static void expression() {
    //TODO:

    while( parser.current.type != TOKEN_EOF ){
        advance();
    }

}

static void consume(TokenType type, const char *msg) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    errorAtCurrent(msg);
}