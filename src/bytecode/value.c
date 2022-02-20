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

Value BOOL_VAL(bool value) {
    return (Value){VAL_BOOL, {.boolean = value}};
}

//#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
Value NIL_VAL() {
    return (Value) {VAL_NIL, {.number = 0}};
}

//#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
Value NUMBER_VAL(double value) {
    return (Value) {VAL_NUMBER, {.number = value}};
}

void freeValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

bool AS_BOOL(Value value){
    return value.as.boolean;
}

double AS_NUMBER(Value value) {
    return value.as.number;
}

bool IS_BOOL(Value value){
    return value.type == VAL_BOOL;
}

bool IS_NIL(Value value){
    return value.type == VAL_NIL;
}

bool IS_NUMBER(Value value){
    return value.type == VAL_NUMBER;
}