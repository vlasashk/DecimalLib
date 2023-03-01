#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 0;
  int sign = 0, scale = 0, exp = 0;
  if (dst != NULL) init_decimal(dst);
  if (src < 0) {
    src *= -1;
    sign = 1;
    set_sign(dst, sign);
  }
  if (dst != NULL && !isnan(src) && !isinf(src) &&
      !(fabsf(src) < s21_F_MIN && fabsf(src) > 0)) {
    exp = get_exp(src);
    s21_decimal ten;
    init_decimal(&ten);
    ten.bits[0] = 10;
    if (exp < 95 && exp > -95) {
      get_meanful(&src, &scale);
      if (scale <= 28) {
        set_decimal(dst, src);
        while (scale < 0) {
          s21_mul(*dst, ten, dst);
          scale++;
        }
        set_scale(dst, scale);
        set_sign(dst, sign);
      } else {
        res = 1;
      }
    } else {
      res = 1;
    }
  } else {
    res = 1;
  }
  return res;
}
