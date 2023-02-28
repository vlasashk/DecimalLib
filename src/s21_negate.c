#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (result) {
    copy_decimal(value, result);
    set_sign(result, !get_sign(value));
  } else {
    res = 1;
  }
  return res;
}
