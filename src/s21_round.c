#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int scale = get_scale(value);
  if (result && scale < 29) {
    init_decimal(result);
    s21_decimal one_digit = {{1, 0, 0, 0}};
    s21_decimal ten_val = {{1, 0, 0, 0}};
    s21_decimal reminder = {0};
    int sign = get_sign(value);
    int last_digit = 0;
    if (scale > 0) {
      for (int i = 0; i < scale - 1; i++) {
        mul_by_10(ten_val, &ten_val);
      }
      if (check_if_zero(value) == 0 && scale > 1) {
        int res_sign = get_sign(value);
        set_sign(&value, 0);
        simple_div(&value, ten_val, &reminder);
        set_sign(&value, res_sign);
      }
      last_digit = div_by_10(&value);
      set_scale(&value, 0);
    }
    copy_decimal(value, result);
    if (last_digit >= 5) {
      if (sign == 1) {
        s21_sub(value, one_digit, result);
      } else {
        s21_add(value, one_digit, result);
      }
    }
  } else {
    res = 1;
  }
  return res;
}
