#include <stdio.h>
#include "value.h"
#include "../util/memory.h"

void initValueArray(ValueArray *array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray *array, Value value) {
    if (array->capacity < array->count + 1) {
        const int oldCapacity = array->capacity;
        const int newCapacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value , array->values, oldCapacity, newCapacity);
        array->capacity = newCapacity;
    }

    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}