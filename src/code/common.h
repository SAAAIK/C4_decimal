#ifndef COMMON_H
#define COMMON_H

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_decimal.h"

#define INT_SIZE 32

#define DECIMAL_SIZE 4
#define DECIMAL_LEN (INT_SIZE * DECIMAL_SIZE)

#define LONG_DECIMAL_SIZE 7
#define LONG_DECIMAL_LEN (INT_SIZE * LONG_DECIMAL_SIZE)

#define MANTISSA_SIZE 3
#define MANTISSA_LEN (INT_SIZE * MANTISSA_SIZE)
#define LONG_MANTISSA_SIZE MANTISSA_SIZE * 2
#define LONG_MANTISSA_LEN MANTISSA_LEN * 2

#define SCALE_POS (MANTISSA_LEN + 16)
#define LONG_SCALE_POS (LONG_MANTISSA_LEN + 16)
#define SCALE_LEN 8

typedef enum return_values {
  OK = 0,
  CONVERSION_ERROR = 1,
} return_values;

#define MAX_DECIMAL                                                            \
  {                                                                            \
    { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 }                         \
  }
#define LONG_MAX_DECIMAL                                                       \
  {                                                                            \
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,    \
        0x00000000                                                             \
  }

void print_bits(int number);
int power(int n, int i);

int mant_len(s21_decimal num);
int mant_len_long(s21_long_decimal num);

int get_bit(s21_decimal num, int idx);
int get_bit_long(s21_long_decimal num, int idx);

s21_long_decimal *compare_scale_long(s21_long_decimal *num1,
                                     s21_long_decimal *num2);

s21_long_decimal left_shift_long(s21_long_decimal value, int count);
s21_long_decimal right_shift_long(s21_long_decimal value, int count);

int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, int sign);
void set_sign_long(s21_long_decimal *num, int sign);

int set_scale(s21_decimal *num, s21_scale scale);
s21_scale get_scale(s21_decimal num);

int set_scale_long(s21_long_decimal *num, s21_scale scale);
s21_scale get_scale_long(s21_long_decimal num);

void set_bit(s21_decimal *num, int bit, int idx);
void set_bit_long(s21_long_decimal *num, int bit, int idx);

void output_bytes(s21_decimal value);
void output_bytes_long(s21_long_decimal value);

s21_long_decimal to_long_decimal(s21_decimal value);

void scale_up(s21_long_decimal *num, int count);
void normalize_long(s21_long_decimal *num1, s21_long_decimal *num2);

s21_long_decimal *decimal_with_min_scale_long(s21_long_decimal *num1,
                                              s21_long_decimal *num2);
s21_long_decimal sum_bits_long(s21_long_decimal value_1,
                               s21_long_decimal value_2);

s21_decimal sum_bits(s21_decimal value_1, s21_decimal value_2);

s21_decimal left_shift(s21_decimal value, int count);
s21_decimal right_shift(s21_decimal value, int count);
s21_decimal right_big_shift(s21_decimal value, int count);
s21_long_decimal right_big_shift_long(s21_long_decimal value, int count);
s21_decimal subtract(const s21_decimal a, const s21_decimal b);
s21_long_decimal subtract_long(const s21_long_decimal a,
                               const s21_long_decimal b);
s21_decimal div_10_mantissa(s21_decimal n);
s21_long_decimal div_10_mantissa_long(s21_long_decimal n);
s21_decimal remainder_div_10_mantissa(s21_decimal n);
s21_long_decimal remainder_div_10_mantissa_long(s21_long_decimal n);
s21_decimal div_10(s21_decimal n);
s21_decimal get_decimal_from_long(s21_long_decimal value);
s21_long_decimal config_for_sub(s21_long_decimal a, s21_long_decimal b);
s21_long_decimal config_for_add(s21_long_decimal a, s21_long_decimal b);
s21_long_decimal banking_OCD(s21_long_decimal value, s21_long_decimal r);

int compare(s21_decimal d1, s21_decimal d2);
int compare_long(s21_long_decimal d1, s21_long_decimal d2);
int get_sign_long(s21_long_decimal num);
void mant_up(s21_long_decimal *num, int count);
s21_long_decimal left_shift_long(s21_long_decimal value, int count);

// hotbl
int is_zero_s21_decimal(s21_decimal value);
int is_zero_long_decimal(s21_long_decimal big);
void zero_s21_decimal(s21_decimal *value);
void zero_s21_decimal_long(s21_long_decimal *value);
int s21_is_greater_or_equal_long(s21_long_decimal d1, s21_long_decimal d2);
int s21_is_greater_long(s21_long_decimal d1, s21_long_decimal d2);

void set_to_zero(s21_decimal *val);
// void get_mantissa(int *mantissa, char *str);
int get_mantissa(char *str);

// void get_scale_from_str(s21_scale *scale, char *str);
s21_scale get_scale_from_str(char *str);

int get_ans(s21_long_decimal value);
int s21_is_mant_greater_long(s21_long_decimal d1, s21_long_decimal d2);
int compare_mantissa_long(s21_long_decimal ld1, s21_long_decimal ld2);
int s21_is_mant_greater_or_equal_long(s21_long_decimal d1, s21_long_decimal d2);
int is_zero_long_decimal_mant(s21_long_decimal big);
int is_zero_long(s21_long_decimal big);

void div_big(s21_long_decimal value1, s21_long_decimal value2,
             s21_long_decimal *res);
#endif