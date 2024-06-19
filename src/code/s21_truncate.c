#include "common.h"

// moaneyoc: Возвращает целые цифры указанного Decimal числа; любые дробные
// цифры отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int n = get_scale(value);
  while (get_scale(value)) {
    value = div_10(value);
  }
  if (result) {
    *result = value;
  }
  if (!result || n > 28) {
    return 1;
  }
  return 0;
}