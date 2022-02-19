//
// Generic string utility library
//
#include "string_utils.h"

char *substring(const char *baseStr, size_t length) {
    char *tokenValue = (char *) malloc(length + 1);

    for (int i = 0; i < length; i++) {
        tokenValue[i] = baseStr[i];
    }

    tokenValue[length] = '\0';

    return tokenValue;
}