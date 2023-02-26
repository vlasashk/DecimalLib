#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  s21_decimal one_digit = {{1, 0, 0, 0}};
  int sign = get_sign(value);
  int scale = get_scale(value);
  int last_digit = 0;
  for (int i = 0; i < scale; i++) {
    last_digit = div_by_10(&value);
  }
  set_scale(&value, 0);
  copy_decimal(value, result);
  if (last_digit >= 5) {
    if (sign == 1) {
      s21_sub(value, one_digit, result);
    } else {
      s21_add(value, one_digit, result);
    }
  }
  return res;
}
