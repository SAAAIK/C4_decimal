#include "common.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (result) {
    zero_s21_decimal(result);
  }
  if (!is_zero_s21_decimal(value_2)) {
    status = 3;
  } else {
    /*Взять знак*/
    int new_sign = (get_sign(value_1) + get_sign(value_2)) % 2;
    /*Найти порядок результата - он равен разности порядков делимого и
     * делителя*/
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    /*Поделить мантиссы*/
    s21_long_decimal lvalue_1;
    s21_long_decimal lvalue_2;
    s21_long_decimal lresult = {0};

    lvalue_1 = to_long_decimal(value_1);
    lvalue_2 = to_long_decimal(value_2);
    normalize_long(&lvalue_1, &lvalue_2);
    lresult = to_long_decimal(*result);
    div_big(lvalue_1, lvalue_2, &lresult);
    *result = get_decimal_from_long(lresult);
    set_sign(result, new_sign);
  }
  return status;
}