#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  init_decimal(result);
  int res_sign = get_sign(value_1) ^ get_sign(value_2);
  if (check_if_zero(value_2) == 1) {
    res = 3;
  } else if (check_if_zero(value_1) == 0) {
    big_decimal temp1 = {0};
    big_decimal temp2 = {0};
    big_decimal reg_a = {0};

    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    convert_to_big(value_1, &temp1);
    convert_to_big(value_2, &temp2);

    normalize_big(&temp1, &temp2);
    big_simple_div(&temp1, temp2, &reg_a);

    big_set_scale(&temp1, 0);
    if (check_if_fit(temp1) == 0 && big_check_if_zero(reg_a) == 0) {
      squeeze_back_dec(reg_a, &reg_a, 1);
      big_decimal temp_div = {0};
      big_copy_decimal(reg_a, &temp_div);
      while (big_get_scale(temp1) < 29 && big_check_if_zero(temp_div) == 0) {
        normalize_big(&temp_div, &temp2);
        big_decimal temp_mod = {0};
        while (big_compare_bits(temp_div, temp2) == 2 &&
               big_get_scale(temp1) < 29) {
          big_mul_by_10(temp_div, &temp_div);
          big_mul_by_10(temp1, &temp1);
          big_set_scale(&temp_div, big_get_scale(temp_div) + 1);
          big_set_scale(&temp1, big_get_scale(temp1) + 1);
        }
        big_simple_div(&temp_div, temp2, &temp_mod);
        simple_big_add(temp1, temp_div, &temp1);
        big_copy_decimal(temp_mod, &temp_div);
      }
    }
    big_set_sign(&temp1, res_sign);
    res = squeeze_back_dec(temp1, &temp1, 1);
    convert_from_big(temp1, result);
  } else {
    set_sign(result, res_sign);
  }
  return res;
}

void big_simple_div(big_decimal *value_1, big_decimal value_2,
                    big_decimal *reg_a) {
  for (int i = 192; i > 0; i--) {
    big_shift_betwen_val_left(value_1, reg_a);
    simple_big_sub(*reg_a, value_2, reg_a);
    if (big_get_sign(*reg_a) == 1) {
      big_set_bit(value_1, 0, 0);
      simple_big_add(*reg_a, value_2, reg_a);
    } else {
      big_set_bit(value_1, 0, 1);
    }
  }
}
