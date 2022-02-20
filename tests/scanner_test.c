//
// Created by Maksym Stepanenko on 19.02.2022.
//
#include <stdio.h>
#include <check.h>
#include "../src/scanner/scanner.h"

/**
 * Create test cases.
 */
START_TEST(test_scanner_simple_arithmetic) {
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

START_TEST(test_scanner_simple_programm) {
    initScanner("var x = 133;");
    ck_assert_int_eq(scanToken().type, TOKEN_VAR);
    ck_assert_int_eq(scanToken().type, TOKEN_IDENTIFIER);
    ck_assert_int_eq(scanToken().type, TOKEN_EQUAL);
    ck_assert_int_eq(scanToken().type, TOKEN_NUMBER);
    ck_assert_int_eq(scanToken().type, TOKEN_SEMICOLON);
    ck_assert_int_eq(scanToken().type, TOKEN_EOF);
}

END_TEST


/**
 * Add test cases to test suite.
 */
Suite *chunk_suite(void) {
    Suite *s = suite_create("Scanner Test");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_scanner_simple_arithmetic);
    tcase_add_test(tc_core, test_scanner_simple_programm);
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