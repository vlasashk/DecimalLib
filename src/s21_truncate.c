#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  int scale = get_scale(value);
  for (int i = 0; i < scale; i++) {
    div_by_10(&value);
  }
  set_scale(&value, 0);
  copy_decimal(value, result);
  return res;
}
