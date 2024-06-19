#include "common.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  set_to_zero(dst);
  char str[256] = "";
  sprintf(str, "%E", src);
  s21_scale scale = get_scale_from_str(str);
  int mantissa = get_mantissa(str);
  int sign = 0;
  if (str[0] - '0' >= 0 && str[0] - '0' <= 9) {
    sign = 0;
  } else if (str[0] == '-') {
    sign = 1;
  }
  dst->bits[0] = mantissa;

  s21_decimal zero = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  while (compare(remainder_div_10_mantissa(*dst), zero) == 0 &&
         dst->bits[0] > 0) {
    *dst = div_10_mantissa(*dst);
  }
  dst->bits[3] = 0;
  set_sign(dst, sign);
  set_scale(dst, scale);
  if (scale > 28) {
    return 1;
  }
  return OK;
}