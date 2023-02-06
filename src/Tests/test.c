#include "test.h"
void showBinaryFull(s21_decimal value) {
  unsigned i;
  printf("SCALE\n");
  for (i = 1 << 31; i > 0; i = i / 2) {
    (value.bits[3] & i) ? printf("1") : printf("0");
  }
  printf("\nFULL NUMBER\n");
  for (i = 1 << 31; i > 0; i = i / 2) {
    (value.bits[2] & i) ? printf("1") : printf("0");
  }
  printf(" ");
  for (i = 1 << 31; i > 0; i = i / 2) {
    (value.bits[1] & i) ? printf("1") : printf("0");
  }
  printf(" ");
  for (i = 1 << 31; i > 0; i = i / 2) {
    (value.bits[0] & i) ? printf("1") : printf("0");
  }
  printf("\n");
}
void showBinarySingle(unsigned value) {
  unsigned i;
  printf("SINGLE NUMBER\n");
  for (i = 1 << 31; i > 0; i = i / 2) {
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
  /* UNCOMMENT TO RUN MAIN TESTS */

  // int number_failed = 0;
  // Suite *suites_list[] = {test_abs_suite(), NULL};
  // for (Suite **current = suites_list; *current != NULL; current++) {
  //   printf("_______________________________________\n");
  //   number_failed += run_tests(*current);
  // }

  // return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

  /* UNCOMMENT TO DEBUG PARTICULAR CASE */
  s21_decimal value_1 = {{3, 0, 0, 89489465}};
  s21_decimal value_2 = {{4, 0, 0, 1 << 31}};
  // setScale(&value_1, 127);
  showBinaryFull(value_1);
  // showBinarySingle((int)(-1));
  printf("%d | %d\n", getBit(value_1, 2), getBit(value_2, 2));
  return 0;
}