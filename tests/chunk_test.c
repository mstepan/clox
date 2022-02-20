//
// Created by Maksym Stepanenko on 19.02.2022.
//

#include <check.h>
#include "../src/bytecode/chunk.h"

/**
 * Create test cases.
 */
START_TEST(test_chunk) {
    Chunk chunk;
    initChunk(&chunk);

    writeChunk(&chunk, OP_RETURN, 1);

    ck_assert_int_eq(chunk.count, 1);
    ck_assert_int_eq(chunk.capacity, 8);

    const int CONSTANT_VALUE = 133;
    int index = addConstant(&chunk, CONSTANT_VALUE);
    writeChunk(&chunk, OP_CONST, 2);
    writeChunk(&chunk, index, 2);

    ck_assert_int_eq(chunk.count, 3);
    ck_assert_int_eq(chunk.capacity, 8);
    ck_assert_int_eq(chunk.constants.count, 1);
    ck_assert_int_eq(chunk.constants.capacity, 8);
    ck_assert_int_eq(chunk.constants.values[0], CONSTANT_VALUE);

    freeChunk(&chunk);
}

END_TEST

/**
 * Add test cases to test suite.
 */
Suite *chunk_suite(void) {
    Suite *s = suite_create("Chunk");
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