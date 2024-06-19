#include "common.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_long_decimal temp1 = to_long_decimal(value_1);
  s21_long_decimal temp2 = to_long_decimal(value_2);
  int ans;
  normalize_long(&temp1, &temp2);
  temp1 = config_for_sub(temp1, temp2);
  if (result) {
    *result = get_decimal_from_long(temp1);
  }
  ans = get_ans(temp1);
  return ans;
}
