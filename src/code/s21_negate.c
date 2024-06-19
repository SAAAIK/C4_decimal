#include "common.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  if (get_sign(value) == 0) {
    set_sign(result, 1);
  } else {
    set_sign(result, 0);
  }
  return 0;
}