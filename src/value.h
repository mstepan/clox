#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *arrayPtr);

void writeValueArray(ValueArray *arrayPtr, Value value);

void freeValueArray(ValueArray *arrayPtr);

#endif //CLOX_VALUE_H
