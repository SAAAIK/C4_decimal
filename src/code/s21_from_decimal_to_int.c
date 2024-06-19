#include "common.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_decimal min = {{0x80000000, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal max = {{0x7FFFFFFF, 0x00000000, 0x00000000, 0x00000000}};
  if (!dst) return CONVERSION_ERROR;
  *dst = s21_truncate(src, &src);
  if (src.bits[1] == 0 && src.bits[2] == 0 && compare(src, min) >= 0 &&
      compare(src, max) <= 0) {
    int i = 0;
    while (i < MANTISSA_LEN) {
      if (get_bit(src, i) != 0) {
        *dst += pow(2, i);
      }
      i++;
    }
  } else {
    *dst = INT_MAX;
    return CONVERSION_ERROR;
  }
  if (get_sign(src) == 1) {
    *dst *= (-1);
  }
  return OK;
}