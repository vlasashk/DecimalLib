#include "test.h"

void showBinaryFull(s21_decimal value) {
  unsigned i;
  printf("SCALE\n");
  for (i = 0x80000000; i > 0; i = i / 2) {
    (value.bits[3] & i) ? printf("1") : printf("0");
    if (i == 1 << 24 || i == 1 << 16) {
      printf("|");
    }
  }
  printf("\nFULL NUMBER\n");
  for (int j = 2; j >= 0; j--) {
    for (i = 0x80000000; i > 0; i = i / 2) {
      (value.bits[j] & i) ? printf("1") : printf("0");
    }
    printf(" ");
  }
  printf("\n");
}

void showBigBinaryFull(big_decimal value) {
  unsigned i;
  printf("SCALE\n");
  for (i = 0x80000000; i > 0; i = i / 2) {
    (value.bits[6] & i) ? printf("1") : printf("0");
    if (i == 1 << 24 || i == 1 << 16) {
      printf("|");
    }
  }
  printf("\nFULL NUMBER\n");
  for (int j = 5; j >= 0; j--) {
    for (i = 0x80000000; i > 0; i = i / 2) {
      (value.bits[j] & i) ? printf("1") : printf("0");
    }
    printf(" ");
  }
  printf("\n");
}
void showBinarySingle(unsigned value) {
  unsigned i;
  printf("SINGLE NUMBER\n");
  for (i = 0x80000000; i > 0; i = i / 2) {
    (value & i) ? printf("1") : printf("0");
  }
  printf("\n");
}

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
  // /* UNCOMMENT TO RUN MAIN TESTS */

  // int number_failed = 0;
  // Suite *suites_list[] = {test_is_equal_suite(),
  //                         test_is_not_equal_suite(),
  //                         test_is_less_suite(),
  //                         test_is_less_or_equal_suite(),
  //                         test_is_greater_or_equal_suite(),
  //                         test_is_greater_suite(),
  //                         test_add_suite(),
  //                         test_sub_suite(),
  //                         test_div_suite(),
  //                         test_mod_suite(),
  //                         test_mul_suite(),
  //                         test_round_suite(),
  //                         test_negate_suite(),
  //                         test_truncate_suite(),
  //                         test_floor_suite(),
  //                         NULL};
  // for (Suite **current = suites_list; *current != NULL; current++) {
  //   printf("_______________________________________\n");
  //   number_failed += run_tests(*current);
  // }

  // return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

  /* UNCOMMENT TO DEBUG PARTICULAR CASE */

  s21_decimal value_1 = {{2, 0, 0, 0x80000000 | 28 << 16}};
  s21_decimal value_2 = {{16, 0, 0, 28 << 16}};

  s21_decimal result;
  showBinaryFull(value_1);
  showBinaryFull(value_2);

  int res = s21_mul(value_1, value_2, &result);

  // int res = s21_div(value_3, value_4, &result);
  showBinaryFull(result);

  printf("%f | %f | %d\n", 12.3456 / 1.1, fmod(12.3456, 1.1), res);
  return 0;
}
