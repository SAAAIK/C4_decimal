#include "common.h"

// moaneyoc: Округляет указанное Decimal число до ближайшего целого числа в
// сторону отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
  bool flag = false;
  bool is_r = false;
  int n = get_scale(value);
  s21_decimal r;
  s21_decimal temp0 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  while (get_scale(value)) {
    r = remainder_div_10_mantissa(value);
    if (compare(r, temp0) != 0) {
      is_r = true;
    }
    value = div_10(value);
    flag = true;
  }
  if (flag && get_sign(value) == 1 && is_r) {
    s21_decimal temp1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
    int temp = value.bits[3];
    value = sum_bits(value, temp1);
    value.bits[3] = temp;
  }
  if (result) {
    *result = value;
  }
  if (!result || n > 28) {
    return 1;
  }
  return 0;
}