#include "common.h"

// Узнать длину значимой части числа
int mant_len(s21_decimal num) {
  int idx;
  bool exit = false;
  for (idx = MANTISSA_LEN - 1; !exit && idx >= 0; --idx) {
    if (get_bit(num, idx) == 1) {
      exit = true;
      idx++;
    }
  }
  return idx + 1;
}

// Узнать длину значимой части числа
int mant_len_long(s21_long_decimal num) {
  int idx;
  bool exit = false;
  for (idx = LONG_MANTISSA_LEN - 1; !exit && idx >= 0; --idx) {
    if (get_bit_long(num, idx) == 1) {
      exit = true;
      idx++;
    }
  }
  return idx + 1;
}

// Получение значения бита по индексу
int get_bit(s21_decimal num, int idx) {
  if (idx / INT_SIZE >= DECIMAL_SIZE) return 0;

  int bit_idx = idx / INT_SIZE;
  idx = idx % INT_SIZE;
  return (num.bits[bit_idx] >> idx) & 1;
}

// Получение значения бита по индексу
int get_bit_long(s21_long_decimal num, int idx) {
  if (idx / INT_SIZE >= LONG_DECIMAL_SIZE) return 0;

  int bit_idx = idx / INT_SIZE;
  idx = idx % INT_SIZE;
  return (num.bits[bit_idx] >> idx) & 1;
}

// Получение бита, определяющего знак числа
int get_sign(s21_decimal num) {
  return num.bits[DECIMAL_SIZE - 1] >> (INT_SIZE - 1) & 1;
}

// Установить знак для числа (0 - положительное, 1 - отрицательное)
void set_sign(s21_decimal *num, int sign) {
  set_bit(num, sign, DECIMAL_LEN - 1);
}

// Установить знак для числа (0 - положительное, 1 - отрицательное)
void set_sign_long(s21_long_decimal *num, int sign) {
  set_bit_long(num, sign, LONG_DECIMAL_LEN - 1);
}

// Установить значение в бит
void set_bit(s21_decimal *num, int bit, int idx) {
  if (idx / INT_SIZE >= DECIMAL_SIZE) return;

  int bit_idx = idx / INT_SIZE;
  int new_idx = idx % INT_SIZE;
  if (bit == 1) {
    num->bits[bit_idx] |= 1 << new_idx;  // Установка бита в 1
  } else {
    num->bits[bit_idx] &= ~(1 << new_idx);  // Сброс бита в 0
  }
}

// Установить значение в бит
void set_bit_long(s21_long_decimal *num, int bit, int idx) {
  if (idx / INT_SIZE >= LONG_DECIMAL_SIZE) return;

  int bit_idx = idx / INT_SIZE;
  idx = idx % INT_SIZE;

  if (bit == 1) {
    num->bits[bit_idx] |= 1 << idx;  // Установка бита в 1
  } else {
    num->bits[bit_idx] &= ~(1 << idx);  // Сброс бита в 0
  }
}

// Функция установки порядка для decimal
int set_scale(s21_decimal *value, s21_scale scale) {
  for (int i = 0; i < SCALE_LEN; i++) {
    int bit = scale & 1;
    set_bit(value, bit, SCALE_POS + i);
    scale = scale >> 1;
  }
  return 0;
}

// Функция установки порядка для long_decimal
int set_scale_long(s21_long_decimal *num, s21_scale scale) {
  for (int i = 0; i < SCALE_LEN; i++) {
    int bit = scale & 1;
    set_bit_long(num, bit, LONG_SCALE_POS + i);
    scale = scale >> 1;
  }
  return 0;
}

// получить значение порядка long_decimal
s21_scale get_scale_long(s21_long_decimal num) {
  s21_scale scale = {0};
  for (int i = 0; i < SCALE_LEN; i++) {
    scale |= get_bit_long(num, LONG_SCALE_POS + i) << i;
  }
  return scale;
}

// получить значение порядка decimal
s21_scale get_scale(s21_decimal num) {
  s21_scale scale = {0};
  for (int i = 0; i < SCALE_LEN; i++) {
    scale |= get_bit(num, SCALE_POS + i) << i;
  }
  return scale;
}

// Сдвиг битов вправо в s21_long_decimal на заданное число
s21_long_decimal right_shift_long(s21_long_decimal value, int count) {
  if (count <= 0) return value;
  unsigned int carry = 0;
  s21_long_decimal result = value;
  for (int i = LONG_MANTISSA_SIZE - 1; i >= 0; i--) {
    unsigned int temp = value.bits[i];
    result.bits[i] = value.bits[i] >> count;
    result.bits[i] |= carry << (INT_SIZE - count);
    carry = temp & ((1 << count) - 1);
  }
  return result;
}

// Сдвиг битов влево в s21_long_decimal на заданное число
s21_long_decimal left_shift_long(s21_long_decimal value, int count) {
  if (count <= 0) return value;

  unsigned int carry = 0;
  s21_long_decimal result = value;

  for (int c = 0; c < count; c++) {
    carry = 0;  // Обнуляем carry перед каждым сдвигом

    for (int i = 0; i < LONG_MANTISSA_SIZE; i++) {
      unsigned int temp = result.bits[i];
      result.bits[i] = result.bits[i] << 1;
      result.bits[i] |= carry;
      carry = temp >> (INT_SIZE - 1);
    }
  }

  return result;
}

// Сдвиг битов влево в s21_long_decimal на заданное число
s21_decimal left_shift(s21_decimal value, int count) {
  if (count <= 0) return value;

  unsigned int carry = 0;
  s21_decimal result = value;

  for (int c = 0; c < count; c++) {
    carry = 0;  // Обнуляем carry перед каждым сдвигом

    for (int i = 0; i < MANTISSA_SIZE; i++) {
      unsigned int temp = result.bits[i];
      result.bits[i] = result.bits[i] << 1;
      result.bits[i] |= carry;
      carry = temp >> (INT_SIZE - 1);
    }
  }

  return result;
}

// Умножить мантиссу на 10 в заданной степени
void mant_up(s21_long_decimal *num, int count) {
  for (int i = 0; i < count; i++) {
    *num = sum_bits_long(left_shift_long(*num, 3), left_shift_long(*num, 1));
  }
}

// Приведение двух long_decimal чисел к одному порядку
void normalize_long(s21_long_decimal *num1, s21_long_decimal *num2) {
  s21_long_decimal *num_with_min = decimal_with_min_scale_long(num1, num2);
  if (num_with_min == NULL) {
    return;
  }

  s21_scale min_scale = get_scale_long(*num_with_min);
  s21_scale max_scale =
      get_scale_long(*num1) + get_scale_long(*num2) - min_scale;

  int sign = get_sign_long(*num_with_min);
  mant_up(num_with_min, (int)max_scale - (int)min_scale);
  set_scale_long(num_with_min, (int)max_scale);
  set_sign_long(num_with_min, sign);
}

// Возвращает число, которое получается из сложения мантисс
s21_long_decimal sum_bits_long(s21_long_decimal value_1,
                               s21_long_decimal value_2) {
  s21_long_decimal res_dec = {0};
  int carry_prev = 0;

  for (int i = 0; i < LONG_MANTISSA_SIZE; i++) {
    int carry = carry_prev;
    for (int j = 0; j < INT_SIZE; j++) {
      int bit_1 = get_bit_long(value_1, j + i * INT_SIZE);
      int bit_2 = get_bit_long(value_2, j + i * INT_SIZE);
      int sum = (bit_1 + bit_2 + carry) % 2;
      set_bit_long(&res_dec, sum, j + i * INT_SIZE);
      carry = (bit_1 + bit_2 + carry) / 2;
    }
    carry_prev = carry;
  }
  return res_dec;
}

// Возвращает число, которое получается из сложения мантисс
s21_decimal sum_bits(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal res_dec = {0};
  int carry_prev = 0;

  for (int i = 0; i < LONG_MANTISSA_SIZE; i++) {
    int carry = carry_prev;
    for (int j = 0; j < INT_SIZE; j++) {
      int bit_1 = get_bit(value_1, j + i * INT_SIZE);
      int bit_2 = get_bit(value_2, j + i * INT_SIZE);
      int sum = (bit_1 + bit_2 + carry) % 2;
      set_bit(&res_dec, sum, j + i * INT_SIZE);
      carry = (bit_1 + bit_2 + carry) / 2;
    }
    carry_prev = carry;
  }
  return res_dec;
}

// Возвращает указатель на минимальный long_decimal
s21_long_decimal *decimal_with_min_scale_long(s21_long_decimal *num1,
                                              s21_long_decimal *num2) {
  s21_scale sc1 = get_scale_long(*num1);
  s21_scale sc2 = get_scale_long(*num2);
  if (sc1 == sc2) {
    return NULL;
  } else if (sc1 < sc2) {
    return num1;
  } else {
    return num2;
  }
}
s21_decimal left_shift_decimal(s21_decimal value, int count) {
  if (count <= 0) return value;
  unsigned int carry = 0;
  s21_decimal result = value;
  for (int i = 0; i < LONG_MANTISSA_SIZE; i++) {
    unsigned int temp = value.bits[i];
    result.bits[i] = value.bits[i] << count;
    result.bits[i] |= carry;
    carry = temp >> (INT_SIZE - count);
  }
  return result;
}

// Переводит decimal в long_decimal
s21_long_decimal to_long_decimal(s21_decimal value) {
  s21_long_decimal result = {0};
  for (int i = 0; i < MANTISSA_LEN; i++) {
    set_bit_long(&result, get_bit(value, i), i);
  }
  for (int i = 0; i < INT_SIZE; i++) {
    set_bit_long(&result, get_bit(value, MANTISSA_LEN + i),
                 i + LONG_MANTISSA_LEN);
  }
  return result;
}

// moaneyoc: сдвиг до 32 вправо
s21_decimal right_shift(s21_decimal value, int count) {
  if (count <= 0) return value;
  unsigned int carry = 0;
  s21_decimal result = value;
  for (int i = MANTISSA_SIZE - 1; i >= 0; i--) {
    unsigned int temp = value.bits[i];
    result.bits[i] = value.bits[i] >> count;
    result.bits[i] |= carry << (INT_SIZE - count);
    carry = temp & ((1 << count) - 1);
  }
  return result;
}
// moaneyoc: сдвиг право для большого сдвига
s21_decimal right_big_shift(s21_decimal value, int count) {
  while (count > 0) {
    if (count >= 16) {
      value = right_shift(value, 16);
      count -= 16;
    } else {
      value = right_shift(value, count);
      count = 0;
    }
  }
  return value;
}
// moaneyoc: сдвиг право для большого сдвига
s21_long_decimal right_big_shift_long(s21_long_decimal value, int count) {
  while (count > 0) {
    if (count >= 16) {
      value = right_shift_long(value, 16);
      count -= 16;
    } else {
      value = right_shift_long(value, count);
      count = 0;
    }
  }
  return value;
}

// moaneyoc: побитовое вычитание
s21_decimal subtract(const s21_decimal a, const s21_decimal b) {
  s21_decimal result = {0};
  int borrow = 0;
  for (int i = 0; i < MANTISSA_SIZE; i++) {
    int diff = a.bits[i] - b.bits[i] - borrow;
    result.bits[i] = diff;
    borrow = (b.bits[i] + borrow > a.bits[i]) ? 1 : 0;
  }
  return result;
}

// moaneyoc: изменение числа под вычитание
s21_long_decimal config_for_sub(s21_long_decimal a, s21_long_decimal b) {
  s21_long_decimal result = {0};
  bool flag = false;
  int sign_b = get_sign_long(b);
  int sign_a = get_sign_long(a);
  int sign_result = 0;
  set_sign_long(&a, 0);
  set_sign_long(&b, 0);
  if (compare_long(a, b) == -1) {
    flag = true;
    s21_long_decimal temp = a;
    a = b;
    b = temp;
  }
  if (flag) {
    if (sign_a == 1) {
      if (sign_b == 1) {
        result = subtract_long(a, b);
        sign_result = 0;
      } else if (sign_b == 0) {
        result = sum_bits_long(a, b);
        sign_result = 1;
      }
    } else if (sign_a == 0) {
      if (sign_b == 1) {
        result = sum_bits_long(a, b);
        sign_result = 0;
      } else if (sign_b == 0) {
        result = subtract_long(a, b);
        sign_result = 1;
      }
    }
  } else {
    if (sign_a == 1) {
      if (sign_b == 1) {
        result = subtract_long(a, b);
        sign_result = 1;
      } else if (sign_b == 0) {
        result = sum_bits_long(a, b);
        sign_result = 1;
      }
    } else if (sign_a == 0) {
      if (sign_b == 1) {
        result = sum_bits_long(a, b);
        sign_result = 0;
      } else if (sign_b == 0) {
        result = subtract_long(a, b);
        sign_result = 0;
      }
    }
  }
  set_scale_long(&result, get_scale_long(a));
  set_sign_long(&result, sign_result);
  return result;
}

s21_long_decimal config_for_add(s21_long_decimal a, s21_long_decimal b) {
  s21_long_decimal result = {0};
  bool flag = false;
  int sign_b = get_sign_long(b);
  int sign_a = get_sign_long(a);
  int sign_result = 0;
  set_sign_long(&a, 0);
  set_sign_long(&b, 0);
  if (compare_long(a, b) == -1) {
    flag = true;
    s21_long_decimal temp = a;
    a = b;
    b = temp;
  }
  if (flag) {
    if (sign_a == 1) {
      if (sign_b == 1) {
        result = sum_bits_long(a, b);
        sign_result = 1;
      } else if (sign_b == 0) {
        result = subtract_long(a, b);
        sign_result = 0;
      }
    } else if (sign_a == 0) {
      if (sign_b == 1) {
        result = subtract_long(a, b);
        sign_result = 1;
      } else if (sign_b == 0) {
        result = sum_bits_long(a, b);
        sign_result = 0;
      }
    }
  } else {
    if (sign_a == 1) {
      if (sign_b == 1) {
        result = sum_bits_long(a, b);
        sign_result = 1;
      } else if (sign_b == 0) {
        result = subtract_long(a, b);
        sign_result = 1;
      }
    } else if (sign_a == 0) {
      if (sign_b == 1) {
        result = subtract_long(a, b);
        sign_result = 0;
      } else if (sign_b == 0) {
        result = sum_bits_long(a, b);
        sign_result = 0;
      }
    }
  }
  set_scale_long(&result, get_scale_long(a));
  set_sign_long(&result, sign_result);
  return result;
}

// moaneyoc: побитовое вычитание
s21_long_decimal subtract_long(s21_long_decimal a, s21_long_decimal b) {
  s21_long_decimal result = {0};
  int borrow = 0;
  for (int i = 0; i < LONG_MANTISSA_SIZE - 1; i++) {
    int diff = a.bits[i] - b.bits[i] - borrow;
    result.bits[i] = diff;
    borrow = (b.bits[i] + borrow > a.bits[i]) ? 1 : 0;
  }
  return result;
}

// moaneyoc: делит мантису на 10
s21_decimal div_10_mantissa(s21_decimal n) {
  s21_decimal q, r;
  q = sum_bits(right_big_shift(n, 1), right_big_shift(n, 2));
  q = sum_bits(q, right_big_shift(q, 4));
  q = sum_bits(q, right_big_shift(q, 8));
  q = sum_bits(q, right_big_shift(q, 16));
  q = sum_bits(q, right_big_shift(q, 32));
  q = sum_bits(q, right_big_shift(q, 64));
  q = right_big_shift(q, 3);
  r = subtract(n, (sum_bits(left_shift(q, 3), left_shift(q, 1))));
  s21_decimal temp6 = {{0x00000006, 0x00000000, 0x00000000, 0x00000000}};
  r = right_big_shift(sum_bits(r, temp6), 4);
  return sum_bits(q, r);
}

// moaneyoc: делит мантису на 10
s21_long_decimal div_10_mantissa_long(s21_long_decimal n) {
  s21_long_decimal q =
      sum_bits_long(right_big_shift_long(n, 1), right_big_shift_long(n, 2));
  s21_long_decimal bl = {{0x000000C0, 0x00000000, 0x00000000, 0x00000000,
                          0x00000000, 0x00000000, 0x00000000}};
  int i = 2;
  s21_long_decimal temp1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000,
                             0x00000000, 0x00000000, 0x00000000}};
  s21_long_decimal temp9 = {{0x00000009, 0x00000000, 0x00000000, 0x00000000,
                             0x00000000, 0x00000000, 0x00000000}};
  s21_long_decimal r;
  while (compare_long(left_shift_long(temp1, i), bl) == -1) {
    q = sum_bits_long(q, right_big_shift_long(q, (1 << i)));
    i++;
  }
  q = right_big_shift_long(q, 3);
  r = subtract_long(
      n, left_shift_long(sum_bits_long(left_shift_long(q, 2), q), 1));
  if (compare_long(r, temp9) == 1) {
    q = sum_bits_long(q, temp1);
  }
  return q;
}

// moaneyoc: возращает остаток от деления
s21_decimal remainder_div_10_mantissa(s21_decimal n) {
  int x = 0, q = 1;
  for (int i = 0; i < MANTISSA_LEN; ++i) {
    if (get_bit(n, i) == 1) {
      x = (x + q) % 10;
    }
    q = (q * 2) % 10;
  }
  s21_decimal ans = {{x, 0x00000000, 0x00000000, 0x00000000}};
  return ans;
}
// moaneyoc: возращает остаток от деления
s21_long_decimal remainder_div_10_mantissa_long(s21_long_decimal n) {
  int x = 0, q = 1;
  for (int i = 0; i < LONG_MANTISSA_LEN; ++i) {
    if (get_bit_long(n, i) == 1) {
      x = (x + q) % 10;
    }
    q = (q * 2) % 10;
  }
  s21_long_decimal ans = {{x, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                           0x00000000, 0x00000000}};
  return ans;
}

// moaneyoc: делит число на 10 и убирает порядок если он есть
// примеры: 123 -> 12
//          123.123 -> 123.12
s21_decimal div_10(s21_decimal n) {
  s21_decimal temp = n;
  if (get_scale(temp)) {
    set_scale(&temp, get_scale(temp) - 1);
  }
  n.bits[MANTISSA_SIZE] = 0x00000000;
  n = div_10_mantissa(n);
  n.bits[MANTISSA_SIZE] = temp.bits[MANTISSA_SIZE];
  return n;
}
// moaneyoc: делит число на 10 и убирает порядок если он есть
// примеры: 123 -> 12
//          123.123 -> 123.12

s21_long_decimal div_10_long(s21_long_decimal n) {
  s21_long_decimal temp = n;
  if (get_scale_long(temp)) {
    set_scale_long(&temp, get_scale_long(temp) - 1);
  }
  n.bits[LONG_MANTISSA_SIZE] = 0x00000000;
  n = div_10_mantissa_long(n);
  n.bits[LONG_MANTISSA_SIZE] = temp.bits[LONG_MANTISSA_SIZE];
  return n;
}

s21_long_decimal banking_OCD(s21_long_decimal value, s21_long_decimal r) {
  s21_long_decimal temp5 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000,
                             0x00000000, 0x00000000, 0x00000000}};
  s21_long_decimal temp6 = {{0x00000006, 0x00000000, 0x00000000, 0x00000000,
                             0x00000000, 0x00000000, 0x00000000}};
  s21_long_decimal temp1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000,
                             0x00000000, 0x00000000, 0x00000000}};
  int temp = value.bits[6];
  if (get_bit_long(value, 0)) {
    if (compare_long(r, temp5) >= 0) {
      value = sum_bits_long(value, temp1);
    }
  } else {
    if (compare_long(r, temp6) >= 0) {
      value = sum_bits_long(value, temp1);
    }
  }
  value.bits[6] = temp;
  return value;
}

// moaneyoc: переводит число из long в decimal
s21_decimal get_decimal_from_long(s21_long_decimal value) {
  s21_decimal result = {0};
  s21_decimal m = MAX_DECIMAL;
  s21_long_decimal r;
  bool flag = false;
  while ((value.bits[5] > 0 || value.bits[4] > 0 || value.bits[3] > 0) &&
         get_scale_long(value)) {
    r = remainder_div_10_mantissa_long(value);
    value = div_10_long(value);
    flag = true;
  }
  if (flag) {
    value = banking_OCD(value, r);
  }
  if (value.bits[5] > 0 || value.bits[4] > 0 || value.bits[3] > 0) {
    result = m;  // знак надо проверить но мне лень
  } else {
    result.bits[0] = value.bits[0];
    result.bits[1] = value.bits[1];
    result.bits[2] = value.bits[2];
    result.bits[3] = value.bits[6];
  }
  return result;
}

int get_sign_long(s21_long_decimal num) {
  return num.bits[LONG_DECIMAL_SIZE - 1] >> (INT_SIZE - 1) & 1;
}

// если 0 - равны, если 1 - первое больше, если -1 - второе больше
int compare_long(s21_long_decimal ld1, s21_long_decimal ld2) {
  int sign1 = get_sign_long(ld1);
  int sign2 = get_sign_long(ld2);

  if (!(is_zero_long_decimal(ld1)) && !(is_zero_long_decimal(ld2))) {
    return 0;
  }
  if (sign1 > sign2) {
    return -1;
  } else if (sign1 < sign2) {
    return 1;
  } else if (get_scale_long(ld1) != get_scale_long(ld2)) {
    normalize_long(&ld1, &ld2);
  }
  for (int i = LONG_DECIMAL_SIZE - 1; i >= 0; i--) {
    if (ld1.bits[i] < ld2.bits[i]) {
      if (sign1 && sign2)
        return 1;
      else
        return -1;
    } else if (ld1.bits[i] > ld2.bits[i]) {
      if (sign1 && sign2)
        return -1;
      else
        return 1;
    }
  }
  return 0;
}

// если 0 - равны, если 1 - первое больше, если -1 - второе больше
int compare_mantissa_long(s21_long_decimal ld1, s21_long_decimal ld2) {
  for (int i = LONG_DECIMAL_SIZE - 2; i >= 0; i--) {
    if (ld1.bits[i] < ld2.bits[i]) {
      return -1;
    } else if (ld1.bits[i] > ld2.bits[i]) {
      return 1;
    }
  }
  return 0;
}

int compare(s21_decimal d1, s21_decimal d2) {
  s21_long_decimal ld1 = to_long_decimal(d1);
  s21_long_decimal ld2 = to_long_decimal(d2);

  int sign1 = get_sign_long(ld1);
  int sign2 = get_sign_long(ld2);

  normalize_long(&ld1, &ld2);

  if (!(is_zero_long_decimal(ld1)) && !(is_zero_long_decimal(ld2))) {
    return 0;
  }

  if (sign1 > sign2) {
    return -1;
  } else if (sign1 < sign2) {
    return 1;
  }

  for (int i = LONG_DECIMAL_SIZE - 1; i >= 0; i--) {
    if (ld1.bits[i] < ld2.bits[i]) {
      if (sign1 && sign2)
        return 1;
      else
        return -1;
    } else if (ld1.bits[i] > ld2.bits[i]) {
      if (sign1 && sign2)
        return -1;
      else
        return 1;
    }
  }
  return 0;
}

void set_to_zero(s21_decimal *val) {
  val->bits[0] = 0;
  val->bits[1] = 0;
  val->bits[2] = 0;
  val->bits[3] = 0;
}

int get_mantissa(char *str) {
  int mant = 0;
  int idx = 0;
  while (idx < 7 && *str != '\0') {
    if (*str - '0' >= 0 && *str - '0' <= 9) {
      mant += pow(10, 7 - idx - 1) * (*str - '0');
      idx++;
    }
    str++;
  }
  return mant;
}

s21_scale get_scale_from_str(char *str) {
  int len = strlen(str);
  int value = (str[len - 2] - '0') * 10 + (str[len - 1] - '0');
  return (s21_scale)value;
}

int get_ans(s21_long_decimal value) {
  s21_long_decimal r;
  int ans = 0;
  bool flag = false;
  while ((value.bits[5] > 0 || value.bits[4] > 0 || value.bits[3] > 0) &&
         get_scale_long(value)) {
    r = remainder_div_10_mantissa_long(value);
    value = div_10_long(value);
    flag = true;
  }
  if (flag) {
    value = banking_OCD(value, r);
  }
  if (value.bits[5] > 0 || value.bits[4] > 0 || value.bits[3] > 0) {
    ans = 1;
    if (get_sign_long(value) == 1) {
      ans = 2;
    }
  }
  return ans;
}

// hotbl/ зануляем s21_decimal
void zero_s21_decimal(s21_decimal *value) {
  value->bits[0] = value->bits[1] = value->bits[2] = value->bits[3] = 0;
}

int is_zero_long_decimal(s21_long_decimal big) {
  int result = 0;
  for (int i = 4; i >= 0; i--) {
    if (big.bits[i] != 0) {
      result = 1;
      break;
    }
  }
  return result;
}
// hotbl/ проверяет на ноль s21_decimal
int is_zero_s21_decimal(s21_decimal value) {
  int res = 0;
  for (int i = 2; i >= 0; i--) {
    if (value.bits[i] != 0) {
      res = 1;
      break;
    }
  }
  // res = value.bits[0] + value.bits[1] + value.bits[2];
  return res;
}

// деление с s21_long_decimal
void div_big(s21_long_decimal value1, s21_long_decimal value2,
             s21_long_decimal *res) {
  int q = 0;
  s21_long_decimal q_part = {0};  // вычитаемое из делителя при найденном q
  s21_long_decimal q_part_next = {0};  // 2^q или q+1
  s21_long_decimal result = {0};  // текущая сумма, которая должна знать ответом
  s21_long_decimal pre_result = {0};  // новый член в сумме

  while (is_zero_long_decimal(value1) && get_scale_long(value1) > 0 &&
         is_zero_long_decimal(value2)) {
    while (s21_is_mant_greater_long(value2, value1)) {
      s21_scale scale_1 = get_scale_long(value1);
      s21_scale scale_res = get_scale_long(result);
      mant_up(&value1, 1);
      set_scale_long(&value1, scale_1 + 1);
      mant_up(&result, 1);
      set_scale_long(&result, scale_res + 1);
    }

    q = 0;
    q_part = value2;
    q_part_next = value2;

    while (s21_is_mant_greater_or_equal_long(  // условие не работает - тк
               value1, q_part_next) &&
           is_zero_long_decimal(value1)) {  // пока делитель(val2)*2^q < остаток
      q_part = q_part_next;
      q_part_next = left_shift_long(q_part_next, 1);
      q++;
    }
    set_bit_long(&pre_result, 1, q);
    result =
        sum_bits_long(result, pre_result);  // мне нужно лонговое сложение с
    // записью результата в переменную.
    if (s21_is_mant_greater_or_equal_long(value1, q_part)) {
      value1 = subtract_long(value1, q_part);  //+ вычесть
    }
  }
  *res = result;
}
