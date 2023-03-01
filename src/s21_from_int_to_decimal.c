#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 0;
  if (dst != NULL) {
    init_decimal(dst);
    if (src < 0) {
      set_sign(dst, 1);
      src *= -1;
    }
    dst->bits[0] = src;
  } else {
    res = 1;
  }
  return res;
}
