#include "s21_scanf.h"

int s21_sscanf(const char *str, const char *format, ...) {
  va_list input;
  va_start(input, format);
  const char *s = str;
  int count = 0;
  int countN = 0;

  while (*format || *s) {
    if (*format == '%') {
      format++;
      int isNotEmpty = 1;

      int skip = 0;
      if (*format == '*') {
        skip += 1;
        format++;
      }

      int width = 0;
      while (isdigit(*format)) {
        width = width * 10 + (*format - '0');
        format++;
      }

      char length = ' ';
      if (*format == 'l' || *format == 'h' || *format == 'L') {
        length = *format;
        format++;
      }

      if (*format == 'c') {
        int readChars = (width > 0) ? width : 1;

        if (length == 'l') {
          if (skip != 1) {
            wchar_t *wptr = va_arg(input, wchar_t *);

            for (int i = 0; i < readChars && *s; i++) {
              size_t bytes = mbtowc(&wptr[i], s, MB_CUR_MAX);
              s += bytes;
              countN += bytes;
            }

            count++;
          } else {
            for (int i = 0; i < readChars && *s; i++) {
              wchar_t dummy;
              size_t bytes = mbtowc(&dummy, s, MB_CUR_MAX);
              if (bytes == (size_t)-1) break;
              s += bytes;
              countN += bytes;
            }
          }
        } else {
          if (skip != 1) {
            char *ptr = va_arg(input, char *);
            for (int i = 0; i < readChars && *s; i++) {
              ptr[i] = *s++;
              countN++;
            }
            count++;
          } else {
            s += readChars;
            countN += readChars;
          }
        }

        format++;
      } else if (*format == 'd') {
        if (skip != 1) {
          long int num = s21_sscanf_process_d(&s, &countN, width, &isNotEmpty);

          if (length == 'h') {
            short int *ptr = va_arg(input, short int *);
            *ptr = (short int)num;
          } else if (length == 'l') {
            long int *ptr = va_arg(input, long int *);
            *ptr = num;
          } else {
            int *ptr = va_arg(input, int *);
            *ptr = (int)num;
          }

          count += isNotEmpty;
        } else {
          s21_sscanf_process_d(&s, &countN, width, &isNotEmpty);
        }
        format++;
      } else if (*format == 'i') {
        if (skip != 1) {
          long int num = s21_sscanf_process_i(&s, &countN, width, &isNotEmpty);

          if (length == 'h') {
            short int *ptr = va_arg(input, short int *);
            *ptr = (short int)num;
          } else if (length == 'l') {
            long int *ptr = va_arg(input, long int *);
            *ptr = num;
          } else {
            int *ptr = va_arg(input, int *);
            *ptr = (int)num;
          }

          count += isNotEmpty;
        } else {
          s21_sscanf_process_i(&s, &countN, width, &isNotEmpty);
        }
        format++;
      } else if (*format == 'e' || *format == 'E' || *format == 'f' ||
                 *format == 'g' || *format == 'G') {
        if (skip != 1) {
          long double num =
              s21_sscanf_process_e(&s, &countN, width, &isNotEmpty);

          if (length == 'L') {
            long double *ptr = va_arg(input, long double *);
            *ptr = num;
          } else {
            float *ptr = va_arg(input, float *);
            *ptr = (float)num;
          }

          count += isNotEmpty;
        } else {
          s21_sscanf_process_e(&s, &countN, width, &isNotEmpty);
        }
        format++;
      } else if (*format == 'o') {
        if (skip != 1) {
          long unsigned int num =
              s21_sscanf_process_o(&s, &countN, width, &isNotEmpty);

          if (length == 'h') {
            short unsigned int *ptr = va_arg(input, short unsigned int *);
            *ptr = (short unsigned int)num;
          } else if (length == 'l') {
            long unsigned int *ptr = va_arg(input, long unsigned int *);
            *ptr = num;
          } else {
            unsigned int *ptr = va_arg(input, unsigned int *);
            *ptr = (unsigned int)num;
          }

          count += isNotEmpty;
        } else {
          s21_sscanf_process_o(&s, &countN, width, &isNotEmpty);
        }

        format++;
      } else if (*format == 's') {
        if (length == 'l') {
          if (skip != 1) {
            wchar_t *wptr = va_arg(input, wchar_t *);
            s21_sscanf_process_ls(&s, wptr, &countN, width);
            count += 1;
          } else {
            wchar_t temp;
            s21_sscanf_process_ls(&s, &temp, &countN, width);
          }
        } else {
          if (skip != 1) {
            char *ptr = va_arg(input, char *);
            s21_sscanf_process_s(&s, ptr, &countN, width);
            count++;
          } else {
            char temp_buf[1024];
            s21_sscanf_process_s(&s, temp_buf, &countN, width);
          }
        }

        format++;
      } else if (*format == 'u') {
        if (skip != 1) {
          long unsigned int num =
              s21_sscanf_process_u(&s, &countN, width, &isNotEmpty);

          if (length == 'h') {
            short unsigned int *ptr = va_arg(input, short unsigned int *);
            *ptr = (short unsigned int)num;
          } else if (length == 'l') {
            long unsigned int *ptr = va_arg(input, long unsigned int *);
            *ptr = num;
          } else {
            unsigned int *ptr = va_arg(input, unsigned int *);
            *ptr = (unsigned int)num;
          }

          count += isNotEmpty;
        } else {
          s21_sscanf_process_u(&s, &countN, width, &isNotEmpty);
        }

        format++;
      } else if (*format == 'x' || *format == 'X') {
        if (skip != 1) {
          long unsigned int num =
              s21_sscanf_process_x(&s, &countN, width, &isNotEmpty);

          if (length == 'h') {
            short unsigned int *ptr = va_arg(input, short unsigned int *);
            *ptr = (short unsigned int)num;
          } else if (length == 'l') {
            long unsigned int *ptr = va_arg(input, long unsigned int *);
            *ptr = num;
          } else {
            unsigned int *ptr = va_arg(input, unsigned int *);
            *ptr = (unsigned int)num;
          }

          count += isNotEmpty;
        } else {
          s21_sscanf_process_x(&s, &countN, width, &isNotEmpty);
        }

        format++;
      } else if (*format == 'p') {
        if (skip != 1) {
          void **ptr = va_arg(input, void *);
          s21_sscanf_process_p(&s, ptr, &countN, width, &isNotEmpty);
          count += isNotEmpty;
        } else {
          void *ptr;
          s21_sscanf_process_p(&s, &ptr, &countN, width, &isNotEmpty);
        }

        format++;
      } else if (*format == 'n') {
        int *ptr = va_arg(input, int *);
        *ptr = countN;
        format++;
      } else if (*format == '%') {
        if (*s != '%') {
          break;
        }

        s++;
        format++;
        countN++;
      }
    } else {
      while (isspace(*format)) {
        format++;
      }

      while (isspace(*s)) {
        s++;
        countN++;
      }

      if (*format != *s && *format != '%') {
        break;
      } else if (*format != '%') {
        format++;
        s++;
        countN++;
      }
    }
  }

  va_end(input);
  return count;
}

int checkEmpty(const char *str) {
  int isNotEmpty = 1;

  if (str == NULL || *str == '\0') {
    isNotEmpty = 0;
  }

  return isNotEmpty;
}

long int s21_sscanf_process_d(const char **str, int *countN, int width,
                              int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int isNegative = 0;
  int maxWidth = (width > 0) ? width : 32;

  s21_sscanf_extract_number(str, &numberStr, &isNegative, countN, maxWidth);
  *isNotEmpty = checkEmpty(numberStr);

  long int number = 0;
  char *temp = numberStr;

  while (*temp) {
    number = number * 10 + (*temp - '0');
    temp++;
  }

  free(numberStr);

  if (isNegative == 1) {
    number = -number;
  }

  return number;
}

long int s21_sscanf_process_i(const char **str, int *countN, int width,
                              int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int count = 0;
  int isNegative = 0;
  int maxWidth = (width > 0) ? width : 32;

  if (**str == '-') {
    isNegative = 1;
    (*str)++;
    (*countN)++;
  } else if (**str == '+') {
    (*str)++;
    (*countN)++;
  }

  if (**str == '0') {
    s21_sscanf_process_i_add_char_to_str(&numberStr, &count, str, countN);

    if (**str == 'x' || **str == 'X') {
      s21_sscanf_process_i_add_char_to_str(&numberStr, &count, str, countN);

      while (count < maxWidth &&
             (isdigit(**str) != 0 || (**str >= 'a' && **str <= 'f') ||
              (**str >= 'A' && **str <= 'F'))) {
        s21_sscanf_process_i_add_char_to_str(&numberStr, &count, str, countN);
      }
    } else {
      while (count < maxWidth && isdigit(**str) != 0 && **str >= '0' &&
             **str <= '7') {
        s21_sscanf_process_i_add_char_to_str(&numberStr, &count, str, countN);
      }
    }

    numberStr[count] = '\0';
  } else {
    s21_sscanf_extract_number(str, &numberStr, 0, countN, maxWidth);
  }

  *isNotEmpty = checkEmpty(numberStr);
  char *temp = numberStr;
  long int number = 0;

  if (*temp == '0') {
    temp++;

    if (*temp == 'x' || *temp == 'X') {
      temp++;
      number = s21_sscanf_hex_to_decimal(temp);

    } else {
      number = s21_sscanf_octal_to_decimal(temp);
    }
  } else {
    while (*temp) {
      number = number * 10 + (*temp - '0');
      temp++;
    }
  }

  if (isNegative == 1) {
    number = -number;
  }

  free(numberStr);
  return number;
}

void s21_sscanf_process_i_add_char_to_str(char **numberStr, int *count,
                                          const char **str, int *countN) {
  (*count)++;
  char *temp = realloc(*numberStr, (*count + 1) * sizeof(char));
  *numberStr = temp;
  (*numberStr)[*count - 1] = **str;
  (*countN)++;
  (*str)++;
}

long int s21_sscanf_hex_to_decimal(const char *hex) {
  long int number = 0;
  while (*hex) {
    if (*hex >= '0' && *hex <= '9') {
      number = number * 16 + (*hex - '0');
    } else if (*hex >= 'A' && *hex <= 'F') {
      number = number * 16 + (*hex - 'A' + 10);
    } else if (*hex >= 'a' && *hex <= 'f') {
      number = number * 16 + (*hex - 'a' + 10);
    }

    hex++;
  }
  return number;
}

long int s21_sscanf_octal_to_decimal(const char *octal) {
  long int number = 0;
  while (*octal) {
    if (*octal >= '0' && *octal <= '7') {
      number = number * 8 + (*octal - '0');
    }

    octal++;
  }
  return number;
}

int s21_sscanf_extract_number(const char **str, char **numberStr,
                              int *isNegative, int *countN, int widthMax) {
  int count = 0;

  while (count < widthMax && (isdigit(**str) != 0 ||
                              ((**str == '-' || **str == '+') && count == 0))) {
    if (**str == '-') {
      *isNegative = 1;
    } else if (**str != '+') {
      count++;
      char *temp = realloc(*numberStr, (count + 1) * sizeof(char));
      *numberStr = temp;
      (*numberStr)[count - 1] = **str;
    }

    (*countN)++;
    (*str)++;
  }

  (*numberStr)[count] = '\0';
  return count;
}

long double s21_sscanf_process_e(const char **str, int *countN, int width,
                                 int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int isNegative = 1;
  int count = 0;
  int maxWidth = (width > 0) ? width : 32;

  while (count < maxWidth &&
         (**str == '-' || isdigit(**str) != 0 || **str == 'e' || **str == 'E' ||
          ((**str == '+' || **str == '-') &&
           (numberStr[count - 1] == 'e' || numberStr[count - 1] == 'E')) ||
          **str == '.')) {
    if (**str == '-' && count == 0) {
      isNegative = -1;
    } else {
      count++;
      char *temp = realloc(numberStr, (count + 1) * sizeof(char));
      numberStr = temp;
      numberStr[count - 1] = **str;
    }

    (*countN)++;
    (*str)++;
  }

  numberStr[count] = '\0';
  *isNotEmpty = checkEmpty(numberStr);

  char *temp = numberStr;
  long double number = s21_sscanf_process_e_str_to_double(&temp);

  free(numberStr);
  return (long double)number * isNegative;
}

long double s21_sscanf_process_e_str_to_double(char **temp_ptr) {
  char *temp = *temp_ptr;
  long double number = 0;

  while (isdigit(*temp) != 0) {
    number = number * 10 + (*temp - '0');
    temp++;
  }

  if (*temp == '.') {
    temp++;
    long double step = 0.1;

    while (isdigit(*temp) != 0) {
      number += step * (*temp - '0');
      step *= 0.1;
      temp++;
    }
  }

  if (*temp == 'e' || *temp == 'E') {
    temp++;
    int isNegativeE = 0;

    if (*temp && *temp == '-') {
      isNegativeE = 1;
    }

    if (*temp != '\0') {
      temp++;
    }

    int degree = 0;

    while (isdigit(*temp) != 0) {
      degree = degree * 10 + (*temp - '0');
      temp++;
    }

    float power = 1.0;

    for (int i = 0; i < degree; i++) {
      power *= (isNegativeE ? 0.1 : 10.0);
    }

    number *= power;
  }

  return number;
}

long unsigned int s21_sscanf_process_o(const char **str, int *countN, int width,
                                       int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int isNegative = 1;
  int count = 0;
  int maxWidth = (width > 0) ? width : 32;

  while (
      count < maxWidth &&
      ((isdigit(**str) != 0 && **str >= '0' && **str <= '7') || **str == '-')) {
    if (**str == '-' && count == 0) {
      isNegative = -1;
    } else {
      count++;
      char *temp = realloc(numberStr, (count + 1) * sizeof(char));
      numberStr = temp;
      numberStr[count - 1] = **str;
    }

    (*countN)++;
    (*str)++;
  }

  numberStr[count] = '\0';
  *isNotEmpty = checkEmpty(numberStr);

  char *temp = numberStr;
  long unsigned int number =
      (long unsigned int)s21_sscanf_octal_to_decimal(temp);

  free(numberStr);
  return number * isNegative;
}

void s21_sscanf_process_s(const char **str, char *output, int *countN,
                          int width) {
  int count = 0;
  int maxWidth = (width > 0) ? width : 1024;

  while (count < maxWidth && isspace(**str) == 0 && **str != '\0') {
    output[count] = **str;
    count++;
    (*str)++;
    (*countN)++;
  }

  output[count] = '\0';
}

void s21_sscanf_process_ls(const char **str, wchar_t *output, int *countN,
                           int width) {
  int count = 0;
  int maxWidth = (width > 0) ? width : 1024;

  while (count < maxWidth && isspace(**str) == 0 && **str != '\0') {
    size_t bytes = mbtowc(&output[count], *str, MB_CUR_MAX);
    count++;
    (*str) += bytes;
    (*countN) += bytes;
  }

  output[count] = L'\0';
}

long unsigned int s21_sscanf_process_u(const char **str, int *countN, int width,
                                       int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int maxWidth = (width > 0) ? width : 32;
  int isNegative = 1;

  s21_sscanf_extract_number(str, &numberStr, &isNegative, countN, maxWidth);
  *isNotEmpty = checkEmpty(numberStr);

  long unsigned int number = 0;
  char *temp = numberStr;

  while (*temp) {
    number = number * 10 + (*temp - '0');
    temp++;
  }

  free(numberStr);
  return number * isNegative;
}

long unsigned int s21_sscanf_process_x(const char **str, int *countN, int width,
                                       int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int isNegative = 1;
  int count = 0;
  int maxWidth = (width > 0) ? width : 32;

  while (count < maxWidth &&
         (isdigit(**str) != 0 || (**str >= 'a' && **str <= 'f') ||
          (**str >= 'A' && **str <= 'F') || **str == '-' || (**str) == 'x' ||
          (**str) == 'X')) {
    if (**str == '-' && count == 0) {
      isNegative = -1;
    } else {
      count++;
      char *temp = realloc(numberStr, (count + 1) * sizeof(char));
      numberStr = temp;
      numberStr[count - 1] = **str;
    }

    (*countN)++;
    (*str)++;
  }

  numberStr[count] = '\0';
  *isNotEmpty = checkEmpty(numberStr);

  char *temp = numberStr;
  long unsigned int number = (long unsigned int)s21_sscanf_hex_to_decimal(temp);

  free(numberStr);
  return number * isNegative;
}

void s21_sscanf_process_p(const char **str, void **ptr, int *countN, int width,
                          int *isNotEmpty) {
  char *numberStr = malloc(1 * sizeof(char));
  int count = 0;
  int maxWidth = (width > 0) ? width : 32;

  while (count < maxWidth &&
         (isdigit(**str) != 0 || (**str >= 'a' && **str <= 'f') ||
          (**str >= 'A' && **str <= 'F') || (**str) == 'x' || (**str) == 'X')) {
    count++;
    char *temp = realloc(numberStr, (count + 1) * sizeof(char));
    numberStr = temp;
    numberStr[count - 1] = **str;
    (*str)++;
    (*countN)++;
  }

  numberStr[count] = '\0';
  *isNotEmpty = checkEmpty(numberStr);
  unsigned long long addr = 0;
  int i = 0;

  for (; numberStr[i] != '\0'; i++) {
    char c = numberStr[i];
    addr <<= 4;

    if (c >= '0' && c <= '9') {
      addr |= (c - '0');
    } else if (c >= 'a' && c <= 'f') {
      addr |= (10 + (c - 'a'));
    } else if (c >= 'A' && c <= 'F') {
      addr |= (10 + (c - 'A'));
    }
  }

  *ptr = (void *)(uintptr_t)addr;
  free(numberStr);
}