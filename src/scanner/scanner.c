#include <stdio.h>
#include <string.h>
#include "../util/common.h"
#include "scanner.h"

typedef struct {

    // beginning of current lexeme
    const char *start;

    // current character
    const char *current;

    // current line number
    int line;
} Scanner;


// Global scanner instance
Scanner scanner;

void initScanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    printf("Scanner initialized successfully\n");
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char *errorMsg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = errorMsg;
    token.length = (int) strlen(errorMsg);
    token.line = scanner.line;

    return token;
}

static char advance() {
    char ch = *scanner.current;
    scanner.current++;
    return ch;
}

static bool match(char expectedCh) {
    if (isAtEnd()) {
        return false;
    }

    if (*scanner.current != expectedCh) {
        return false;
    }
    scanner.current++;
    return true;
}

static char peek() {
    return *scanner.current;
}

/*
 * Skip all whitespace character + skip single line comments.
 */
static void skipWhitespaces() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            default:
                return;
        }
    }
}

Token scanToken() {
    skipWhitespaces();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();

    switch (c) {
        // 1 char match
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '*':
            return makeToken(TOKEN_STAR);
        case '/':
            return makeToken(TOKEN_SLASH);

            // 1 or 2 chars match
        case '!':
            return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_EQUAL_EQUAL);
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    }

    return errorToken("Unexpected character");
}

