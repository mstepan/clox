#include <stdio.h>
#include "value.h"
#include "memory.h"

void initValueArray(ValueArray *arrayPtr) {
    arrayPtr->count = 0;
    arrayPtr->capacity = 0;
    arrayPtr->values = NULL;
}

void writeValueArray(ValueArray *arrayPtr, Value value) {
    if (arrayPtr->capacity < arrayPtr->count + 1) {
        const int oldCapacity = arrayPtr->capacity;
        const int newCapacity = GROW_CAPACITY(oldCapacity);
        arrayPtr->values = GROW_ARRAY(Value , arrayPtr->values, oldCapacity, newCapacity);
    }

    arrayPtr->values[arrayPtr->count] = value;
    arrayPtr->count++;
}

void freeValueArray(ValueArray *arrayPtr) {
    FREE_ARRAY(Value, arrayPtr->values, arrayPtr->capacity);
    initValueArray(arrayPtr);
}