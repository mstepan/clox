#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

static char peekNext() {
    if (isAtEnd()) {
        return '\0';
    }
    return *(scanner.current + 1);
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
            case '/':
                if (peekNext() == '/') {
                    // skip everything till the end of line or EOF
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        // we support multi-line strings, so just handle new line properly
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd())return errorToken("Unterminated string detected");

    // consume enclosing '"' character
    advance();
    return makeToken(TOKEN_STRING);
}

static bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

static Token number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        // consume '.'
        advance();

        // consume all digits after dot
        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static bool isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           ch == '_';
}

/*
 * Check if we have identifier or some reserved word here.
 */
static TokenType identifierType() {


    //TODO:
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    // consume all alphabets or digits
    while (isAlpha(peek()) || isDigit(peek())) advance();

    return makeToken(identifierType());
}

Token scanToken() {
    skipWhitespaces();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();

    // handle identifiers and reserved keywords
    if (isAlpha(c))return identifier();

    // handle number
    if (isDigit(c)) return number();

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

            // handle string value enclosed in double quotes
        case '"':
            return string();
    }

    return errorToken("Unexpected character");
}

