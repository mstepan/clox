//
// Created by Maksym Stepanenko on 19.02.2022.
//
#include <stdio.h>
#include <check.h>
#include "../src/scanner/scanner.h"

/**
 * Create test cases.
 */
START_TEST(test_scanner) {
    initScanner("2 + (3 * 6)");
    ck_assert_int_eq(scanToken().type, TOKEN_NUMBER);
    ck_assert_int_eq(scanToken().type, TOKEN_PLUS);
    ck_assert_int_eq(scanToken().type, TOKEN_LEFT_PAREN);
    ck_assert_int_eq(scanToken().type, TOKEN_NUMBER);
    ck_assert_int_eq(scanToken().type, TOKEN_STAR);
    ck_assert_int_eq(scanToken().type, TOKEN_NUMBER);
    ck_assert_int_eq(scanToken().type, TOKEN_RIGHT_PAREN);
    ck_assert_int_eq(scanToken().type, TOKEN_EOF);
}

END_TEST

/**
 * Add test cases to test suite.
 */
Suite *chunk_suite(void) {
    Suite *s = suite_create("Scanner Test");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_scanner);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = chunk_suite();
    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    int no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? 0 : -1;
}