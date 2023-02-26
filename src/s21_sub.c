#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  big_decimal temp1 = {0};
  big_decimal temp2 = {0};
  big_decimal temp_res = {0};
  convert_to_big(value_1, &temp1);
  convert_to_big(value_2, &temp2);
  normalize_big(&temp1, &temp2);
  simple_big_sub(temp1, temp2, &temp_res);
  res = squeeze_back_dec(temp_res, &temp_res, 1);
  convert_from_big(temp_res, result);
  return res;
}

int simple_big_sub(big_decimal value_1, big_decimal value_2,
                   big_decimal *result) {
  int res = 0;
  big_init_decimal(result);
  int sign1 = big_get_sign(value_1);
  int sign2 = big_get_sign(value_2);
  if (big_check_if_zero(value_2) == 1) {
    big_set_sign(&value_2, 0);
    big_copy_decimal(value_1, result);
  } else if (big_check_if_zero(value_1) == 1) {
    big_set_sign(&value_1, 0);
    big_negate(value_2, &value_2);
    big_copy_decimal(value_2, result);
  } else if (sign1 == sign2) {
    big_decimal tmp1 = value_1;
    big_decimal tmp2 = value_2;
    if (sign1 == 1) {
      tmp1 = value_2;
      tmp2 = value_1;
    }
    big_set_sign(&tmp1, 0);
    big_set_sign(&tmp2, 0);
    big_convert_complement(&tmp2);
    if (simple_big_add(tmp1, tmp2, result) == 0 &&
        big_check_if_zero(value_2) == 0 && big_check_if_zero(value_1) == 0) {
      big_convert_complement(result);
      big_set_sign(result, 1);
    }
    big_set_scale(result, big_get_scale(tmp1));
  } else {
    big_set_sign(&value_2, !sign2);
    res = simple_big_add(value_1, value_2, result);
  }
  return res;
}
