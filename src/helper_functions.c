#include "s21_decimal.h"

/*
 --------------------------------
|                                |
|        Get/Set helpers         |
|                                |
 --------------------------------
*/

int get_bit(s21_decimal src, int index) {
  int res = 0;
  unsigned move = 1;
  unsigned mask = move << (index % 32);
  res = src.bits[index / 32] & mask;
  return (res != 0);
}

void set_bit(s21_decimal *value, int index, int set) {
  unsigned move = 1;
  unsigned mask = move << (index % 32);
  if (set == 0) {
    value->bits[index / 32] = value->bits[index / 32] & ~mask;
  } else {
    value->bits[index / 32] = value->bits[index / 32] | mask;
  }
}

int get_scale(s21_decimal src) {
  unsigned move = 255;
  unsigned mask = move << 16;
  int res = (src.bits[3] & mask) >> 16;
  return res;
}

void set_scale(s21_decimal *value, int scale) {
  int sign = get_sign(*value);
  value->bits[3] = 0;
  unsigned mask = scale << 16;
  value->bits[3] = value->bits[3] | mask;
  if (sign) {
    set_sign(value, sign);
  }
}

int get_sign(s21_decimal src) { return src.bits[3] >> 31; }

void set_sign(s21_decimal *value, int sign) {
  unsigned move = 1;
  unsigned mask = move << 31;
  if (sign == 0) {
    value->bits[3] = value->bits[3] & ~mask;
  } else {
    value->bits[3] = value->bits[3] | mask;
  }
}

/*
 --------------------------------
|                                |
|        General helpers         |
|                                |
 --------------------------------
*/
void init_decimal(s21_decimal *dst) {
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = 0;
  }
}

void copy_decimal(s21_decimal src, s21_decimal *dst) {
  init_decimal(dst);
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = src.bits[i];
  }
}

// 1 - value is zero, 0 - not zero
int check_if_zero(s21_decimal value) {
  int res = 0;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
    res = 1;
  }
  return res;
}

/*
 --------------------------------
|                                |
|       Arithmetic helpers       |
|                                |
 --------------------------------
*/
void inf_check(int *res, int sign, int index, int sum) {
  if (index == 95) {
    if (sum == 2 || sum == 3) {
      if (sign == 1) {
        *res = 2;
      } else {
        *res = 1;
      }
    }
  }
}

void shift_left(s21_decimal *value) {
  int carry_bit31 = get_bit(*value, 31);
  int carry_bit63 = get_bit(*value, 63);
  for (int i = 0; i < 3; i++) {
    value->bits[i] <<= 1;
  }
  set_bit(value, 32, carry_bit31);
  set_bit(value, 64, carry_bit63);
}

void shift_right(s21_decimal *value) {
  int carry_bit32 = get_bit(*value, 32);
  int carry_bit64 = get_bit(*value, 64);
  for (int i = 0; i < 3; i++) {
    value->bits[i] >>= 1;
  }
  set_bit(value, 31, carry_bit32);
  set_bit(value, 63, carry_bit64);
}

void shift_betwen_val_left(s21_decimal *value1, s21_decimal *value2) {
  int carry_bit = get_bit(*value1, 95);
  shift_left(value1);
  shift_left(value2);
  set_bit(value2, 0, carry_bit);
}

void shift_betwen_val_right(s21_decimal *value1, s21_decimal *value2) {
  int carry_bit = get_bit(*value1, 0);
  shift_right(value1);
  shift_right(value2);
  set_bit(value2, 95, carry_bit);
}

void simple_div(s21_decimal *value_1, s21_decimal value_2, s21_decimal *reg_a) {
  for (int i = 96; i > 0; i--) {
    shift_betwen_val_left(value_1, reg_a);
    if (s21_is_less(*reg_a, value_2)) {
      set_bit(value_1, 0, 0);
    } else {
      set_bit(value_1, 0, 1);
      s21_sub(*reg_a, value_2, reg_a);
    }
  }
}

// returns mod 10 and changes incoming value to result of div 10
int div_by_10(s21_decimal *value) {
  s21_decimal ten_val = {{10, 0, 0, 0}};
  s21_decimal reg_a = {0};
  if (check_if_zero(*value) == 0) {
    int res_sign = get_sign(*value);
    set_sign(value, 0);

    simple_div(value, ten_val, &reg_a);
    set_sign(value, res_sign);
    set_sign(&reg_a, res_sign);
  }
  return reg_a.bits[0];
}
