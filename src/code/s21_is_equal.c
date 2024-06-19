#include "common.h"

int s21_is_equal(s21_decimal d1, s21_decimal d2) {
  int c = compare(d1, d2);
  if (c == 0) {
    return 1;
  } else
    return 0;
}