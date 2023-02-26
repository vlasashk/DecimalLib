#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  big_decimal temp_res = {0};
  int scale1 = get_scale(value_1);
  int scale2 = get_scale(value_2);
  int res_scale = scale1 + scale2;
  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);
  int res_sign = sign1 ^ sign2;
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  big_simple_mul(value_1, value_2, &temp_res);
  big_set_sign(&temp_res, res_sign);
  big_set_scale(&temp_res, res_scale);

  res = squeeze_back_dec(temp_res, &temp_res, 1);
  convert_from_big(temp_res, result);
  return res;
}

// only positive values possible inside the function
void big_simple_mul(s21_decimal value_1, s21_decimal value_2,
                    big_decimal *result) {
  int q_overflow = 0;
  int q_current = get_bit(value_2, 0);
  big_init_decimal(result);
  s21_decimal reg_a = {0};
  for (int i = 96; i > 0; i--) {
    if (q_current == 1) {
      q_overflow = simple_add(reg_a, value_1, &reg_a);
    }
    shift_betwen_val_right(&reg_a, &value_2);
    set_bit(&reg_a, 95, q_overflow);
    q_current = get_bit(value_2, 0);
  }
  for (int i = 0; i < 3; i++) {
    result->bits[i] = value_2.bits[i];
    result->bits[i + 3] = reg_a.bits[i];
  }
}
