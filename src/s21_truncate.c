#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int scale = get_scale(value);
  if (result && scale < 29) {
    init_decimal(result);
    s21_decimal ten_val = {{1, 0, 0, 0}};
    s21_decimal reminder = {0};

    if (scale > 0) {
      for (int i = 0; i < scale; i++) {
        mul_by_10(ten_val, &ten_val);
      }
      if (check_if_zero(value) == 0) {
        int res_sign = get_sign(value);
        set_sign(&value, 0);
        simple_div(&value, ten_val, &reminder);
        set_sign(&value, res_sign);
      }
      set_scale(&value, 0);
    }
    set_scale(&value, 0);
    copy_decimal(value, result);
  } else {
    res = 1;
  }
  return res;
}
