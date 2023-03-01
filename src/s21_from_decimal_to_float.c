#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int res = 0;
  if (dst != NULL) {
    int scale = 0, sign = 0;
    *dst = 0.0f;
    scale = get_scale(src);
    sign = get_sign(src);
    if (scale > 28) res = 1;
    if (!res) {
      for (int i = 0; i < 96; i++) {
        if (get_bit(src, i)) *dst += (float)pow(2, i);
      }
      *dst /= pow(10, scale);
      if (sign) *dst *= -1;
    }
  } else {
    res = 1;
  }
  return res;
}
