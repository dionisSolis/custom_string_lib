CFLAGS=-std=c11 -Wall -Werror -Wextra
LDFLAGS = -lm -lcheck

SRCS = tests/test_string.c s21_sprintf.c s21_string.c scanf/s21_scanf.c
OBJS = $(SRCS:.c=.o)

all: s21_string.a

s21_string.a:
	gcc $(CFLAGS) -c s21_string.c -o s21_string.o
	ar rcs s21_string.a s21_string.o
	ranlib s21_string.a

test: s21_string.a
	gcc $(CFLAGS) -fprofile-arcs -ftest-coverage $(SRCS) -lcheck -lsubunit -lm -o test_start
	./test_start

gcov_report: test_start
	mkdir -p gcov_report
	./test_start
	lcov --capture --directory . --output-file gcov_report/coverage.info
	genhtml gcov_report/coverage.info --output-directory gcov_report/html

clean:
	rm -rf *.o *.a test_start *.gcno *.gcda *.info gcov_report

.PHONY: all clean gcov_report