#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "../util/common.h"

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

/**
 * Convert clox value to c-value
 */
//#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
Value BOOL_VAL(bool value);

//#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
Value NIL_VAL();

//#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
Value NUMBER_VAL(double value);


/**
 * Convert c-value to clox value.
 */
//#define AS_BOOL (value) ((value).as.boolean)
bool AS_BOOL(Value value);

//#define AS_NUMBER (value) ((value).as.number)
double AS_NUMBER(Value value);

/**
 * Check clox value type
 */
//#define IS_BOOL (value) ((value).type == VAL_BOOL)
bool IS_BOOL(Value value);

//#define IS_NIL (value) ((value).type == VAL_NIL)
bool IS_NIL(Value value);

//#define IS_NUMBER (value) ((value).type == VAL_NUMBER)
bool IS_NUMBER(Value value);

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *array);

void writeValueArray(ValueArray *array, Value value);

void freeValueArray(ValueArray *array);

#endif //CLOX_VALUE_H
