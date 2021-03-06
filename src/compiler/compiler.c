#include "compiler.h"
#include "../scanner/scanner.h"
#include "../util/debug.h"

// Forward declaration
static void advance();

static void errorAtCurrent(const char *msg);

static void expression();

static void consume(TokenType type, const char *msg);

static void errorAt(Token *token, const char *msg);

static void binary();

static void grouping();

static void unary();

static void number();

static void literal();

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

/**
 * Write single byte to current compilation Chunk.
 */
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

/**
 * Write 2 bytes to current compilation Chunk.
 */
static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn() {
    emitByte(OP_RETURN);

#ifdef DEBUG_COMPILER
    if (parser.hadError) {
        fprintf(stderr, "Compilation error detected");
    } else {
        disassembleChunk(currentChunk(), "VM Chunk");
    }
#endif
}

static void endCompiler() {
    emitReturn();
}

/**
 * Use Vaughan Pratt's top-down (recursive) operator precedence parsing algorithm.
*/
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

static void consume(TokenType type, const char *msg) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    errorAtCurrent(msg);
}

// ========== ERROR ==========

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

// ========== PRECEDENCE ==========
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR, // or
    PREC_AND, // and
    PREC_EQUALITY, // ==, !=
    PREC_COMPARISON, // <, <=, >, >=
    PREC_TERM, // +, -
    PREC_FACTOR, // *, /
    PREC_UNARY, // -, !
    PREC_CALL, // ., ()
    PREC_PRIMARY
} Precedence;


typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

ParseRule rules[] = {

        /**
         * Use the following format for table-driver parser:
         * [scanner TokenType] =>  {<prefix function>, <infix function>, <precedence level>}
         */
        [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
        [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},

        [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},

        [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
        [TOKEN_DOT] = {NULL, NULL, PREC_NONE},

        [TOKEN_MINUS] = {unary, binary, PREC_TERM},
        [TOKEN_PLUS] = {NULL, binary, PREC_TERM},

        [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},

        [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
        [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},

        [TOKEN_BANG] = {unary, NULL, PREC_NONE},
        [TOKEN_BANG_EQUAL] = {NULL, binary, PREC_EQUALITY},
        [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_EQUAL_EQUAL] = {NULL, binary, PREC_EQUALITY},
        [TOKEN_GREATER] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_GREATER_EQUAL] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_LESS] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_LESS_EQUAL] = {NULL, binary, PREC_COMPARISON},

        [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
        [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
        [TOKEN_NUMBER] = {number, NULL, PREC_NONE},

        [TOKEN_AND] = {NULL, NULL, PREC_NONE},
        [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
        [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
        [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
        [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
        [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
        [TOKEN_IF] = {NULL, NULL, PREC_NONE},
        [TOKEN_NIL] = {literal, NULL, PREC_NONE},
        [TOKEN_OR] = {NULL, NULL, PREC_NONE},
        [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
        [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
        [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
        [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
        [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
        [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
        [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},

        [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
        [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

static ParseRule *getRule(TokenType type) {
    return &rules[type];
}

static void parsePrecedence(Precedence curPrecedenceLevel) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;

    if (prefixRule == NULL) {
        error("Expected prefix expression.");
        return;
    }
    // execute prefix rule function
    prefixRule();

    while (curPrecedenceLevel <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;

        // execute infix rule function
        infixRule();
    }
}

// ========== EXPRESSION ==========

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

// ========== NUMBER ==========

static uint8_t makeConstant(Value value) {
    int index = addConstant(currentChunk(), value);
    if (index > UINT8_MAX) {
        error("Constant index greater than 255. Too many constants.");
        return 0;
    }
    return (uint8_t) index;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONST, makeConstant(value));
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

// ========== GROUPING ==========

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after grouping.");
}

// ========== UNARY ==========

static void unary() {
    TokenType type = parser.previous.type;

    // parse & compile operand
    parsePrecedence(PREC_UNARY);

    switch (type) {
        case TOKEN_BANG:
            emitByte(OP_NOT);
            break;
        case TOKEN_MINUS:
            emitByte(OP_NEGATE);
            break;
        default:
            // should never reach here
            return;
    }
}

// ========== BINARY ==========

static void binary() {
    // store operator (+, -, *, /) to emit opcode
    TokenType operatorType = parser.previous.type;

    ParseRule *rule = getRule(operatorType);

    // parse tokens with higher precedence level if any
    parsePrecedence((Precedence) (rule->precedence + 1));

    // emit opcode
    switch (operatorType) {

        //  a != b is equivalent to !(a == b)
        case TOKEN_BANG_EQUAL:
            emitBytes(OP_EQUAL, OP_NOT);
            break;
        case TOKEN_EQUAL_EQUAL:
            emitByte(OP_EQUAL);
            break;
        case TOKEN_LESS:
            emitByte(OP_LESS);
            break;
        //  a <= b is equivalent to !(a > b)
        case TOKEN_LESS_EQUAL:
            emitBytes(OP_GREATER, OP_NOT);
            break;
        case TOKEN_GREATER:
            emitByte(OP_GREATER);
            break;
        //  a >= b is equivalent to !(a < b)
        case TOKEN_GREATER_EQUAL:
            emitBytes(OP_LESS, OP_NOT);
            break;

        case TOKEN_PLUS:
            emitByte(OP_ADD);
            break;
        case TOKEN_MINUS:
            emitByte(OP_SUBTRACT);
            break;
        case TOKEN_STAR:
            emitByte(OP_MULTIPLY);
            break;
        case TOKEN_SLASH:
            emitByte(OP_DIVIDE);
            break;
        default:
            return;
    }
}

// ========== LITERAL ==========

static void literal() {
    switch (parser.previous.type) {
        case TOKEN_TRUE:
            emitByte(OP_TRUE);
            break;
        case TOKEN_FALSE:
            emitByte(OP_FALSE);
            break;
        case TOKEN_NIL:
            emitByte(OP_NIL);
            break;
        default:
            // we should never reach here
            return;
    }
}





