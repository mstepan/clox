//
// Generic string utility library
//
#include "string_utils.h"
#include <stdlib.h>

char *substring(const char *baseStr, size_t length) {
    char *tokenValue = (char *) malloc(length + 1);

    for (int i = 0; i < length; i++) {
        tokenValue[i] = baseStr[i];
    }

    tokenValue[length] = '\0';

    return tokenValue;
}

bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           ch == '_';
}