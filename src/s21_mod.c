#include "s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  int res_sign = get_sign(value_1);
  if (check_if_zero(value_2) == 1) {
    res = 3;
  } else if (check_if_zero(value_1) == 0) {
    big_decimal temp1 = {0};
    big_decimal temp2 = {0};
    big_decimal reg_a = {0};
    int inf_sign = res_sign ^ get_sign(value_2);
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);

    convert_to_big(value_1, &temp1);
    convert_to_big(value_2, &temp2);
    normalize_big(&temp1, &temp2);
    big_simple_div(&temp1, temp2, &reg_a);
    if (check_if_fit(temp1) == 0) {
      res = squeeze_back_dec(reg_a, &reg_a, 1);
      convert_from_big(reg_a, result);
      set_sign(result, res_sign);
      if (check_if_zero(*result) == 0) {
        set_scale(result, big_get_scale(reg_a));
      }
    } else {
      init_decimal(result);
      if (inf_sign) {
        res = 2;
      } else {
        res = 1;
      }
    }
  } else {
    set_sign(result, res_sign);
  }
  return res;
}
