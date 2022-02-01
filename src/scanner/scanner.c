#include <stdio.h>

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

