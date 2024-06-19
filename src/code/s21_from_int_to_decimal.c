#include "common.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) {
    return CONVERSION_ERROR;
  }
  set_to_zero(dst);
  if (src < 0) {
    set_sign(dst, 1);
    src *= (-1);
  }
  dst->bits[0] = src;
  return OK;
}
