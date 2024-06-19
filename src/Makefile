CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -std=c11
LIB_NAME = s21_decimal
CODE_PATH = ./code
TESTS_PATH = ./tests
BUILD_PATH = ./build
TEST= tests.sh
TEST_FLAGS = -std=c11 -g
GCOVFLAGS = -L. -l$(LIB_NAME) --coverage
OS = $(shell uname)
DIV_FILES = $(CODE_PATH)/common.c $(CODE_PATH)/s21_decimal.c $(CODE_PATH)/s21_div.c $(CODE_PATH)/s21_div_big.c $(CODE_PATH)/s21_is* $(CODE_PATH)/s21_from_de* $(CODE_PATH)/s21_truncate* $(CODE_PATH)/fast_code.c 

ifeq ($(OS), Linux)
	CHECK_FLAGS = -lcheck -pthread -lrt -lm -lsubunit
	#CHECK_FLAGS = -lcheck -pthread -lrt -lm
else
	CHECK_FLAGS = -lcheck 
endif

UTIL = -fsanitize=address

TEST_COMPILE = $(CC) $(CC_FLAGS) $(TEST_FLAGS) tests/test*.c lib$(LIB_NAME).a $(CHECK_FLAGS) -L. -l$(LIB_NAME)

all: style test clean

peer: gcov_report

clean:
	rm -rf *.o *.g* *.info *.out report *.a *.dSYM debug *.txt $(TESTS_PATH)/make* ./report/*

style:
	clang-format -i $(CODE_PATH)/*.c $(CODE_PATH)/*.h ./*.h $(TESTS_PATH)/*.c $(TESTS_PATH)/*.h
	clang-format -n $(CODE_PATH)/*.c $(CODE_PATH)/*.h ./*.h $(TESTS_PATH)/*.c $(TESTS_PATH)/*.h

leaks:
	cppcheck --enable=all --suppress=missingIncludeSystem .
	cppcheck --enable=all --suppress=missingIncludeSystem $(CODE_PATH)

good: clean all test
	sleep 3
	clear

build:
	gcc $(CC_FLAGS) $(CODE_PATH)/common.c $(CODE_PATH)/s21_decimal.c  $(CODE_PATH)/s21_div_big.c $(CODE_PATH)/s21_is* $(CODE_PATH)/s21_from_decimal_to_int.c $(CODE_PATH)/s21_truncate* $(CODE_PATH)/fast_code.c -lm
	./a.out

rebuild: clean all


test: $(LIB_NAME).a
	$(TEST_COMPILE)
	-./a.out

$(LIB_NAME).a: $(LIB_NAME).o
	ar rcs lib$(LIB_NAME).a *.o
	rm -rf *.o
	ranlib lib$(LIB_NAME).a
	cp lib$(LIB_NAME).a $(LIB_NAME).a
	cp lib$(LIB_NAME).a l$(LIB_NAME).a

$(LIB_NAME).o:
	$(CC) $(CC_FLAGS) -c $(CODE_PATH)/*.c

gcov_report: clean
	$(CC) $(CC_FLAGS) -c $(CODE_PATH)/*.c $(CODE_PATH)/*.c $(GCOVFLAGS)
	#$(CC) -c $(CODE_PATH)/*.c $(CODE_PATH)/*.c $(GCOVFLAGS)
	ar rcs lib$(LIB_NAME).a *.o
	rm -rf *.o
	ranlib lib$(LIB_NAME).a
	cp lib$(LIB_NAME).a $(LIB_NAME).a
	cp lib$(LIB_NAME).a l$(LIB_NAME).a
	gcc $(TEST_FLAGS) $(TESTS_PATH)/test_*.c lib$(LIB_NAME).a $(CHECK_FLAGS) $(GCOVFLAGS)
	-./a.out
	lcov -t a.out -o all_test.info -c -d .
	genhtml -o report all_test.info
	rm -rf *.gcda *.gcno ./a.out *.info $(TESTS_PATH)/*.a $(TESTS_PATH)/*.o ./*.a
	#-open ./report/index.html
	#vivaldi ./report/index.html
