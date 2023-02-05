#include "test.h"

// int run_tests(Suite *test_case) {
//   int number_failed;
//   SRunner *sr = srunner_create(test_case);
//   srunner_set_fork_status(sr, CK_NOFORK);
//   srunner_run_all(sr, CK_NORMAL);
//   number_failed = srunner_ntests_failed(sr);
//   srunner_free(sr);
//   return number_failed;
// }

int main() {
  /* UNCOMMENT TO RUN MAIN TESTS */

  // int number_failed = 0;
  // Suite *suites_list[] = {test_abs_suite(), NULL};
  // for (Suite **current = suites_list; *current != NULL; current++) {
  //   printf("_______________________________________\n");
  //   number_failed += run_tests(*current);
  // }

  // return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

  /* UNCOMMENT TO DEBUG OF PARTICULAR CASE */
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 20;
  value_2.bits[0] = 5;
  s21_decimal result = {0};
  printf("%d | %d\n", s21_add(value_1, value_2, &result),
         s21_is_less(value_1, value_2));
  return 0;
}