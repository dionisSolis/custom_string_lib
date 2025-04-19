#ifndef S21_SCANF_H
#define S21_SSCANF_H

#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef unsigned long s21_size_t;

int s21_sscanf(const char *str, const char *format, ...);

int checkEmpty(const char *str);

long int s21_sscanf_process_d(const char **str, int *countN, int width,
                              int *isNotEmpty);
int s21_sscanf_extract_number(const char **str, char **numberStr,
                              int *isNegative, int *countN, int widthMax);

long int s21_sscanf_process_i(const char **str, int *countN, int width,
                              int *isNotEmpty);
void s21_sscanf_process_i_add_char_to_str(char **numberStr, int *count,
                                          const char **str, int *countN);
long int s21_sscanf_octal_to_decimal(const char *octal);
long int s21_sscanf_hex_to_decimal(const char *hex);

long double s21_sscanf_process_e(const char **str, int *countN, int width,
                                 int *isNotEmpty);
long double s21_sscanf_process_e_str_to_double(char **temp_ptr);

long unsigned int s21_sscanf_process_o(const char **str, int *countN, int width,
                                       int *isNotEmpty);
long unsigned int s21_sscanf_process_u(const char **str, int *countN, int width,
                                       int *isNotEmpty);
long unsigned int s21_sscanf_process_x(const char **str, int *countN, int width,
                                       int *isNotEmpty);

void s21_sscanf_process_s(const char **str, char *output, int *countN,
                          int width);
void s21_sscanf_process_ls(const char **str, wchar_t *output, int *countN,
                           int width);

void s21_sscanf_process_p(const char **str, void **ptr, int *countN, int width,
                          int *isNotEmpty);

#endif