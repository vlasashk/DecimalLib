#include "test.h"

int run_tests(Suite *test_case) {
  int number_failed;
  SRunner *sr = srunner_create(test_case);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}

int main() {
  int number_failed = 0;
  Suite *suites_list[] = {test_is_equal_suite(),
                          test_is_not_equal_suite(),
                          test_is_less_suite(),
                          test_is_less_or_equal_suite(),
                          test_is_greater_or_equal_suite(),
                          test_is_greater_suite(),
                          test_add_suite(),
                          test_sub_suite(),
                          test_div_suite(),
                          test_mod_suite(),
                          test_mul_suite(),
                          test_round_suite(),
                          test_negate_suite(),
                          test_truncate_suite(),
                          test_floor_suite(),
                          test_dec_to_float_suite(),
                          test_dec_to_int_suite(),
                          test_float_to_dec_suite(),
                          test_int_to_dec_suite(),
                          NULL};
  for (Suite **current = suites_list; *current != NULL; current++) {
    printf("_______________________________________\n");
    number_failed += run_tests(*current);
  }

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
