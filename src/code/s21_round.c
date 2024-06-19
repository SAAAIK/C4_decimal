#include "common.h"

// moaneyoc: Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result) {
  s21_decimal r;
  bool flag = false;
  s21_decimal temp5 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  int n = get_scale(value);
  while (get_scale(value)) {
    r = remainder_div_10_mantissa(value);
    value = div_10(value);
    flag = true;
  }
  if (flag && compare(r, temp5) >= 0) {
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