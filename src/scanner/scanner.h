#ifndef CLOX_SCANNER_H
#define CLOX_SCANNER_H

typedef enum {

    // 1 char
    TOKEN_LEFT_PAREN, // (
    TOKEN_RIGHT_PAREN, // )
    TOKEN_LEFT_BRACE, // {
    TOKEN_RIGHT_BRACE, // }
    TOKEN_COMMA, // ,
    TOKEN_DOT, // .
    TOKEN_SEMICOLON, // ;
    TOKEN_MINUS, // -
    TOKEN_PLUS, // +
    TOKEN_STAR, // *
    TOKEN_SLASH, // /

    // 1 or 2 chars
    TOKEN_BANG, // !
    TOKEN_BANG_EQUAL, // !=
    TOKEN_EQUAL, // =
    TOKEN_EQUAL_EQUAL, // ==
    TOKEN_GREATER, // >
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_LESS, // <
    TOKEN_LESS_EQUAL, // <=

    // literals
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

    // reserved keywords
    TOKEN_AND, TOKEN_OR,
    TOKEN_CLASS, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_IF, TOKEN_ELSE,
    TOKEN_WHILE, TOKEN_FOR,
    TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_NIL,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_VAR, TOKEN_FUN,

    TOKEN_ERROR,
    TOKEN_EOF,

}TokenType;

typedef struct {
    TokenType type;

    // pointer to token start
    const char* start;

    // token length
    int length;

    // line number
    int line;
} Token;

void initScanner(const char *source);

Token scanToken();

#endif //CLOX_SCANNER_H
