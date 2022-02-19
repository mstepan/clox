//
// Generic string utility library
//

#ifndef CLOX_STRING_UTILS_H
#define CLOX_STRING_UTILS_H
#include "common.h"

char *substring(const char *baseStr, size_t length);

bool isDigit(char ch);

bool isAlpha(char ch);

#endif //CLOX_STRING_UTILS_H
