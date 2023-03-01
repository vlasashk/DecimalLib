#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define s21_F_MIN 1E-28

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} big_decimal;

// Arithmetic
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparison
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// Convertors
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Another
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Helpers get/set
int get_bit(s21_decimal dst, int index);
int get_scale(s21_decimal src);
int get_sign(s21_decimal src);
void set_bit(s21_decimal *value, int index, int set);
void set_scale(s21_decimal *value, int scale);
void set_sign(s21_decimal *value, int sign);
void get_meanful(float *src, int *scale);
void set_decimal(s21_decimal *dst, float src);
int get_exp(float src);

// General helpers
void init_decimal(s21_decimal *dst);
void copy_decimal(s21_decimal src, s21_decimal *dst);
int check_if_zero(s21_decimal value);

// Compare helpers
int big_compare_bits(big_decimal value_1, big_decimal value_2);

// Arithmetic helpers
void inf_check(int *res, int sign, int index, int sum);
void shift_left(s21_decimal *value);
void shift_right(s21_decimal *value);
void shift_betwen_val_left(s21_decimal *value1, s21_decimal *value2);
void shift_betwen_val_right(s21_decimal *value1, s21_decimal *value2);
void simple_div(s21_decimal *value_1, s21_decimal value_2, s21_decimal *reg_a);
int div_by_10(s21_decimal *value);
int simple_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void mul_by_10(s21_decimal value, s21_decimal *result);
/*
 --------------------------------
|         BIG FUNCTIONS          |
 --------------------------------
*/
// Helpers get/set
int big_get_bit(big_decimal src, int index);
int big_get_scale(big_decimal src);
int big_get_sign(big_decimal src);
void big_set_bit(big_decimal *value, int index, int set);
void big_set_scale(big_decimal *value, int scale);
void big_set_sign(big_decimal *value, int sign);

// General helpers
void big_init_decimal(big_decimal *dst);
void convert_to_big(s21_decimal value, big_decimal *result);
void convert_from_big(big_decimal value, s21_decimal *result);
int normalize_big(big_decimal *value_1, big_decimal *value_2);
int big_check_if_zero(big_decimal value);
int big_negate(big_decimal value, big_decimal *result);

// Arithmetic helpers

void big_copy_decimal(big_decimal src, big_decimal *dst);
void big_mul_by_10(big_decimal value, big_decimal *result);
int big_div_by_10(big_decimal *value);
int simple_big_add(big_decimal value_1, big_decimal value_2,
                   big_decimal *result);
void big_convert_complement(big_decimal *value);
void big_inf_check(int *res, int index, int sum);
int simple_big_sub(big_decimal value_1, big_decimal value_2,
                   big_decimal *result);

void big_simple_div(big_decimal *value_1, big_decimal value_2,
                    big_decimal *reg_a);
void big_shift_left(big_decimal *value);
void big_shift_betwen_val_left(big_decimal *value1, big_decimal *value2);
int squeeze_back_dec(big_decimal value, big_decimal *result, int flag);
int remove_trailing_zeroes(big_decimal *value);
int check_if_fit(big_decimal value);
void bank_rounding(big_decimal *value, int mod_res);
void big_simple_mul(s21_decimal value_1, s21_decimal value_2,
                    big_decimal *result);

#endif  // SRC_S21_DECIMAL_H_
