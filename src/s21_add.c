#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  big_decimal temp1 = {0};
  big_decimal temp2 = {0};
  big_decimal temp_res = {0};
  convert_to_big(value_1, &temp1);
  convert_to_big(value_2, &temp2);

  normalize_big(&temp1, &temp2);

  simple_big_add(temp1, temp2, &temp_res);
  res = squeeze_back_dec(temp_res, &temp_res, 1);
  convert_from_big(temp_res, result);
  return res;
}

int simple_big_add(big_decimal value_1, big_decimal value_2,
                   big_decimal *result) {
  int res = 0;
  big_init_decimal(result);
  int sign1 = big_get_sign(value_1);
  int sign2 = big_get_sign(value_2);
  if (sign1 == sign2) {
    int carry = 0;
    big_set_sign(result, sign1);
    big_set_scale(result, big_get_scale(value_1));
    for (int i = 0; i < 192 && res == 0; i++) {
      int v1_bit = big_get_bit(value_1, i);
      int v2_bit = big_get_bit(value_2, i);
      int sum = v1_bit + v2_bit + carry;
      carry = 0;
      big_inf_check(&res, i, sum);
      if (sum == 0 || sum == 2) {
        big_set_bit(result, i, 0);
        if (sum == 2) {
          carry = 1;
        }
      } else {
        big_set_bit(result, i, 1);
        if (sum == 3) {
          carry = 1;
        }
      }
    }
  } else {
    if (sign1 == 0) {
      big_negate(value_2, &value_2);
      simple_big_sub(value_1, value_2, result);
    } else {
      big_negate(value_1, &value_1);
      simple_big_sub(value_2, value_1, result);
    }
  }
  return res;
}

int simple_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  int sign1 = get_sign(value_1);
  int carry = 0;
  set_sign(result, sign1);
  for (int i = 0; i < 96 && res == 0; i++) {
    int v1_bit = get_bit(value_1, i);
    int v2_bit = get_bit(value_2, i);
    int sum = v1_bit + v2_bit + carry;
    carry = 0;
    inf_check(&res, sign1, i, sum);
    if (sum == 0 || sum == 2) {
      set_bit(result, i, 0);
      if (sum == 2) {
        carry = 1;
      }
    } else {
      set_bit(result, i, 1);
      if (sum == 3) {
        carry = 1;
      }
    }
  }
  return res;
}
