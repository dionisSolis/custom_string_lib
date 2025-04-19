#include "s21_string.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
static char *errorMessages[] = {"Success",
                                "Operation not permitted",
                                "No such file or directory",
                                "No such process",
                                "Interrupted system call",
                                "Input/output error",
                                "No such device or address",
                                "Argument list too long",
                                "Exec format error",
                                "Bad file descriptor",
                                "No child processes",
                                "Resource temporarily unavailable",
                                "Cannot allocate memory",
                                "Permission denied",
                                "Bad address",
                                "Block device required",
                                "Device or resource busy",
                                "File exists",
                                "Invalid cross-device link",
                                "No such device",
                                "Not a directory",
                                "Is a directory",
                                "Invalid argument",
                                "Too many open files in system",
                                "Too many open files",
                                "Inappropriate ioctl for device",
                                "Text file busy",
                                "File too large",
                                "No space left on device",
                                "Illegal seek",
                                "Read-only file system",
                                "Too many links",
                                "Broken pipe",
                                "Numerical argument out of domain",
                                "Numerical result out of range",
                                "Resource deadlock avoided",
                                "File name too long",
                                "No locks available",
                                "Function not implemented",
                                "Directory not empty",
                                "Too many levels of symbolic links",
                                "Unknown error 41",
                                "No message of desired type",
                                "Identifier removed",
                                "Unknown error 44",
                                "Unknown error 45",
                                "Unknown error 46",
                                "Unknown error 47",
                                "Unknown error 48",
                                "Unknown error 49",
                                "Unknown error 50",
                                "Unknown error 51",
                                "Unknown error 52",
                                "Unknown error 53",
                                "Unknown error 54",
                                "Unknown error 55",
                                "Unknown error 56",
                                "Unknown error 57",
                                "Unknown error 58",
                                "Unknown error 59",
                                "Unknown error 60",
                                "Connection refused",
                                "Too many levels of symbolic links",
                                "File name too long",
                                "Host is down",
                                "No route to host",
                                "Directory not empty",
                                "Too many processes",
                                "Too many users",
                                "Disc quota exceeded",
                                "Stale NFS file handle",
                                "Unknown error 71",
                                "Unknown error 72",
                                "Unknown error 73",
                                "Unknown error 74",
                                "Unknown error 75",
                                "Unknown error 76",
                                "No locks available",
                                "Function not implemented",
                                "Unknown error 79",
                                "Unknown error 80",
                                "Unknown error 81",
                                "Unknown error 82",
                                "Unknown error 83",
                                "Value too large for defined data type",
                                "Unknown error 85",
                                "Unknown error 86",
                                "Unknown error 87",
                                "Unknown error 88",
                                "Operation canceled",
                                "Identifier removed",
                                "No message of desired type",
                                "Illegal byte sequence",
                                "Unknown error 93",
                                "Bad message",
                                "Multihop attempted",
                                "No data available",
                                "Link has been severed",
                                "Out of streams resources",
                                "Device not a stream",
                                "Protocol error",
                                "Timer expired",
                                "Operation not supported",
                                "Unknown error 103",
                                "State not recoverable",
                                "Owner died",
                                "Unknown error 106",
                                "Unknown error 107",
                                "Unknown error 108",
                                "Unknown error 109",
                                "Unknown error 110",
                                "Unknown error 111",
                                "Unknown error 112",
                                "Unknown error 113",
                                "Unknown error 114",
                                "Unknown error 115",
                                "Unknown error 116",
                                "Unknown error 117",
                                "Unknown error 118",
                                "Unknown error 119",
                                "Unknown error 120",
                                "Unknown error 121",
                                "Required key not available",
                                "Key has expired",
                                "Key was rejected by service",
                                "Service unavailable",
                                "No medium found",
                                "Wrong medium type",
                                "Unknown error 128",
                                "Unknown error 129",
                                "Unknown error 130",
                                "Previous owner died",
                                "State not recoverable"};
#define S21_MAX_ERROR (sizeof(errorMessages) / sizeof(errorMessages[0]))
#elif __APPLE__

static char *errorMessages[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource deadlock avoided",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Device / Resource busy",
    "File exists",
    "Cross-device link",
    "Operation not supported by device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Result too large",
    "Resource temporarily unavailable",
    "Operation now in progress",
    "Operation already in progress",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol family",
    "Address already in use",
    "Can't assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Socket is already connected",
    "Socket is not connected",
    "Can't send after socket shutdown",
    "Too many references: can't splice",
    "Operation timed out",
    "Connection refused",
    "Too many levels of symbolic links",
    "File name too long",
    "Host is down",
    "No route to host",
    "Directory not empty",
    "Too many processes",
    "Too many users",
    "Disc quota exceeded",
    "Stale NFS file handle",
    "Too many levels of remote in path",
    "RPC struct is bad",
    "RPC version wrong",
    "RPC prog. not avail",
    "Program version wrong",
    "Bad procedure for program",
    "No locks available",
    "Function not implemented",
    "Inappropriate file type or format",
    "Authentication error",
    "Need authenticator",
    "Device power is off",
    "Device error, e.g. paper out",
    "Value too large to be stored in data type",
    "Bad executable (or shared library)",
    "Bad CPU type in executable",
    "Shared library version mismatch",
    "Malformed Mach-o file",
    "Operation canceled",
    "Identifier removed",
    "No message of desired type",
    "Illegal byte sequence",
    "Attribute not found",
    "Bad message",
    "Reserved",
    "No message available on STREAM",
    "Reserved",
    "No STREAM resources",
    "Not a STREAM",
    "Protocol error",
    "STREAM ioctl timeout",
    "Operation not supported on socket",
    "Policy not found",
    "State not recoverable",
    "Previous owner died",
    "Interface output queue is full"};
#define S21_MAX_ERROR (sizeof(errorMessages) / sizeof(errorMessages[0]))
#else

static char *errorMessages[] = {"Unknown error"};
#define S21_MAX_ERROR 1
#endif

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  if (str == S21_NULL) return 0;
  for (; *(str + len); len++);
  return len;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  if (n == 0) return 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (str1[i] != str2[i] || str1[i] == '\0') {
      return (unsigned char)str1[i] - (unsigned char)str2[i];
    }
  }

  return 0;
}

char *s21_strncpy(char *destination, const char *source, s21_size_t n) {
  s21_size_t i;
  for (i = 0; i < n && source[i] != '\0'; i++) {
    destination[i] = source[i];
  }

  for (; i < n; i++) {
    destination[i] = '\0';
  }

  return destination;
}

char *s21_strncat(char *destination, const char *src, s21_size_t n) {
  char *dest_ptr = destination;

  while (*dest_ptr != '\0') {
    dest_ptr++;
  }

  s21_size_t i = 0;

  while (src[i] != '\0' && i < n) {
    *dest_ptr = src[i];
    dest_ptr++;
    i++;
  }

  *dest_ptr = '\0';
  return destination;
}

char *s21_strchr(const char *str, int c) {
  if (str == S21_NULL) return S21_NULL;

  char ch = (char)c;
  while (*str != '\0') {
    if (*str == ch) {
      return (char *)str;
    }
    str++;
  }

  return (ch == '\0') ? (char *)str : S21_NULL;
}

char *s21_strstr(const char *haystack, const char *needle) {
  if (haystack == S21_NULL) return S21_NULL;
  if (needle == S21_NULL) return (char *)haystack;
  if (*needle == '\0') return (char *)haystack;

  const char *h = haystack;
  const char *n = needle;

  while (*haystack != '\0') {
    h = haystack;
    n = needle;

    while (*h != '\0' && *n != '\0' && *h == *n) {
      h++;
      n++;
    }

    if (*n == '\0') {
      return (char *)haystack;
    }

    haystack++;
  }

  return S21_NULL;
}

char *s21_strtok(char *str, const char *delim) {
  static char *last_token = S21_NULL;
  char *current_token_start;

  if (str != S21_NULL) {
    last_token = str;
  }

  if (last_token == S21_NULL || *last_token == '\0') {
    return S21_NULL;
  }

  while (*last_token != '\0' && s21_strchr(delim, *last_token) != S21_NULL) {
    last_token++;
  }

  if (*last_token == '\0') {
    last_token = S21_NULL;
    return S21_NULL;
  }

  current_token_start = last_token;

  while (*last_token != '\0' && s21_strchr(delim, *last_token) == S21_NULL) {
    last_token++;
  }

  if (*last_token != '\0') {
    *last_token = '\0';
    last_token++;
  } else {
    last_token = S21_NULL;
  }

  return current_token_start;
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
  if (str == S21_NULL || n == 0) return S21_NULL;

  const unsigned char *ptr = (const unsigned char *)str;
  unsigned char uc = (unsigned char)c;

  for (s21_size_t i = 0; i < n; i++) {
    if (ptr[i] == uc) {
      return (void *)(ptr + i);
    }
  }

  return S21_NULL;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  if (n == 0) return 0;

  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;

  for (s21_size_t i = 0; i < n; i++) {
    if (s1[i] != s2[i]) {
      return s1[i] - s2[i];
    }
  }

  return 0;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  if (dest == S21_NULL || src == S21_NULL || n == 0) return dest;

  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  if (str == S21_NULL || n == 0) return str;

  unsigned char *ptr = (unsigned char *)str;
  unsigned char uc = (unsigned char)c;

  for (s21_size_t i = 0; i < n; i++) {
    ptr[i] = uc;
  }
  return str;
}

char *s21_strerror(int errnum) {
  static char unknown_error_msg[50];

  if (errnum < 0 || (long unsigned int)errnum >= S21_MAX_ERROR) {
    snprintf(unknown_error_msg, sizeof(unknown_error_msg), "Unknown error %d",
             errnum);
    return unknown_error_msg;
  } else {
    return errorMessages[errnum];
  }
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  if (start_index > src_len) {
    return S21_NULL;
  }

  s21_size_t total_len = src_len + str_len;
  char *result = (char *)malloc((total_len + 1) * sizeof(char));
  if (result == S21_NULL) {
    return S21_NULL;
  }

  s21_memcpy(result, src, start_index);
  s21_memcpy(result + start_index, str, str_len);
  s21_memcpy(result + start_index + str_len, src + start_index,
             src_len - start_index);

  result[total_len] = '\0';

  return (void *)result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL) return S21_NULL;
  if (trim_chars == S21_NULL || *trim_chars == '\0') {
    s21_size_t src_len = s21_strlen(src);
    char *result = (char *)malloc(src_len + 1);
    if (result == S21_NULL) return S21_NULL;
    s21_memcpy(result, src, src_len + 1);
    return result;
  }

  const char *start = src;
  const char *end = src + s21_strlen(src);

  while (start < end && s21_strchr(trim_chars, *start) != S21_NULL) {
    start++;
  }

  while (end > start && s21_strchr(trim_chars, *(end - 1)) != S21_NULL) {
    end--;
  }

  s21_size_t result_len = end - start;

  char *result = (char *)malloc(result_len + 1);
  if (result == S21_NULL) {
    return S21_NULL;
  }

  s21_memcpy(result, start, result_len);

  result[result_len] = '\0';

  return (void *)result;
}

char *s21_str_cpy(char *dest, const char *src) {
  if (dest == S21_NULL || src == S21_NULL) return dest;
  char *start = dest;
  while ((*dest++ = *src++) != '\0');
  return start;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *a1;
  const char *a2;

  for (a1 = str1; *a1 != '\0'; a1++) {
    for (a2 = str2; *a2 != '\0'; a2++) {
      if (*a1 == *a2) return a1 - str1;
    }
  }
  return a1 - str1;
}

char *s21_strrchr(const char *str, int c) {
  char *last_inside = NULL;
  while (*str != '\0') {
    if (*str == (char)c) last_inside = (char *)str;
    str++;
  }
  return last_inside;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  if (str1 == NULL || (str2 == NULL && *str1 == '\0')) {
    return NULL;
  }

  while (*str1) {
    const char *temp = str2;
    while (*temp) {
      if (*str1 == *temp) {
        return (char *)str1;
      }
      temp++;
    }
    str1++;
  }

  return NULL;
}

void *s21_to_upper(const char *str) {
  if (str == NULL) {
    return NULL;
  }

  s21_size_t len = s21_strlen(str);
  char *upper_str = (char *)malloc(len + 1);
  if (upper_str == NULL) {
    return NULL;
  }
  for (s21_size_t i = 0; i < len; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      upper_str[i] = str[i] - 32;

    } else
      upper_str[i] = str[i];
  }
  upper_str[len] = '\0';

  return upper_str;
}

void *s21_to_lower(const char *str) {
  if (str == NULL) {
    return NULL;
  }

  s21_size_t len = s21_strlen(str);
  char *lower_str = (char *)malloc(len + 1);
  if (lower_str == NULL) {
    return NULL;
  }
  for (s21_size_t i = 0; i < len; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      lower_str[i] = str[i] + 32;

    } else
      lower_str[i] = str[i];
  }
  lower_str[len] = '\0';

  return lower_str;
}