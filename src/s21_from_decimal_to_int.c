#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 0;
  if (dst != NULL) {
    int scale = 0;
    *dst = 0;
    scale = get_scale(src);
    if (scale > 28) res = 1;
    if (!res) {
      s21_truncate(src, &src);
      for (int i = 0; i < 96; i++) {
        if (get_bit(src, i)) *dst += pow(2, i);
      }
      if (get_sign(src)) *dst *= -1;
    }
  }
  return res;
}
