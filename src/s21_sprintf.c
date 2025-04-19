#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

typedef struct {
  int left_align;
  int force_sign;
  int space_sign;
  int zero_padding;
  int width;
  int length;
  int precision;
  int alternate_form;
  char specifier;
} FormatSpec;

void s21_process_d(char *buffer, long num);
void s21_process_f(char *buffer, long double num, FormatSpec spec);
void s21_process_u(char *buffer, unsigned long num, FormatSpec spec);
void s21_process_x(char *buffer, unsigned long num, FormatSpec spec,
                   int uppercase);
void s21_process_e(char *buffer, long double num, FormatSpec spec);
void s21_process_g(char *buffer, long double num, FormatSpec spec);
void s21_process_o(char *buffer, unsigned long num, FormatSpec spec);
void s21_process_p(char *buffer, void *ptr);
void s21_apply_width(char **dest, const char *buffer, FormatSpec spec);
void s21_handle_specifier(char **s, FormatSpec spec, va_list *input);
void s21_format_mantissa(char **buf, long double rounded,
                         long double multiplier, int precision);
void s21_format_exponent(char **buf, int exponent, char exp_char);
FormatSpec s21_parse_format(const char **format, va_list *input);

int s21_sprintf(char *str, const char *format, ...) {
  va_list input;
  va_start(input, format);
  char *s = str;

  while (*format) {
    if (*format == '%') {
      format++;
      FormatSpec spec = s21_parse_format(&format, &input);
      s21_handle_specifier(&s, spec, &input);

    } else {
      *s++ = *format;
    }
    format++;
  }

  *s = '\0';
  va_end(input);
  return s - str;
}

void s21_handle_specifier(char **s, FormatSpec spec, va_list *input) {
  const size_t buffer_size = 4096;
  char *buffer = (char *)malloc(buffer_size);

  if (buffer == NULL) {
    return;
  }

  s21_memset(buffer, 0, buffer_size);

  switch (spec.specifier) {
    case 'c':
      *(*s)++ = (char)va_arg(*input, int);
      break;
    case 'd': {
      long d;
      if (spec.length == 'h')
        d = (short)va_arg(*input, int);
      else if (spec.length == 'l')
        d = va_arg(*input, long);
      else
        d = va_arg(*input, int);

      if (d < 0) {
        *(*s)++ = '-';
        d = -d;
      } else if (spec.force_sign) {
        *(*s)++ = '+';
      } else if (spec.space_sign) {
        *(*s)++ = ' ';
      }
      s21_process_d(buffer, d);
      s21_apply_width(s, buffer, spec);
      break;
    }
    case 's': {
      char *string = va_arg(*input, char *);
      s21_apply_width(s, string, spec);
      break;
    }
    case 'f': {
      long double f;
      if (spec.length == 'L')
        f = va_arg(*input, long double);
      else
        f = va_arg(*input, double);

      s21_process_f(buffer, f, spec);
      s21_apply_width(s, buffer, spec);
      break;
    }
    case 'u': {
      unsigned long u;
      if (spec.length == 'h')
        u = (unsigned short)va_arg(*input, unsigned int);
      else if (spec.length == 'l')
        u = va_arg(*input, unsigned long);
      else
        u = va_arg(*input, unsigned int);

      s21_process_u(buffer, u, spec);
      s21_apply_width(s, buffer, spec);
      break;
    }

    case 'x':
    case 'X':
      unsigned long x;
      if (spec.length == 'h')
        x = (unsigned short)va_arg(*input, unsigned int);
      else if (spec.length == 'l')
        x = va_arg(*input, unsigned long);
      else
        x = va_arg(*input, unsigned int);

      s21_process_x(buffer, x, spec, spec.specifier == 'X');
      s21_apply_width(s, buffer, spec);
      break;

    case 'e':
    case 'E':
      long double f;
      if (spec.length == 'L')
        f = va_arg(*input, long double);
      else
        f = va_arg(*input, double);

      s21_process_e(buffer, f, spec);
      s21_apply_width(s, buffer, spec);
      break;

    case 'g':
    case 'G':
      long double g;
      if (spec.length == 'L')
        g = va_arg(*input, long double);
      else
        g = va_arg(*input, double);

      s21_process_g(buffer, g, spec);
      s21_apply_width(s, buffer, spec);
      break;

    case 'o':
      unsigned int o;
      if (spec.length == 'h')
        o = (unsigned short)va_arg(*input, unsigned int);
      else if (spec.length == 'l')
        o = va_arg(*input, unsigned long);
      else
        o = va_arg(*input, unsigned int);

      s21_process_o(buffer, o, spec);
      s21_apply_width(s, buffer, spec);
      break;

    case 'p':
      void *ptr = va_arg(*input, void *);
      s21_process_p(buffer, ptr);
      s21_apply_width(s, buffer, spec);
      break;

    default:
      *(*s)++ = '%';
      break;
  }
  free(buffer);
}

FormatSpec s21_parse_format(const char **format, va_list *input) {
  FormatSpec spec = {0};
  while (**format == ' ' || **format == '-' || **format == '+' ||
         **format == '0' || **format == '#') {
    if (**format == '-')
      spec.left_align = 1;
    else if (**format == '+')
      spec.force_sign = 1;
    else if (**format == ' ')
      spec.space_sign = 1;
    else if (**format == '0' && !spec.left_align)
      spec.zero_padding = 1;
    else if (**format == '#')
      spec.alternate_form = 1;
    (*format)++;
  }

  if (**format == 'h' || **format == 'l' || **format == 'L') {
    spec.length = **format;
    (*format)++;
  }

  if (**format == '*') {
    spec.width = va_arg(*input, int);
    (*format)++;
  } else {
    while (isdigit(**format)) {
      spec.width = spec.width * 10 + (**format - '0');
      (*format)++;
    }
  }

  if (**format == '.') {
    (*format)++;
    if (**format == '*') {
      spec.precision = va_arg(*input, int);
      (*format)++;
    } else {
      while (isdigit(**format)) {
        spec.precision = spec.precision * 10 + (**format - '0');
        (*format)++;
      }
    }
  } else {
    spec.precision = -1;
  }

  spec.specifier = **format;
  return spec;
}

void s21_process_d(char *buffer, long num) {
  int i = 0;

  int is_negative = num < 0;
  if (is_negative) num = -num;
  if (num == 0) buffer[i++] = '0';

  while (num > 0) {
    buffer[i++] = num % 10 + '0';
    num /= 10;
  }

  buffer[i] = '\0';
  for (int j = 0; j < i / 2; j++) {
    char temp = buffer[j];
    buffer[j] = buffer[i - 1 - j];
    buffer[i - 1 - j] = temp;
  }
}

void s21_process_f(char *buffer, long double num, FormatSpec spec) {
  int int_part = (int)num;
  long double fract_part = fabsl(num - int_part);

  if (num < 0)
    *buffer++ = '-';
  else if (spec.force_sign)
    *buffer++ = '+';
  else if (spec.space_sign)
    *buffer++ = ' ';

  s21_process_d(buffer, abs(int_part));
  while (*buffer) buffer++;
  if (spec.precision != 0 || spec.alternate_form) *buffer++ = '.';

  if (spec.precision == -1) spec.precision = 6;
  if (spec.precision > 0) {
    fract_part = roundl(fract_part * powl(10, spec.precision));
    char fract_str[50];
    int fract_len = 0;
    if (fract_part == 0) {
      fract_str[fract_len++] = '0';
    } else {
      while (fract_part >= 1) {
        fract_str[fract_len++] = (long long)fmodl(fract_part, 10) + '0';
        fract_part = floorl(fract_part / 10);
      }
    }

    while (fract_len < spec.precision) {
      fract_str[fract_len++] = '0';
    }

    for (int k = fract_len - 1; k >= 0; k--) {
      *buffer++ = fract_str[k];
    }
  }
  *buffer = '\0';
}

void s21_process_u(char *buffer, unsigned long num, FormatSpec spec) {
  int i = 0;

  if (num == 0 && spec.precision != 0) buffer[i++] = '0';
  while (num > 0) {
    buffer[i++] = num % 10 + '0';
    num /= 10;
  }
  buffer[i] = '\0';

  for (int j = 0; j < i / 2; j++) {
    char temp = buffer[j];
    buffer[j] = buffer[i - 1 - j];
    buffer[i - 1 - j] = temp;
  }
}

void s21_process_x(char *buffer, unsigned long num, FormatSpec spec,
                   int uppercase) {
  int i = 0;
  unsigned long save_num = num;

  if (num == 0) {
    if (spec.precision != 0) {
      buffer[i++] = '0';
    }
    buffer[i] = '\0';
    return;
  }

  while (num > 0) {
    int digit = num % 16;
    if (digit < 10) {
      buffer[i++] = digit + '0';
    } else {
      buffer[i++] = (uppercase ? 'A' : 'a') + digit - 10;
    }
    num /= 16;
  }

  if (spec.alternate_form && save_num != 0) {
    buffer[i++] = (uppercase ? 'X' : 'x');
    buffer[i++] = '0';
  }

  buffer[i] = '\0';

  for (int j = 0; j < i / 2; j++) {
    char tmp = buffer[j];
    buffer[j] = buffer[i - 1 - j];
    buffer[i - 1 - j] = tmp;
  }
}

void s21_process_e(char *buffer, long double num, FormatSpec spec) {
  char *buf = buffer;

  int sign = 1;
  if (num < 0) {
    sign = -1;
    num = -num;
  } else if (spec.force_sign) {
    *buf++ = '+';
  } else if (spec.space_sign) {
    *buf++ = ' ';
  }
  if (sign == -1) *buf++ = '-';

  if (spec.precision < 0) spec.precision = 6;

  if (num == 0.0L) {
    *buf++ = '0';
    if (spec.precision > 0 || spec.alternate_form) {
      *buf++ = '.';
      for (int i = 0; i < spec.precision; i++) *buf++ = '0';
    }
    s21_format_exponent(&buf, 0, (spec.specifier == 'E' ? 'E' : 'e'));
    *buf = '\0';
    return;
  }

  long double norm = num;
  int exponent = 0;
  if (num > 0) {
    while (norm >= 10.0L) {
      norm /= 10.0L;
      exponent++;
    }
    while (norm < 1.0L) {
      norm *= 10.0L;
      exponent--;
    }
  }

  long double multiplier = powl(10.0L, spec.precision);
  long double rounded = roundl(norm * multiplier);

  if (roundl(norm * multiplier) >= 10 * multiplier) {
    rounded /= 10.0L;
    exponent++;
  }

  s21_format_mantissa(&buf, rounded, multiplier, spec.precision);
  if (spec.alternate_form && spec.precision == 0 &&
      s21_strchr(buffer, '.') == NULL) {
    *buf++ = '.';
  }
  s21_format_exponent(&buf, exponent, (spec.specifier == 'E' ? 'E' : 'e'));
  *buf = '\0';
}

void s21_process_g(char *buffer, long double num, FormatSpec spec) {
  int is_negative = 0;
  if (num < 0) {
    is_negative = 1;
    num = -num;
  }

  if (spec.precision < 0) spec.precision = 6;
  if (spec.precision == 0) spec.precision = 1;

  if (num == 0.0L) {
    if (is_negative)
      *buffer++ = '-';
    else if (spec.force_sign)
      *buffer++ = '+';
    else if (spec.space_sign)
      *buffer++ = ' ';
    *buffer++ = '0';
    if (spec.alternate_form && spec.precision > 0) {
      *buffer++ = '.';
      for (int i = 0; i < spec.precision - 1; i++) *buffer++ = '0';
    }
    *buffer = '\0';
    return;
  }

  int exponent = 0;
  long double norm = num;

  if (num > 0) {
    if (num >= 1.0L) {
      while (norm >= 10.0L) {
        norm /= 10.0L;
        exponent++;
      }
    } else {
      while (norm < 1.0L) {
        norm *= 10.0L;
        exponent--;
      }
    }
  }

  FormatSpec temp_spec = spec;
  char *start_buf = buffer;

  if (exponent < -4 || exponent >= spec.precision) {
    temp_spec.specifier = (spec.specifier == 'G') ? 'E' : 'e';
    temp_spec.precision = spec.precision - 1;
    s21_process_e(buffer, (is_negative ? -num : num), temp_spec);
  } else {
    temp_spec.specifier = 'f';
    temp_spec.precision = spec.precision - (exponent + 1);
    s21_process_f(buffer, (is_negative ? -num : num), temp_spec);
  }

  if (!spec.alternate_form) {
    char *dot = s21_strchr(start_buf, '.');
    if (dot) {
      char *end = start_buf + s21_strlen(start_buf) - 1;
      char *exp_char =
          s21_strchr(start_buf, (spec.specifier == 'G' ? 'E' : 'e'));
      if (exp_char) end = exp_char - 1;

      while (end > dot && *end == '0') {
        *end = '\0';
        end--;
      }
      if (end == dot) *end = '\0';
    }
  }
}

void s21_process_o(char *buffer, unsigned long num, FormatSpec spec) {
  int i = 0;
  unsigned long save_num = num;

  if (num == 0) {
    if (spec.precision != 0) {
      buffer[i++] = '0';
    }
    buffer[i] = '\0';
    return;
  }

  while (num > 0) {
    buffer[i++] = num % 8 + '0';
    num /= 8;
  }

  if (spec.alternate_form && save_num != 0) {
    buffer[i++] = '0';
  }

  buffer[i] = '\0';

  for (int j = 0; j < i / 2; j++) {
    char temp = buffer[j];
    buffer[j] = buffer[i - 1 - j];
    buffer[i - 1 - j] = temp;
  }
}

void s21_process_p(char *buffer, void *ptr) {
  unsigned long addr = (unsigned long)ptr;

  if (addr == 0) {
    s21_str_cpy(buffer, "(nil)");
  } else {
    *buffer++ = '0';
    *buffer++ = 'x';
    char temp[50];
    int len = 0;
    int num;
    while (addr > 0) {
      num = addr % 16;
      temp[len++] = "0123456789abcdef"[num];
      addr /= 16;
    }
    for (int i = len - 1; i >= 0; i--) {
      *buffer++ = temp[i];
    }
    *buffer = '\0';
  }
}

void s21_format_mantissa(char **buf, long double rounded,
                         long double multiplier, int precision) {
  int int_part = (int)(rounded / multiplier);
  *(*buf)++ = '0' + int_part;
  if (precision > 0) {
    *(*buf)++ = '.';
    long double frac_part = fmodl(rounded, multiplier);
    long long frac_part_int = (long long)(frac_part + 0.5L);
    char frac_str[32];
    s21_memset(frac_str, '0', precision);
    frac_str[precision] = '\0';
    int current_pos = precision - 1;
    while (frac_part_int > 0 && current_pos >= 0) {
      frac_str[current_pos--] = '0' + (frac_part_int % 10);
      frac_part_int /= 10;
    }
    for (int i = 0; i < precision; i++) *(*buf)++ = frac_str[i];
  }
}

void s21_format_exponent(char **buf, int exponent, char exp_char) {
  *(*buf)++ = exp_char;
  if (exponent >= 0)
    *(*buf)++ = '+';
  else {
    *(*buf)++ = '-';
    exponent = -exponent;
  }

  if (exponent < 10) *(*buf)++ = '0';
  char exp_str[5];
  int exp_len = 0;
  if (exponent == 0) {
    exp_str[exp_len++] = '0';
  } else {
    while (exponent > 0) {
      exp_str[exp_len++] = exponent % 10 + '0';
      exponent /= 10;
    }
  }
  for (int i = exp_len - 1; i >= 0; i--) {
    *(*buf)++ = exp_str[i];
  }
}

void s21_apply_width(char **dest, const char *buffer, FormatSpec spec) {
  int length = s21_strlen(buffer);
  int precision_padding = 0;
  int width_padding = 0;
  int original_length = length;

  if (spec.specifier == 's' && spec.precision >= 0 && spec.precision < length) {
    length = spec.precision;
  }

  if ((spec.specifier == 'd' || spec.specifier == 'i' ||
       spec.specifier == 'u' || spec.specifier == 'o' ||
       spec.specifier == 'x' || spec.specifier == 'X' ||
       spec.specifier == 'p') &&
      spec.precision > original_length) {
    int has_sign = (buffer[0] == '+' || buffer[0] == '-');
    int prefix_len = 0;
    if (spec.alternate_form &&
        (spec.specifier == 'x' || spec.specifier == 'X') &&
        s21_strstr(buffer, "0x") == buffer)
      prefix_len = 2;
    if (spec.alternate_form && spec.specifier == 'o' && buffer[0] == '0')
      prefix_len = 1;
    if (spec.specifier == 'p' && s21_strstr(buffer, "0x") == buffer)
      prefix_len = 2;

    precision_padding =
        spec.precision - (original_length - (has_sign ? 1 : 0) - prefix_len);
    if (precision_padding < 0) precision_padding = 0;
    length += precision_padding;
  }

  if (spec.width > length) {
    width_padding = spec.width - length;
  }

  int sign_or_space_present =
      (buffer[0] == '+' || buffer[0] == '-' || buffer[0] == ' ');
  int has_prefix = ((spec.alternate_form &&
                     (spec.specifier == 'x' || spec.specifier == 'X' ||
                      spec.specifier == 'o') &&
                     original_length > 0) ||
                    spec.specifier == 'p');
  int prefix_len = 0;
  if (spec.specifier == 'p' && s21_strncmp(buffer, "0x", 2) == 0)
    prefix_len = 2;
  else if (spec.alternate_form &&
           (spec.specifier == 'x' || spec.specifier == 'X') &&
           s21_strncmp(buffer + (sign_or_space_present ? 1 : 0), "0x", 2) == 0)
    prefix_len = 2;
  else if (spec.alternate_form && spec.specifier == 'o' &&
           buffer[sign_or_space_present ? 1 : 0] == '0')
    prefix_len = 1;

  if (!spec.left_align && spec.zero_padding && spec.precision < 0 &&
      !sign_or_space_present && !has_prefix) {
    char pad = '0';
    while (width_padding--) {
      **dest = pad;
      (*dest)++;
    }
    width_padding = 0;
  }

  if (sign_or_space_present && spec.zero_padding && spec.precision < 0) {
    **dest = buffer[0];
    (*dest)++;
    buffer++;
    length--;
    original_length--;
  }

  if (has_prefix && spec.zero_padding && spec.precision < 0) {
    for (int i = 0; i < prefix_len; ++i) {
      **dest = buffer[i];
      (*dest)++;
    }
    buffer += prefix_len;
    length -= prefix_len;
    original_length -= prefix_len;
  }

  if (!spec.left_align) {
    char pad = (spec.zero_padding && spec.precision < 0) ? '0' : ' ';
    while (width_padding--) {
      **dest = pad;
      (*dest)++;
    }
  }

  if (sign_or_space_present && (!spec.zero_padding || spec.precision >= 0)) {
    **dest = buffer[0];
    (*dest)++;
    buffer++;
    length--;
    original_length--;
  }
  if (has_prefix && (!spec.zero_padding || spec.precision >= 0)) {
    for (int i = 0; i < prefix_len; ++i) {
      **dest = buffer[i];
      (*dest)++;
    }
    buffer += prefix_len;
    length -= prefix_len;
    original_length -= prefix_len;
  }

  while (precision_padding--) {
    **dest = '0';
    (*dest)++;
  }

  for (int i = 0;
       i < (spec.specifier == 's' && spec.precision >= 0 ? spec.precision
                                                         : original_length);
       i++) {
    if (i < original_length) {
      **dest = buffer[i];
      (*dest)++;
    } else {
      break;
    }
  }

  if (spec.left_align) {
    while (width_padding--) {
      **dest = ' ';
      (*dest)++;
    }
  }
}