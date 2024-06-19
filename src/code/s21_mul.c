#include "common.h"

// Возвращает байт, который получается в результате суммы порядков двух чисел
s21_scale sum_scale(s21_decimal value_1, s21_decimal value_2) {
  return get_scale(value_1) + get_scale(value_2);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int len = mant_len(value_1);
  s21_long_decimal lvalue_1 = to_long_decimal(value_1);
  s21_long_decimal lvalue_2 = to_long_decimal(value_2);

  s21_long_decimal lresult = {0};

  for (int i = 0; i < len; i++) {
    if (get_bit_long(lvalue_1, i) == 1) {
      lresult = sum_bits_long(lresult, left_shift_long(lvalue_2, i));
    }
  }

  set_scale_long(&lresult, sum_scale(value_1, value_2));

  if (result) {
    if (mant_len_long(lresult) > MANTISSA_LEN)
      return 1 + (get_sign(value_1) ^ get_sign(value_2));

    *result = get_decimal_from_long(lresult);
    set_sign(result, get_sign(value_1) ^ get_sign(value_2));
  }

  return 0;
}