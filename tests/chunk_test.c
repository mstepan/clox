//
// Created by Maksym Stepanenko on 19.02.2022.
//

#include <stdio.h>
#include <check.h>
#include "../src/bytecode/chunk.h"
#include "../src/bytecode/value.h"


static void assert_value(Value expected, Value actual){
    ck_assert_int_eq(expected.type, actual.type);

    if(IS_NUMBER(expected) ){
        ck_assert_double_eq(AS_NUMBER(expected), AS_NUMBER(actual));
    }
    else if(IS_BOOL(expected) ){
        ck_assert(AS_BOOL(expected) == AS_BOOL(actual));
    }
}

/**
 * Create test cases.
 */
START_TEST(test_chunk) {
    Chunk chunk;
    initChunk(&chunk);

    writeChunk(&chunk, OP_RETURN, 1);

    ck_assert_int_eq(chunk.count, 1);
    ck_assert_int_eq(chunk.capacity, 8);

    const Value CONSTANT_VALUE = (Value){VAL_NUMBER, {.number = 133}};
    int index = addConstant(&chunk, CONSTANT_VALUE);
    writeChunk(&chunk, OP_CONST, 2);
    writeChunk(&chunk, index, 2);

    ck_assert_int_eq(chunk.count, 3);
    ck_assert_int_eq(chunk.capacity, 8);
    ck_assert_int_eq(chunk.constants.count, 1);
    ck_assert_int_eq(chunk.constants.capacity, 8);

    assert_value(CONSTANT_VALUE, chunk.constants.values[0]);



    freeChunk(&chunk);
}

END_TEST

/**
 * Add test cases to test suite.
 */
Suite *chunk_suite(void) {
    Suite *s = suite_create("Chunk Tests");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_chunk);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int no_failed = 0;
    Suite *s = chunk_suite();
    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? 0 : -1;
}