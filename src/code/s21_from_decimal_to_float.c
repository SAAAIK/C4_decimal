#include "common.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return CONVERSION_ERROR;
  *dst = 0.0;
  s21_scale scale = get_scale(src);
  int sign = get_sign(src);
  double temp = 0.0;
  for (int j = mant_len(src) - 1; j >= 0; j--) {
    if (get_bit(src, j) != 0) temp += pow(2.0, j);
  }
  double scale_pow = pow(10, (int)scale);
  temp /= scale_pow;
  if (sign == 1) {
    temp *= (-1.0);
  }
  *dst = (float)temp;
  return OK;
}