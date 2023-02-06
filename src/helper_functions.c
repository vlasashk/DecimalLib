#include "s21_decimal.h"

int getBit(s21_decimal src, int index) {
  unsigned move = 1;
  int res = 0;
  if (index / 32 < 4) {
    unsigned mask = move << (index % 32);
    res = src.bits[index / 32] & mask;
  }
  return (res != 0);
}

void setBit(s21_decimal *value, int index, int set) {
  unsigned move = 1;
  if (index / 32 < 4) {
    unsigned mask = move << (index % 32);
    if (set == 0) {
      value->bits[index / 32] = value->bits[index / 32] & ~mask;
    } else {
      value->bits[index / 32] = value->bits[index / 32] | mask;
    }
  }
}

int getScale(s21_decimal src) {
  unsigned move = 255;
  unsigned mask = move << 16;
  int res = src.bits[3] & mask;
  return res;
}

//Нужно добавить проверку на корректный размер scale
void setScale(s21_decimal *value, int scale) {
  int sign = getSign(*value);
  value->bits[3] = 0;
  unsigned mask = scale << 16;
  value->bits[3] = value->bits[3] | mask;
  if (sign) {
    setSign(value, sign);
  }
}

int getSign(s21_decimal src) {
  unsigned move = 1;
  unsigned mask = move << 31;
  int res = src.bits[3] & mask;
  return (res != 0);
}

void setSign(s21_decimal *value, int sign) {
  unsigned move = 1;
  unsigned mask = move << 31;
  if (sign == 0) {
    value->bits[3] = value->bits[3] & ~mask;
  } else {
    value->bits[3] = value->bits[3] | mask;
  }
}
