#include "test_me.h"

static s21_decimal num1[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 0: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x00030000}},  // i= 1: 0.000
    {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}},  // i= 2: 1.5
    {{0x00000001, 0x00000000, 0x00000000,
      0x000F0000}},  // i= 3: 0.000000000000001
    {{0xC4D5AAEC, 0x00000025, 0x00000000, 0x00060000}},  // i= 4:162216.127212
    {{0x74489B46, 0x11C6B4C4, 0x00000000,
      0x00050000}},  // i= 5: 12809098998909.89894
    {{0xA4C194B9, 0xDF05E1A3, 0x000786BE,
      0x00050000}},  // i= 6: 90987987978798797979.37337
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  // i= 7: -79228162514264337593543950335
};

static s21_decimal num2[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 0: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80050000}},  // i= 1: 0.00000
    {{0x00000002, 0x00000000, 0x00000000, 0x00000000}},  // i= 2: 2
    {{0x540BE400, 0x00000002, 0x00000000, 0x00000000}},  // i= 3:10000000000
    {{0x0000033B, 0x00000000, 0x00000000, 0x00040000}},  // i= 4: 0.0827
    {{0x0005C187, 0x00000000, 0x00000000,
      0x000F0000}},  // i= 5: 0.000000000377223
    {{0x000003DD, 0x00000000, 0x00000000, 0x00000000}},  // i= 6: 989
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},  // i= 7: -1
};

static s21_decimal result[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 0: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80080000}},  // i= 1: 0.00000000
    {{0x0000001E, 0x00000000, 0x00000000, 0x00010000}},  // i= 2: 3.0
    {{0x540BE400, 0x00000002, 0x00000000,
      0x000F0000}},  // i= 3: 0.000010000000000
    {{0xDE3F2864, 0x00007A02, 0x00000000,
      0x000A0000}},  // i= 4: 13415.2737204324
    {{0x17B7A7EA, 0xB5E7B4AF, 0x00006651,
      0x00140000}},  // i= 5: 4831.88675166578880784362
    {{0x7FDB8EB5, 0x99B8B533, 0x1D138F63,
      0x00050000}},  // i= 6: 89987120111032011201600.26293
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // i= 7: 79228162514264337593543950335
};

static s21_decimal err_num1[] = {
    {{0x00000010, 0x00000000, 0x00000000, 0x80010000}},  // -1.6
    {{0x00000069, 0x00000000, 0x00000000, 0x00010000}},  // 10.5
    {{0x0000000B, 0x00000000, 0x00000000, 0x80010000}},  // -1.1
    {{0x0000006A, 0x00000000, 0x00000000, 0x00010000}},  // 10.6
};

static s21_decimal err_num2[] = {
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  //-79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  //-79228162514264337593543950335
};

static int err_result[] = {
    2,
    1,
    1,
    2,
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp = {0};
    int ret = s21_mul(num1[i], num2[i], &tmp);
    ck_assert_int_eq(tmp.bits[0], result[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result[i].bits[2]);
    ck_assert_int_eq(tmp.bits[3], result[i].bits[3]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

START_TEST(error_test) {
  for (size_t i = 0; i < sizeof(err_num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp = {0};
    int ret = s21_mul(err_num1[i], err_num2[i], &tmp);
    if (tmp.bits[0] == 0) {
      tmp.bits[0] = 1;
    }
    ck_assert_int_eq(ret, err_result[i]);
  }
}
END_TEST

Suite *suite_mul(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("s21_mul");
  tc = tcase_create("s21_mul");

  if (s != NULL && tc != NULL) {
    tcase_add_test(tc, test);
    tcase_add_test(tc, error_test);
    suite_add_tcase(s, tc);
  }

  return (s);
}
