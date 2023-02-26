#include "s21_decimal.h"

/*
 --------------------------------
|                                |
|      Get/Set BIG helpers       |
|                                |
 --------------------------------
*/

int big_get_bit(big_decimal src, int index) {
  int res = 0;
  unsigned mask = 1u << (index % 32);
  res = src.bits[index / 32] & mask;
  return (res != 0);
}

void big_set_bit(big_decimal *value, int index, int set) {
  unsigned mask = 1u << (index % 32);
  if (set == 0) {
    value->bits[index / 32] = value->bits[index / 32] & ~mask;
  } else {
    value->bits[index / 32] = value->bits[index / 32] | mask;
  }
}

int big_get_scale(big_decimal src) {
  unsigned move = 255;
  unsigned mask = move << 16;
  int res = (src.bits[6] & mask) >> 16;
  return res;
}

void big_set_scale(big_decimal *value, int scale) {
  int sign = big_get_sign(*value);
  value->bits[6] = 0;
  unsigned mask = scale << 16;
  value->bits[6] = value->bits[6] | mask;
  if (sign) {
    big_set_sign(value, sign);
  }
}

int big_get_sign(big_decimal src) { return src.bits[6] >> 31; }

void big_set_sign(big_decimal *value, int sign) {
  unsigned mask = 1u << 31;
  if (sign == 0) {
    value->bits[6] = value->bits[6] & ~mask;
  } else {
    value->bits[6] = value->bits[6] | mask;
  }
}

/*
 --------------------------------
|                                |
|      General BIG helpers       |
|                                |
 --------------------------------
*/
void big_init_decimal(big_decimal *dst) {
  for (int i = 0; i < 7; i++) {
    dst->bits[i] = 0;
  }
}

void convert_to_big(s21_decimal value, big_decimal *result) {
  big_init_decimal(result);
  for (int i = 0; i < 3; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[6] = value.bits[3];
}

void convert_from_big(big_decimal value, s21_decimal *result) {
  init_decimal(result);
  for (int i = 0; i < 3; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[3] = value.bits[6];
}

void big_copy_decimal(big_decimal src, big_decimal *dst) {
  big_init_decimal(dst);
  for (int i = 0; i < 7; i++) {
    dst->bits[i] = src.bits[i];
  }
}

// 1 - value is zero, 0 - not zero
int big_check_if_zero(big_decimal value) {
  int res = 1;
  for (int i = 0; i < 6; i++) {
    if (value.bits[i] != 0) {
      res = 0;
      break;
    }
  }
  return res;
}

int big_negate(big_decimal value, big_decimal *result) {
  int res = 0;
  big_copy_decimal(value, result);
  big_set_sign(result, !big_get_sign(value));
  return res;
}

/*
 --------------------------------
|                                |
|        Compare helpers         |
|                                |
 --------------------------------
*/

// 0 - equal, 1 - first greater, 2 - second greater
int big_compare_bits(big_decimal value_1, big_decimal value_2) {
  int res = 0;
  for (int i = 191; i >= 0; i--) {
    int bit1 = big_get_bit(value_1, i);
    int bit2 = big_get_bit(value_2, i);
    if (bit1 > bit2) {
      res = 1;
      break;
    } else if (bit1 < bit2) {
      res = 2;
      break;
    }
  }
  return res;
}

/*
 --------------------------------
|                                |
|     Arithmetic BIG helpers     |
|                                |
 --------------------------------
*/

void big_inf_check(int *res, int index, int sum) {
  if (index == 191) {
    if (sum == 2 || sum == 3) {
      *res = 1;
    }
  }
}

void big_convert_complement(big_decimal *value) {
  for (int i = 0; i < 6; i++) {
    value->bits[i] = ~value->bits[i];
  }
  big_decimal one_digit = {{1, 0, 0, 0, 0, 0, 0}};
  simple_big_add(*value, one_digit, value);
}

void big_mul_by_10(big_decimal value, big_decimal *result) {
  big_init_decimal(result);
  int sign = big_get_sign(value);
  int scale = big_get_scale(value);
  big_decimal temp = {0};
  value.bits[6] = 0;
  big_copy_decimal(value, &temp);
  big_shift_left(&temp);
  for (int i = 0; i < 3; i++) {
    big_shift_left(&value);
  }
  simple_big_add(value, temp, result);
  big_set_sign(result, sign);
  big_set_scale(result, scale);
}

int squeeze_back_dec(big_decimal value, big_decimal *result, int flag) {
  int res = 0;
  big_init_decimal(result);
  if (flag == 1) {
    remove_trailing_zeroes(&value);
  }
  int sign = big_get_sign(value);
  int scale = big_get_scale(value);
  int mod_res = 0;
  while (check_if_fit(value) == 1 && scale > 0) {
    mod_res = big_div_by_10(&value);
    big_set_scale(&value, --scale);
    remove_trailing_zeroes(&value);
    scale = big_get_scale(value);
  }
  if (flag == 1) {
    while (scale > 28 && big_check_if_zero(value) == 0) {
      mod_res = big_div_by_10(&value);
      big_set_scale(&value, --scale);
      remove_trailing_zeroes(&value);
      scale = big_get_scale(value);
    }
  }
  bank_rounding(&value, mod_res);
  if (big_check_if_zero(value) == 1) {
    big_set_scale(&value, 0);
  } else {
    if (check_if_fit(value) == 1) {
      if (sign == 1) {
        res = 2;
      } else {
        res = 1;
      }
    }
  }

  big_copy_decimal(value, result);
  return res;
}

void bank_rounding(big_decimal *value, int mod_res) {
  big_decimal temp = *value;
  big_decimal one_digit = {{1, 0, 0, 0, 0, 0, 0}};
  int sign = big_get_sign(*value);
  int last_digit = big_div_by_10(&temp);
  if (mod_res >= 5) {
    if (mod_res == 5) {
      if (last_digit % 2 == 1) {
        if (sign == 0) {
          simple_big_add(*value, one_digit, value);
        } else {
          simple_big_sub(*value, one_digit, value);
        }
      }
    } else {
      if (sign == 0) {
        simple_big_add(*value, one_digit, value);
      } else {
        simple_big_sub(*value, one_digit, value);
      }
    }
  }
}

int check_if_fit(big_decimal value) {
  int res = 0;
  for (int i = 191; i > 95; i--) {
    int bit = big_get_bit(value, i);
    if (bit == 1) {
      res = 1;
      break;
    }
  }
  return res;
}

int remove_trailing_zeroes(big_decimal *value) {
  int res = 0;
  big_decimal temp = *value;
  int scale = big_get_scale(*value);
  if (scale > 0) {
    int mod_res = 0;
    while (scale > 0 && mod_res == 0) {
      mod_res = big_div_by_10(&temp);
      if (mod_res == 0) {
        big_copy_decimal(temp, value);
        scale--;
      }
    }
    big_set_scale(value, scale);
  }
  return res;
}

int normalize_big(big_decimal *value_1, big_decimal *value_2) {
  int res = 0;
  int scale1 = big_get_scale(*value_1);
  int scale2 = big_get_scale(*value_2);
  if (scale1 != scale2) {
    int max_scale = scale1;
    if (max_scale < scale2) {
      // max_scale = scale2;
      while (scale1 != scale2) {
        big_mul_by_10(*value_1, value_1);
        scale1++;
      }
      big_set_scale(value_1, scale1);
    } else {
      while (scale1 != scale2) {
        big_mul_by_10(*value_2, value_2);
        scale2++;
      }
      big_set_scale(value_2, scale2);
    }
  }
  return res;
}

// returns mod 10 and changes incoming value to result of div 10
int big_div_by_10(big_decimal *value) {
  big_decimal ten_val = {{10, 0, 0, 0, 0, 0, 0}};
  big_decimal reg_a = {0};
  if (big_check_if_zero(*value) == 0) {
    int res_sign = big_get_sign(*value);
    big_set_sign(value, 0);

    big_simple_div(value, ten_val, &reg_a);
    big_set_sign(value, res_sign);
    big_set_sign(&reg_a, res_sign);
  }
  return reg_a.bits[0];
}

void big_shift_left(big_decimal *value) {
  int carry_bit31 = big_get_bit(*value, 31);
  int carry_bit63 = big_get_bit(*value, 63);
  int carry_bit95 = big_get_bit(*value, 95);
  int carry_bit127 = big_get_bit(*value, 127);
  int carry_bit159 = big_get_bit(*value, 159);
  for (int i = 0; i < 6; i++) {
    value->bits[i] <<= 1;
  }
  big_set_bit(value, 32, carry_bit31);
  big_set_bit(value, 64, carry_bit63);
  big_set_bit(value, 96, carry_bit95);
  big_set_bit(value, 128, carry_bit127);
  big_set_bit(value, 160, carry_bit159);
}

void big_shift_betwen_val_left(big_decimal *value1, big_decimal *value2) {
  int carry_bit = big_get_bit(*value1, 191);
  big_shift_left(value1);
  big_shift_left(value2);
  big_set_bit(value2, 0, carry_bit);
}
