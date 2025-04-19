#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"
#include "../scanf/s21_scanf.h"

#define S21_MAX_ERROR 134

START_TEST(test_strlen) {
  ck_assert_uint_eq(s21_strlen(""), strlen(""));
  ck_assert_uint_eq(s21_strlen("Hello"), strlen("Hello"));
  ck_assert_uint_eq(s21_strlen("1234567890"), strlen("1234567890"));
  char long_str[1000];
  memset(long_str, 'a', 999);
  long_str[999] = '\0';
  ck_assert_uint_eq(s21_strlen(long_str), strlen(long_str));
  ck_assert_uint_eq(s21_strlen("\0hidden"), strlen("\0hidden"));
  ck_assert_uint_eq(s21_strlen("Test\0Test"), strlen("Test\0Test"));
}
END_TEST

START_TEST(test_strncmp) {
  char s1[] = "Hello World";
  char s2[] = "Hello There";
  char s3[] = "Hello World";
  char s4[] = "Hello";

  ck_assert_int_eq(s21_strncmp(s1, s2, 5), strncmp(s1, s2, 5));
  ck_assert_int_eq(s21_strncmp(s1, s2, 6), strncmp(s1, s2, 6));

  ck_assert_int_eq(s21_strncmp(s1, s3, 12), strncmp(s1, s3, 12));
  ck_assert_int_eq(s21_strncmp(s1, s3, 5), strncmp(s1, s3, 5));
  ck_assert_int_eq(s21_strncmp(s1, s3, 0), strncmp(s1, s3, 0));

  ck_assert_int_eq(s21_strncmp(s1, s4, 5), strncmp(s1, s4, 5));

  ck_assert_int_eq(s21_strncmp("abc", "ABC", 3), strncmp("abc", "ABC", 3));
  ck_assert_int_eq(s21_strncmp("ABC", "abc", 3), strncmp("ABC", "abc", 3));
  ck_assert_int_eq(s21_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
  ck_assert_int_eq(s21_strncmp("abd", "abc", 3), strncmp("abd", "abc", 3));

  ck_assert(s21_strncmp("a", "b", 1) < 0);
  ck_assert(strncmp("a", "b", 1) < 0);
  ck_assert(s21_strncmp("b", "a", 1) > 0);
  ck_assert(strncmp("b", "a", 1) > 0);
}
END_TEST

START_TEST(test_strncpy) {
  char s21_dest[100];
  char std_dest[100];
  char src[] = "1234567890";

  memset(s21_dest, 'X', 99);
  s21_dest[99] = '\0';
  memset(std_dest, 'X', 99);
  std_dest[99] = '\0';
  s21_strncpy(s21_dest, src, 5);
  strncpy(std_dest, src, 5);
  ck_assert_mem_eq(s21_dest, std_dest, 10);

  memset(s21_dest, 'X', 99);
  s21_dest[99] = '\0';
  s21_strncpy(s21_dest, "ABC", 5);
  ck_assert_str_eq(s21_dest, "ABC");
  ck_assert_int_eq(s21_dest[3], '\0');
  ck_assert_int_eq(s21_dest[4], '\0');
  ck_assert_int_eq(s21_dest[5], 'X');

  memset(s21_dest, 'X', 99);
  s21_dest[99] = '\0';
  s21_strncpy(s21_dest, "copy", 4);
  ck_assert_int_eq(s21_dest[0], 'c');
  ck_assert_int_eq(s21_dest[1], 'o');
  ck_assert_int_eq(s21_dest[2], 'p');
  ck_assert_int_eq(s21_dest[3], 'y');

  memset(s21_dest, 'X', 99);
  s21_dest[99] = '\0';
  memset(std_dest, 'X', 99);
  std_dest[99] = '\0';
  s21_strncpy(s21_dest, "short", 10);
  strncpy(std_dest, "short", 10);
  ck_assert_str_eq(s21_dest, "short");
  ck_assert_mem_eq(s21_dest, std_dest, 10);
  ck_assert_int_eq(s21_dest[5], '\0');
  ck_assert_int_eq(s21_dest[9], '\0');
  ck_assert_int_eq(s21_dest[10], 'X');

  memset(s21_dest, 'X', 99);
  s21_dest[99] = '\0';
  memset(std_dest, 'X', 99);
  std_dest[99] = '\0';
  s21_strncpy(s21_dest, "", 5);
  strncpy(std_dest, "", 5);
  ck_assert_str_eq(s21_dest, "");
  ck_assert_mem_eq(s21_dest, std_dest, 5);
  ck_assert_int_eq(s21_dest[0], '\0');
  ck_assert_int_eq(s21_dest[4], '\0');
  ck_assert_int_eq(s21_dest[5], 'X');
}
END_TEST

START_TEST(test_strncat) {
  char s21_dest[100];
  char std_dest[100];
  char append[] = "1234567890";

  strcpy(s21_dest, "start_");
  strcpy(std_dest, "start_");
  s21_strncat(s21_dest, append, 5);
  strncat(std_dest, append, 5);
  ck_assert_str_eq(s21_dest, std_dest);
  ck_assert_str_eq(s21_dest, "start_12345");

  strcpy(s21_dest, "begin_");
  strcpy(std_dest, "begin_");
  s21_strncat(s21_dest, "end", 5);
  strncat(std_dest, "end", 5);
  ck_assert_str_eq(s21_dest, std_dest);
  ck_assert_str_eq(s21_dest, "begin_end");

  strcpy(s21_dest, "zero");
  strcpy(std_dest, "zero");
  s21_strncat(s21_dest, append, 0);
  strncat(std_dest, append, 0);
  ck_assert_str_eq(s21_dest, std_dest);
  ck_assert_str_eq(s21_dest, "zero");

  strcpy(s21_dest, "empty_append");
  strcpy(std_dest, "empty_append");
  s21_strncat(s21_dest, "", 5);
  strncat(std_dest, "", 5);
  ck_assert_str_eq(s21_dest, std_dest);
  ck_assert_str_eq(s21_dest, "empty_append");

  strcpy(s21_dest, "");
  strcpy(std_dest, "");
  s21_strncat(s21_dest, append, 5);
  strncat(std_dest, append, 5);
  ck_assert_str_eq(s21_dest, std_dest);
  ck_assert_str_eq(s21_dest, "12345");

  strcpy(s21_dest, "terminate_");
  char long_append[] = "abcdefghijkl";
  s21_strncat(s21_dest, long_append, 5);
  ck_assert_str_eq(s21_dest, "terminate_abcde");
  ck_assert_int_eq(s21_dest[15], '\0');
}
END_TEST

START_TEST(test_strchr) {
  char str[] = "Hello World, Hello Universe!";
  char empty[] = "";

  ck_assert_ptr_eq(s21_strchr(str, 'H'), strchr(str, 'H'));
  ck_assert_ptr_eq(s21_strchr(str, 'o'), strchr(str, 'o'));
  ck_assert_ptr_eq(s21_strchr(str, ' '), strchr(str, ' '));
  ck_assert_ptr_eq(s21_strchr(str, '!'), strchr(str, '!'));
  ck_assert_ptr_eq(s21_strchr(str, 'W'), strchr(str, 'W'));
  ck_assert_ptr_eq(s21_strchr(str, 'U'), strchr(str, 'U'));

  ck_assert_ptr_eq(s21_strchr(str, 'z'), strchr(str, 'z'));
  ck_assert_ptr_eq(s21_strchr(str, 'A'), strchr(str, 'A'));

  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
  ck_assert_ptr_eq(s21_strchr(empty, '\0'), strchr(empty, '\0'));

  ck_assert_ptr_eq(s21_strchr(empty, 'a'), strchr(empty, 'a'));

  char str_plus[] = "Test+Case";
  ck_assert_ptr_eq(s21_strchr(str_plus, 300), strchr(str_plus, 300));
  ck_assert_ptr_eq(s21_strchr(str_plus, (int)'+'), strchr(str_plus, (int)'+'));
}
END_TEST

START_TEST(test_strstr) {
  char haystack[] = "This is a test string for testing strstr.";
  char needle1[] = "test";
  char needle2[] = "string";
  char needle3[] = "testing";
  char needle4[] = "This";
  char needle5[] = "strstr.";
  char needle6[] = "not found";
  char needle7[] = "";
  char haystack_empty[] = "";

  ck_assert_ptr_eq(s21_strstr(haystack, needle1), strstr(haystack, needle1));
  ck_assert_ptr_eq(s21_strstr(haystack, needle2), strstr(haystack, needle2));
  ck_assert_ptr_eq(s21_strstr(haystack, needle3), strstr(haystack, needle3));
  ck_assert_ptr_eq(s21_strstr(haystack, needle4), strstr(haystack, needle4));
  ck_assert_ptr_eq(s21_strstr(haystack, needle5), strstr(haystack, needle5));

  ck_assert_ptr_eq(s21_strstr(haystack, needle6), strstr(haystack, needle6));
  ck_assert_ptr_eq(s21_strstr(haystack, "strstr.."),
                   strstr(haystack, "strstr.."));

  ck_assert_ptr_eq(s21_strstr(haystack, needle7), strstr(haystack, needle7));
  ck_assert_ptr_eq(s21_strstr(haystack_empty, needle7),
                   strstr(haystack_empty, needle7));

  ck_assert_ptr_eq(s21_strstr(haystack_empty, needle1),
                   strstr(haystack_empty, needle1));

  ck_assert_ptr_eq(s21_strstr(needle1, haystack), strstr(needle1, haystack));

  ck_assert_ptr_eq(s21_strstr(needle1, needle1), strstr(needle1, needle1));

  ck_assert_ptr_eq(s21_strstr(haystack, S21_NULL), haystack);

  ck_assert_ptr_eq(s21_strstr(haystack, "Test"), strstr(haystack, "Test"));
}
END_TEST

START_TEST(test_strtok) {
  char str1[] = " Toke n1,Token2;Token3.Token4 ";
  char delim1[] = " ,;. ";

  char str2[] = " Toke n1,Token2;Token3.Token4 ";
  char *s21_token;
  char *std_token;

  s21_token = s21_strtok(str1, delim1);
  std_token = strtok(str2, delim1);
  ck_assert_ptr_nonnull(s21_token);
  ck_assert_ptr_nonnull(std_token);
  ck_assert_str_eq(s21_token, std_token);
  ck_assert_str_eq(s21_token, "Toke");

  s21_token = s21_strtok(NULL, delim1);
  std_token = strtok(NULL, delim1);
  ck_assert_ptr_nonnull(s21_token);
  ck_assert_ptr_nonnull(std_token);
  ck_assert_str_eq(s21_token, std_token);
  ck_assert_str_eq(s21_token, "n1");

  s21_token = s21_strtok(NULL, delim1);
  std_token = strtok(NULL, delim1);
  ck_assert_ptr_nonnull(s21_token);
  ck_assert_ptr_nonnull(std_token);
  ck_assert_str_eq(s21_token, std_token);
  ck_assert_str_eq(s21_token, "Token2");

  s21_token = s21_strtok(NULL, delim1);
  std_token = strtok(NULL, delim1);
  ck_assert_ptr_nonnull(s21_token);
  ck_assert_ptr_nonnull(std_token);
  ck_assert_str_eq(s21_token, std_token);
  ck_assert_str_eq(s21_token, "Token3");

  s21_token = s21_strtok(NULL, delim1);
  std_token = strtok(NULL, delim1);
  ck_assert_ptr_nonnull(s21_token);
  ck_assert_ptr_nonnull(std_token);
  ck_assert_str_eq(s21_token, std_token);
  ck_assert_str_eq(s21_token, "Token4");

  s21_token = s21_strtok(NULL, delim1);
  std_token = strtok(NULL, delim1);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);

  s21_token = s21_strtok(NULL, delim1);
  std_token = strtok(NULL, delim1);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);

  char str3[] = "a,,b; c..d";
  char delim2[] = ",;. ";
  char str4[] = "a,,b; c..d";
  s21_token = s21_strtok(str3, delim2);
  std_token = strtok(str4, delim2);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim2);
  std_token = strtok(NULL, delim2);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim2);
  std_token = strtok(NULL, delim2);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim2);
  std_token = strtok(NULL, delim2);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim2);
  std_token = strtok(NULL, delim2);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);

  char str5[] = "HelloWorld";
  char delim3[] = ",";
  char str6[] = "HelloWorld";
  s21_token = s21_strtok(str5, delim3);
  std_token = strtok(str6, delim3);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim3);
  std_token = strtok(NULL, delim3);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);

  char str7[] = ";;start-middle--end;;";
  char delim4[] = ";-";
  char str8[] = ";;start-middle--end;;";
  s21_token = s21_strtok(str7, delim4);
  std_token = strtok(str8, delim4);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim4);
  std_token = strtok(NULL, delim4);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim4);
  std_token = strtok(NULL, delim4);
  ck_assert_str_eq(s21_token, std_token);
  s21_token = s21_strtok(NULL, delim4);
  std_token = strtok(NULL, delim4);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);

  char str9[] = "";
  char delim5[] = ";";
  char str10[] = "";
  s21_token = s21_strtok(str9, delim5);
  std_token = strtok(str10, delim5);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);

  char str11[] = ",,, ,,";
  char delim6[] = ", ";
  char str12[] = ",,, ,,";
  s21_token = s21_strtok(str11, delim6);
  std_token = strtok(str12, delim6);
  ck_assert_ptr_null(s21_token);
  ck_assert_ptr_null(std_token);
}
END_TEST

START_TEST(test_memchr) {
  char str[] = "Hello\0World";
  char data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1};
  char empty[] = "";

  ck_assert_ptr_eq(s21_memchr(str, 'e', 12), memchr(str, 'e', 12));
  ck_assert_ptr_eq(s21_memchr(str, 'o', 5), memchr(str, 'o', 5));
  ck_assert_ptr_eq(s21_memchr(str, 'W', 12), memchr(str, 'W', 12));
  ck_assert_ptr_eq(s21_memchr(str, '\0', 12), memchr(str, '\0', 12));

  ck_assert_ptr_eq(s21_memchr(str, 'z', 12), memchr(str, 'z', 12));
  ck_assert_ptr_eq(s21_memchr(str, 'o', 4), memchr(str, 'o', 4));
  ck_assert_ptr_eq(s21_memchr(str, 'd', 12), memchr(str, 'd', 12));

  ck_assert_ptr_eq(s21_memchr(data, 5, 12), memchr(data, 5, 12));
  ck_assert_ptr_eq(s21_memchr(data, 0, 12), memchr(data, 0, 12));
  ck_assert_ptr_eq(s21_memchr(data, 9, 10), memchr(data, 9, 10));

  ck_assert_ptr_eq(s21_memchr(data, 10, 12), memchr(data, 10, 12));
  ck_assert_ptr_eq(s21_memchr(data, 0, 0), memchr(data, 0, 0));

  ck_assert_ptr_eq(s21_memchr(empty, 'a', 0), memchr(empty, 'a', 0));
  ck_assert_ptr_eq(s21_memchr(empty, 'a', 1), memchr(empty, 'a', 1));

  ck_assert_ptr_eq(s21_memchr(data, 256 + 3, 12), memchr(data, 256 + 3, 12));
  ck_assert_ptr_eq(s21_memchr(data, 3, 12), memchr(data, 3, 12));
}
END_TEST

START_TEST(test_memcmp) {
  char s1[] = "abcde";
  char s2[] = "abcDe";
  char s3[] = "abcde";
  char d1[] = {1, 2, 3, 0, 5};
  char d2[] = {1, 2, 3, 0, 6};
  char d3[] = {1, 2, 3, 0, 5};

  ck_assert_int_eq(s21_memcmp(s1, s3, 5), memcmp(s1, s3, 5));
  ck_assert_int_eq(s21_memcmp(s1, s3, 3), memcmp(s1, s3, 3));
  ck_assert_int_eq(s21_memcmp(s1, s3, 0), memcmp(s1, s3, 0));

  ck_assert_int_eq(s21_memcmp(s1, s2, 3), memcmp(s1, s2, 3));

  ck_assert_int_eq(s21_memcmp(d1, d3, 5), memcmp(d1, d3, 5));
  ck_assert_int_eq(s21_memcmp(d1, d2, 5), memcmp(d1, d2, 5));
  ck_assert_int_eq(s21_memcmp(d1, d2, 4), memcmp(d1, d2, 4));

  ck_assert(s21_memcmp(s1, s2, 5) > 0);
  ck_assert(memcmp(s1, s2, 5) > 0);
  ck_assert(s21_memcmp(s2, s1, 5) < 0);
  ck_assert(memcmp(s2, s1, 5) < 0);

  ck_assert(s21_memcmp(d1, d2, 5) < 0);
  ck_assert(memcmp(d1, d2, 5) < 0);
  ck_assert(s21_memcmp(d2, d1, 5) > 0);
  ck_assert(memcmp(d2, d1, 5) > 0);
}
END_TEST

START_TEST(test_memcpy) {
  char src[] = "Copy this data!";
  char s21_dest[50];
  char std_dest[50];
  char data_src[] = {1, 2, 3, 0, 4, 5};
  char s21_data_dest[10];
  char std_data_dest[10];

  memset(s21_dest, 'X', 49);
  s21_dest[49] = '\0';
  memset(std_dest, 'X', 49);
  std_dest[49] = '\0';
  s21_memcpy(s21_dest, src, strlen(src) + 1);
  memcpy(std_dest, src, strlen(src) + 1);
  ck_assert_str_eq(s21_dest, src);
  ck_assert_mem_eq(s21_dest, std_dest, strlen(src) + 5);

  memset(s21_dest, 'X', 49);
  s21_dest[49] = '\0';
  memset(std_dest, 'X', 49);
  std_dest[49] = '\0';
  s21_memcpy(s21_dest, src, 4);
  memcpy(std_dest, src, 4);
  ck_assert_mem_eq(s21_dest, "CopyXXXXXXXX", 12);
  ck_assert_mem_eq(s21_dest, std_dest, 12);

  memset(s21_dest, 'X', 49);
  s21_dest[49] = '\0';
  memset(std_dest, 'X', 49);
  std_dest[49] = '\0';
  s21_memcpy(s21_dest, src, 0);
  memcpy(std_dest, src, 0);
  ck_assert_str_eq(s21_dest,
                   "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  ck_assert_mem_eq(s21_dest, std_dest, 50);

  memset(s21_data_dest, 99, 9);
  s21_data_dest[9] = '\0';
  memset(std_data_dest, 99, 9);
  std_data_dest[9] = '\0';
  s21_memcpy(s21_data_dest, data_src, 6);
  memcpy(std_data_dest, data_src, 6);
  ck_assert_mem_eq(s21_data_dest, data_src, 6);
  ck_assert_mem_eq(s21_data_dest, std_data_dest, 10);

  char ret_s21[10];
  char ret_std[10];
  ck_assert_ptr_eq(s21_memcpy(ret_s21, "abc", 4), ret_s21);
  ck_assert_ptr_eq(memcpy(ret_std, "abc", 4), ret_std);
  ck_assert_str_eq(ret_s21, "abc");
}
END_TEST

START_TEST(test_memset) {
  char s21_buffer[50];
  char std_buffer[50];
  char compare_buffer[50];

  s21_memset(s21_buffer, 'A', 49);
  s21_buffer[49] = '\0';
  memset(std_buffer, 'A', 49);
  std_buffer[49] = '\0';
  memset(compare_buffer, 'A', 49);
  compare_buffer[49] = '\0';
  ck_assert_str_eq(s21_buffer, compare_buffer);
  ck_assert_mem_eq(s21_buffer, std_buffer, 50);

  memset(s21_buffer, 'X', 49);
  s21_buffer[49] = '\0';
  memset(std_buffer, 'X', 49);
  std_buffer[49] = '\0';
  s21_memset(s21_buffer, 66, 10);
  memset(std_buffer, 66, 10);
  memset(compare_buffer, 'B', 10);
  memset(compare_buffer + 10, 'X', 39);
  compare_buffer[49] = '\0';
  ck_assert_mem_eq(s21_buffer, compare_buffer, 50);
  ck_assert_mem_eq(s21_buffer, std_buffer, 50);

  memset(s21_buffer, 'X', 49);
  s21_buffer[49] = '\0';
  memset(std_buffer, 'X', 49);
  std_buffer[49] = '\0';
  s21_memset(s21_buffer, 0, 5);
  memset(std_buffer, 0, 5);
  memset(compare_buffer, 0, 5);
  memset(compare_buffer + 5, 'X', 44);
  compare_buffer[49] = '\0';
  ck_assert_mem_eq(s21_buffer, compare_buffer, 50);
  ck_assert_mem_eq(s21_buffer, std_buffer, 50);

  memset(s21_buffer, 'X', 49);
  s21_buffer[49] = '\0';
  memset(std_buffer, 'X', 49);
  std_buffer[49] = '\0';
  s21_memset(s21_buffer, 256 + 67, 3);
  memset(std_buffer, 256 + 67, 3);
  memset(compare_buffer, 'C', 3);
  memset(compare_buffer + 3, 'X', 46);
  compare_buffer[49] = '\0';
  ck_assert_mem_eq(s21_buffer, compare_buffer, 50);
  ck_assert_mem_eq(s21_buffer, std_buffer, 50);

  char ret_s21[10];
  char ret_std[10];
  ck_assert_ptr_eq(s21_memset(ret_s21, 'Z', 10), ret_s21);
  ck_assert_ptr_eq(memset(ret_std, 'Z', 10), ret_std);
  ck_assert_mem_eq(ret_s21, ret_std, 10);
}
END_TEST

START_TEST(test_strerror) {
#if defined(__APPLE__) || defined(__MACH__) || defined(__linux__)
  ck_assert_str_eq(s21_strerror(0), strerror(0));
  ck_assert_str_eq(s21_strerror(1), strerror(1));
  ck_assert_str_eq(s21_strerror(2), strerror(2));
  ck_assert_str_eq(s21_strerror(10), strerror(10));
  ck_assert_str_eq(s21_strerror(34), strerror(34));
#endif

  char s21_unknown_buf[100];

  sprintf(s21_unknown_buf, "Unknown error %d", -1);
  ck_assert_str_eq(s21_strerror(-1), s21_unknown_buf);

  int large_err = S21_MAX_ERROR + 10;
  sprintf(s21_unknown_buf, "Unknown error %d", large_err);
  ck_assert_str_eq(s21_strerror(large_err), s21_unknown_buf);
}
END_TEST

START_TEST(test_insert) {
  char *s21_res = NULL;
  const char *src;
  const char *str_to_insert;

  src = "Hello World";
  str_to_insert = "beautiful ";
  s21_res = s21_insert(src, str_to_insert, 6);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello beautiful World");
  free(s21_res);
  s21_res = NULL;

  src = "World";
  str_to_insert = "Hello ";
  s21_res = s21_insert(src, str_to_insert, 0);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "Hello";
  str_to_insert = " World";
  s21_res = s21_insert(src, str_to_insert, s21_strlen(src));
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "Hello World";
  str_to_insert = "";
  s21_res = s21_insert(src, str_to_insert, 6);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "";
  str_to_insert = "Inserted";
  s21_res = s21_insert(src, str_to_insert, 0);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Inserted");
  free(s21_res);
  s21_res = NULL;

  ck_assert_ptr_null(s21_insert(NULL, "abc", 0));
  ck_assert_ptr_null(s21_insert("abc", NULL, 0));
  ck_assert_ptr_null(s21_insert(NULL, NULL, 0));

  src = "abc";
  str_to_insert = "def";
  ck_assert_ptr_null(s21_insert(src, str_to_insert, 4));
  ck_assert_ptr_null(s21_insert(src, str_to_insert, 100));
}
END_TEST

START_TEST(test_trim) {
  char *s21_res = NULL;
  const char *src;
  const char *trim_chars;

  src = " \t\n Hello World \t\n ";
  trim_chars = " \t\n";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "***Hello World";
  trim_chars = "*";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "Hello World---";
  trim_chars = "-";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "Hello World";
  trim_chars = "xyz";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello World");
  free(s21_res);
  s21_res = NULL;

  src = "abababa";
  trim_chars = "ab";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "");
  free(s21_res);
  s21_res = NULL;

  src = "  Hello  ";
  trim_chars = "";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "  Hello  ");
  free(s21_res);
  s21_res = NULL;

  ck_assert_ptr_null(s21_trim(NULL, NULL));

  src = "---Hello-World---";
  trim_chars = "-";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello-World");
  free(s21_res);
  s21_res = NULL;

  src = "/.,Hello,. /World/.,";
  trim_chars = "/., ";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "Hello,. /World");
  free(s21_res);
  s21_res = NULL;

  src = "123123123";
  trim_chars = "312";
  s21_res = s21_trim(src, trim_chars);
  ck_assert_ptr_nonnull(s21_res);
  ck_assert_str_eq(s21_res, "");
  free(s21_res);
  s21_res = NULL;
}
END_TEST

START_TEST(test_sprintf_c) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "char %c", 'A');
  sprintf(std_buffer, "char %c", 'A');
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "char %c", '0');
  sprintf(std_buffer, "char %c", '0');
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_d) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "int %d", 42);
  sprintf(std_buffer, "int %d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %d", -42);
  sprintf(std_buffer, "int %d", -42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %d", 0);
  sprintf(std_buffer, "int %d", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %+d", 42);
  sprintf(std_buffer, "int %+d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "int %+d", -42);
  sprintf(std_buffer, "int %+d", -42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int % d", 42);
  sprintf(std_buffer, "int % d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "int % d", -42);
  sprintf(std_buffer, "int % d", -42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %05d", 42);
  sprintf(std_buffer, "int %05d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %5d", 42);
  sprintf(std_buffer, "int %5d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %-5d", 42);
  sprintf(std_buffer, "int %-5d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %ld", LONG_MAX);
  sprintf(std_buffer, "int %ld", LONG_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %hd", (short)32767);
  sprintf(std_buffer, "int %hd", (short)32767);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "int %hd", (short)-32768);
  sprintf(std_buffer, "int %hd", (short)-32768);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %d", INT_MAX);
  sprintf(std_buffer, "int %d", INT_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %d", INT_MIN);
  sprintf(std_buffer, "int %d", INT_MIN);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "int %.5d", 42);
  sprintf(std_buffer, "int %.5d", 42);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "int %.5d", -42);
  sprintf(std_buffer, "int %.5d", -42);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_s) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "string %s", "Hello");
  sprintf(std_buffer, "string %s", "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "string %10s", "Hello");
  sprintf(std_buffer, "string %10s", "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "string %-10s", "Hello");
  sprintf(std_buffer, "string %-10s", "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "string %.3s", "Hello");
  sprintf(std_buffer, "string %.3s", "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "string %10.3s", "Hello");
  sprintf(std_buffer, "string %10.3s", "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "string %-10.3s", "Hello");
  sprintf(std_buffer, "string %-10.3s", "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "string %s", "");
  sprintf(std_buffer, "string %s", "");
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_f) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "float %f", 3.14);
  sprintf(std_buffer, "float %f", 3.14);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %f", -3.14);
  sprintf(std_buffer, "float %f", -3.14);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %.2f", 3.14159);
  sprintf(std_buffer, "float %.2f", 3.14159);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %10.2f", 3.14159);
  sprintf(std_buffer, "float %10.2f", 3.14159);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %-10.2f", 3.14159);
  sprintf(std_buffer, "float %-10.2f", 3.14159);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %f", 0.0);
  sprintf(std_buffer, "float %f", 0.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %f", 1234567.89);
  sprintf(std_buffer, "float %f", 1234567.89);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "float %f", -1234567.89);
  sprintf(std_buffer, "float %f", -1234567.89);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_u) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "unsigned int %u", 42u);
  sprintf(std_buffer, "unsigned int %u", 42u);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %u", 0u);
  sprintf(std_buffer, "unsigned int %u", 0u);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %u", UINT_MAX);
  sprintf(std_buffer, "unsigned int %u", UINT_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %5u", 42u);
  sprintf(std_buffer, "unsigned int %5u", 42u);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %05u", 42u);
  sprintf(std_buffer, "unsigned int %05u", 42u);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %-5u", 42u);
  sprintf(std_buffer, "unsigned int %-5u", 42u);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %hu", (unsigned short)65535u);
  sprintf(std_buffer, "unsigned int %hu", (unsigned short)65535u);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "unsigned int %hu", (unsigned short)0u);
  sprintf(std_buffer, "unsigned int %hu", (unsigned short)0u);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %lu", ULONG_MAX);
  sprintf(std_buffer, "unsigned int %lu", ULONG_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "unsigned int %lu", 0ul);
  sprintf(std_buffer, "unsigned int %lu", 0ul);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned int %.5u", 42u);
  sprintf(std_buffer, "unsigned int %.5u", 42u);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "unsigned int %.0u", 0u);
  sprintf(std_buffer, "unsigned int %.0u", 0u);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_x) {
  char s21_buffer[100];
  char std_buffer[100];
  unsigned int val = 255;

  s21_sprintf(s21_buffer, "hex %x", val);
  sprintf(std_buffer, "hex %x", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %X", val);
  sprintf(std_buffer, "hex %X", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %x", 0);
  sprintf(std_buffer, "hex %x", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %X", 0);
  sprintf(std_buffer, "hex %X", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %x", UINT_MAX);
  sprintf(std_buffer, "hex %x", UINT_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %X", UINT_MAX);
  sprintf(std_buffer, "hex %X", UINT_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %5x", val);
  sprintf(std_buffer, "hex %5x", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %05x", val);
  sprintf(std_buffer, "hex %05x", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %-5X", val);
  sprintf(std_buffer, "hex %-5X", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %#x", val);
  sprintf(std_buffer, "hex %#x", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %#X", val);
  sprintf(std_buffer, "hex %#X", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %#x", 0);
  sprintf(std_buffer, "hex %#x", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex [%#8x]", val);
  sprintf(std_buffer, "hex [%#8x]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %hx", (unsigned short)val);
  sprintf(std_buffer, "hex %hx", (unsigned short)val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %lX", (unsigned long)val);
  sprintf(std_buffer, "hex %lX", (unsigned long)val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex %.5x", val);
  sprintf(std_buffer, "hex %.5x", val);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "hex %.0x", 0);
  sprintf(std_buffer, "hex %.0x", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "hex [%10.5X]", val);
  sprintf(std_buffer, "hex [%10.5X]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "hex [%-10.5x]", val);
  sprintf(std_buffer, "hex [%-10.5x]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_o) {
  char s21_buffer[100];
  char std_buffer[100];
  unsigned int val = 63;

  s21_sprintf(s21_buffer, "octal %o", val);
  sprintf(std_buffer, "octal %o", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %o", 0);
  sprintf(std_buffer, "octal %o", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %o", UINT_MAX);
  sprintf(std_buffer, "octal %o", UINT_MAX);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %5o", val);
  sprintf(std_buffer, "octal %5o", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %05o", val);
  sprintf(std_buffer, "octal %05o", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %-5o", val);
  sprintf(std_buffer, "octal %-5o", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %#o", 0);
  sprintf(std_buffer, "octal %#o", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %ho", (unsigned short)val);
  sprintf(std_buffer, "octal %ho", (unsigned short)val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %lo", (unsigned long)val);
  sprintf(std_buffer, "octal %lo", (unsigned long)val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "octal %.5o", val);
  sprintf(std_buffer, "octal %.5o", val);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "octal %.0o", 0);
  sprintf(std_buffer, "octal %.0o", 0);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_e) {
  char s21_buffer[512] = {0};
  char std_buffer[512] = {0};
  double val = 123.456789;

  s21_sprintf(s21_buffer, "exp %e", val);
  sprintf(std_buffer, "exp %e", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %E", val);
  sprintf(std_buffer, "exp %E", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %e", -val);
  sprintf(std_buffer, "exp %e", -val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %.2e", val);
  sprintf(std_buffer, "exp %.2e", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %.0e", val);
  sprintf(std_buffer, "exp %.0e", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp [%15.2E]", val);
  sprintf(std_buffer, "exp [%15.2E]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp [%-15.2e]", val);
  sprintf(std_buffer, "exp [%-15.2e]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp [%+15.2e]", val);
  sprintf(std_buffer, "exp [%+15.2e]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp [% 15.2e]", val);
  sprintf(std_buffer, "exp [% 15.2e]", val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp [% 15.2e]", -val);
  sprintf(std_buffer, "exp [% 15.2e]", -val);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp [%.0e]", 1.0);
  sprintf(std_buffer, "exp [%.0e]", 1.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %e", 0.0);
  sprintf(std_buffer, "exp %e", 0.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %e", 1234567.89);
  sprintf(std_buffer, "exp %e", 1234567.89);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %e", -1234567.89);
  sprintf(std_buffer, "exp %e", -1234567.89);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "exp %e", 0.000012345);
  sprintf(std_buffer, "exp %e", 0.000012345);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_g) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "gen %g", 123.456);
  sprintf(std_buffer, "gen %g", 123.456);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 1234567.0);
  sprintf(std_buffer, "gen %g", 1234567.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 0.00012345);
  sprintf(std_buffer, "gen %g", 0.00012345);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 0.001);
  sprintf(std_buffer, "gen %g", 0.001);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %G", 1234567.0);
  sprintf(std_buffer, "gen %G", 1234567.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %.2g", 123.456);
  sprintf(std_buffer, "gen %.2g", 123.456);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %.3g", 1.2345);
  sprintf(std_buffer, "gen %.3g", 1.2345);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %.3g", 0.0012345);
  sprintf(std_buffer, "gen %.3g", 0.0012345);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %.0g", 123.0);
  sprintf(std_buffer, "gen %.0g", 123.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 3.14000);
  sprintf(std_buffer, "gen %g", 3.14000);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 3.0);
  sprintf(std_buffer, "gen %g", 3.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %#g", 3.0);
  sprintf(std_buffer, "gen %#g", 3.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %#g", 123.450);
  sprintf(std_buffer, "gen %#g", 123.450);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen [%10.3g]", 1.2345);
  sprintf(std_buffer, "gen [%10.3g]", 1.2345);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen [%-10.3G]", 12345.0);
  sprintf(std_buffer, "gen [%-10.3G]", 12345.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen [%+10.3g]", 1.23);
  sprintf(std_buffer, "gen [%+10.3g]", 1.23);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen [% 10.3g]", 1.23);
  sprintf(std_buffer, "gen [% 10.3g]", 1.23);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 0.0);
  sprintf(std_buffer, "gen %g", 0.0);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", 1234567.89);
  sprintf(std_buffer, "gen %g", 1234567.89);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "gen %g", -1234567.89);
  sprintf(std_buffer, "gen %g", -1234567.89);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_p) {
  char s21_buffer[100];
  char std_buffer[100];
  int x = 10;
  void *ptr = (void *)&x;
  void *null_ptr = NULL;

  s21_sprintf(s21_buffer, "%p", ptr);
  sprintf(std_buffer, "%p", ptr);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "[%p]", null_ptr);
  sprintf(std_buffer, "[%p]", null_ptr);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "[%20p]", ptr);
  sprintf(std_buffer, "[%20p]", ptr);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "[%-20p]", ptr);
  sprintf(std_buffer, "[%-20p]", ptr);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "[%20p]", null_ptr);
  sprintf(std_buffer, "[%20p]", null_ptr);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char s21_buffer[100];
  char std_buffer[100];

  s21_sprintf(s21_buffer, "%%");
  sprintf(std_buffer, "%%");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "100%% completion");
  sprintf(std_buffer, "100%% completion");
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_length_h) {
  char s21_buffer[100];
  char std_buffer[100];
  short sh_pos = 123;
  short sh_neg = -123;
  unsigned short ush = 65000;

  s21_sprintf(s21_buffer, "short %hd", sh_pos);
  sprintf(std_buffer, "short %hd", sh_pos);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "short %hd", sh_neg);
  sprintf(std_buffer, "short %hd", sh_neg);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned short %hu", ush);
  sprintf(std_buffer, "unsigned short %hu", ush);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex short %hx", ush);
  sprintf(std_buffer, "hex short %hx", ush);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "oct short %ho", ush);
  sprintf(std_buffer, "oct short %ho", ush);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_length_l) {
  char s21_buffer[100];
  char std_buffer[100];
  long l_pos = 1234567890L;
  long l_neg = -1234567890L;
  unsigned long ul = 4000000000UL;

  s21_sprintf(s21_buffer, "long %ld", l_pos);
  sprintf(std_buffer, "long %ld", l_pos);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "long %ld", l_neg);
  sprintf(std_buffer, "long %ld", l_neg);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "unsigned long %lu", ul);
  sprintf(std_buffer, "unsigned long %lu", ul);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "hex long %lx", ul);
  sprintf(std_buffer, "hex long %lx", ul);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "oct long %lo", ul);
  sprintf(std_buffer, "oct long %lo", ul);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_length_L) {
  char s21_buffer[100];
  char std_buffer[100];
  long double ld = 123456789.987654321L;

  s21_sprintf(s21_buffer, "long double %Lf", ld);
  sprintf(std_buffer, "long double %Lf", ld);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "long double %LE", ld);
  sprintf(std_buffer, "long double %LE", ld);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "long double %Lg", ld);
  sprintf(std_buffer, "long double %Lg", ld);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_width_star) {
  char s21_buffer[100];
  char std_buffer[100];
  int width = 10;

  s21_sprintf(s21_buffer, "width * [%*d]", width, 42);
  sprintf(std_buffer, "width * [%*d]", width, 42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "width * [%*s]", width, "Hello");
  sprintf(std_buffer, "width * [%*s]", width, "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "width * [%*f]", width, 3.14);
  sprintf(std_buffer, "width * [%*f]", width, 3.14);
  ck_assert_str_eq(s21_buffer, std_buffer);

  width = 3;
  s21_sprintf(s21_buffer, "width * [%*d]", width, 12345);
  sprintf(std_buffer, "width * [%*d]", width, 12345);
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_precision_star) {
  char s21_buffer[100];
  char std_buffer[100];
  int precision = 5;

  s21_sprintf(s21_buffer, "precision * [%.*f]", precision, 3.1415926);
  sprintf(std_buffer, "precision * [%.*f]", precision, 3.1415926);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "precision * [%.*s]", precision, "Hello World");
  sprintf(std_buffer, "precision * [%.*s]", precision, "Hello World");
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "precision * [%.*d]", precision, 42);
  sprintf(std_buffer, "precision * [%.*d]", precision, 42);
  ck_assert_str_eq(s21_buffer, std_buffer);

  precision = 0;
  s21_sprintf(s21_buffer, "precision * [%.*f]", precision, 3.14);
  sprintf(std_buffer, "precision * [%.*f]", precision, 3.14);
  ck_assert_str_eq(s21_buffer, std_buffer);

  precision = -1;
  s21_sprintf(s21_buffer, "precision * [%.*f]", precision, 3.1415926);
  sprintf(std_buffer, "precision * [%.*f]", precision, 3.1415926);
  ck_assert_str_eq(s21_buffer, std_buffer);
  s21_sprintf(s21_buffer, "precision * [%.*s]", precision, "Hello");
  sprintf(std_buffer, "precision * [%.*s]", precision, "Hello");
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(test_sprintf_width_precision_star) {
  char s21_buffer[100];
  char std_buffer[100];
  int width = 10;
  int precision = 3;

  s21_sprintf(s21_buffer, "comb [%*.*f]", width, precision, 1.23456);
  sprintf(std_buffer, "comb [%*.*f]", width, precision, 1.23456);
  ck_assert_str_eq(s21_buffer, std_buffer);

  s21_sprintf(s21_buffer, "comb [%*.*s]", width, precision, "HelloWorld");
  sprintf(std_buffer, "comb [%*.*s]", width, precision, "HelloWorld");
  ck_assert_str_eq(s21_buffer, std_buffer);
}
END_TEST

START_TEST(sscanf_single_d) {
  int a = 0;
  int res = s21_sscanf("123", "%d", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, 123);

  res = s21_sscanf("-987", "%d", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, -987);

  res = s21_sscanf("+50", "%d", &a);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(sscanf_single_i) {
  int a = 0;
  int res = s21_sscanf("-456", "%i", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, -456);
  res = s21_sscanf("0777", "%i", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, 511);
  res = s21_sscanf("0x1aF", "%i", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, 431);
  res = s21_sscanf("+0X1A", "%i", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, 26);
}
END_TEST
START_TEST(sscanf_single_o) {
  unsigned int a = 0;
  int res = s21_sscanf("777", "%o", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(a, 0777);
  res = s21_sscanf("1234", "%o", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(a, 01234);
}
END_TEST
START_TEST(sscanf_single_x) {
  unsigned int a = 0;
  int res = s21_sscanf("1aF", "%x", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(a, 0x1AF);
  res = s21_sscanf("0XBEF", "%X", &a);
  ck_assert_int_eq(res, 1);
  res = s21_sscanf("beef", "%x", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(a, 0xBEEF);
}
END_TEST
START_TEST(sscanf_single_u) {
  unsigned int a = 0;
  int res = s21_sscanf("4294967295", "%u", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(a, 4294967295U);
  res = s21_sscanf("0", "%u", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(a, 0);
}
END_TEST
START_TEST(sscanf_single_f) {
  float a = 0.0f;
  int res = s21_sscanf("3.14", "%f", &a);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(a - 3.14f) < 1e-6);
  res = s21_sscanf("-0.001", "%f", &a);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(a - (-0.001f)) < 1e-6);
  res = s21_sscanf("1.23e+2", "%f", &a);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(a - 123.0f) < 1e-6);
  res = s21_sscanf("1.23E-1", "%f", &a);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(a - 0.123f) < 1e-6);
  res = s21_sscanf("6.022e23", "%e", &a);
  ck_assert_int_eq(res, 1);
  res = s21_sscanf("100.0", "%g", &a);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(a - 100.0f) < 1e-6);
}
END_TEST
START_TEST(sscanf_single_s) {
  char str[100];
  memset(str, 0, 100);
  int res = s21_sscanf("hello world", "%s", str);
  ck_assert_int_eq(res, 1);
  ck_assert_str_eq(str, "hello");
  memset(str, 0, 100);
  res = s21_sscanf("\t leading_whitespace", "%s", str);
  ck_assert_int_eq(res, 1);
  memset(str, 0, 100);
  res = s21_sscanf("single", "%s", str);
  ck_assert_int_eq(res, 1);
  ck_assert_str_eq(str, "single");
}
END_TEST
START_TEST(sscanf_single_c) {
  char c = 0;
  int res = s21_sscanf("A", "%c", &c);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(c, 'A');
  res = s21_sscanf(" B", "%c", &c);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(c, ' ');
  res = s21_sscanf("C", "%c", &c);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(c, 'C');
}
END_TEST
START_TEST(sscanf_single_p) {
  void *ptr = NULL;
  void *expected_ptr = (void *)0x1a2b3c4d;
  int res = s21_sscanf("1a2b3c4d", "%p", &ptr);
  ck_assert_int_eq(res, 1);
  ck_assert_ptr_eq(ptr, expected_ptr);
  res = s21_sscanf("0xabcdef01", "%p", &ptr);
  ck_assert_int_eq(res, 1);
  ck_assert_ptr_eq(ptr, (void *)0xabcdef01);
  res = s21_sscanf("0", "%p", &ptr);
  ck_assert_int_eq(res, 1);
  ck_assert_ptr_eq(ptr, (void *)0x0);
}
END_TEST
START_TEST(sscanf_single_n) {
  int pos = -1;
  int a = 0;
  int res = s21_sscanf("123 abc", "%d%n", &a, &pos);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, 123);
  ck_assert_int_eq(pos, 3);
  pos = -1;
  res = s21_sscanf("word 456", "%*s%n", &pos);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(pos, 4);
  pos = -1;
  res = s21_sscanf("hello", "%n", &pos);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(pos, 0);
  pos = -1;
  res = s21_sscanf("   abc", " %n%*s", &pos);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(pos, 3);
}
END_TEST
START_TEST(sscanf_multiple_specifiers) {
  int a = 0;
  float c = 0.0f;
  char str[100];
  memset(str, 0, 100);
  int res = s21_sscanf("42 3.14 hello", "%d %f %s", &a, &c, str);
  ck_assert_int_eq(res, 3);
  ck_assert_int_eq(a, 42);
  ck_assert(fabsf(c - 3.14f) < 1e-6);
  ck_assert_str_eq(str, "hello");
  unsigned u = 0;
  char ch = 0;
  memset(str, 0, 100);
  res = s21_sscanf("100\tA-1.5word", "%u %c %f %s", &u, &ch, &c, str);
  ck_assert_int_eq(res, 4);
}
END_TEST
START_TEST(sscanf_skip_with_asterisk) {
  int a = 0, b = 0;
  int res = s21_sscanf("123 skip_this 456", "%d %*s %d", &a, &b);
  ck_assert_int_eq(res, 2);
  ck_assert_int_eq(a, 123);
  ck_assert_int_eq(b, 456);
  float f = 0.0f;
  res = s21_sscanf("1.1 ignore 2.2 3.3", "%*f %*s %f", &f);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(f - 2.2f) < 1e-6);
}
END_TEST
START_TEST(sscanf_width) {
  char str[100];
  memset(str, 0, 100);
  int res = s21_sscanf("abcdefgh", "%5s", str);
  ck_assert_int_eq(res, 1);
  ck_assert_str_eq(str, "abcde");
  memset(str, 0, 100);
  char c1 = 0, c2 = 0;
  res = s21_sscanf("XY", "%1c%1c", &c1, &c2);
  ck_assert_int_eq(res, 2);
  ck_assert_int_eq(c1, 'X');
  ck_assert_int_eq(c2, 'Y');
  int d = 0;
  res = s21_sscanf("123456", "%3d", &d);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(d, 123);
  float f = 0.0f;
  res = s21_sscanf("1.2345", "%3f", &f);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(f - 1.2f) < 1e-6);
  unsigned x = 0;
  res = s21_sscanf("ABCDEF", "%4x", &x);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(x, 0xABCD);
  unsigned o = 0;
  res = s21_sscanf("76543", "%2o", &o);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(o, 076);
}
END_TEST
START_TEST(sscanf_length_short) {
  short hd = 0;
  int res = s21_sscanf("12345", "%hd", &hd);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(hd, 12345);
  unsigned short hu = 0;
  res = s21_sscanf("65535", "%hu", &hu);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(hu, 65535);
  unsigned short ho = 0;
  res = s21_sscanf("177777", "%ho", &ho);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(ho, 65535);
  unsigned short hx = 0;
  res = s21_sscanf("FFFF", "%hx", &hx);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(hx, 65535);
}
END_TEST
START_TEST(sscanf_length_long) {
  long ld = 0;
  int res = s21_sscanf("1234567890", "%ld", &ld);
  ck_assert_int_eq(res, 1);
  ck_assert(ld == 1234567890L);
  unsigned long lu = 0;
  res = s21_sscanf("429496729512", "%lu", &lu);
  ck_assert_int_eq(res, 1);
  ck_assert(lu == 429496729512UL);
  unsigned long lo = 0;
  res = s21_sscanf("12345670123", "%lo", &lo);
  ck_assert_int_eq(res, 1);
  ck_assert(lo == 012345670123UL);
  unsigned long lx = 0;
  res = s21_sscanf("ABCDEF123", "%lx", &lx);
  ck_assert_int_eq(res, 1);
  ck_assert(lx == 0xABCDEF123UL);
  long li_d = 0;
  res = s21_sscanf("-9876543210", "%li", &li_d);
  ck_assert_int_eq(res, 1);
  ck_assert(li_d == -9876543210L);
  long li_o = 0;
  res = s21_sscanf("012345", "%li", &li_o);
  ck_assert_int_eq(res, 1);
  ck_assert(li_o == 012345L);
  long li_x = 0;
  res = s21_sscanf("0xABC", "%li", &li_x);
  ck_assert_int_eq(res, 1);
  ck_assert(li_x == 0xABCL);
}
END_TEST
START_TEST(sscanf_length_double) {
  double lf = 0.0;
  int res = s21_sscanf("1.23456789", "%lf", &lf);
  ck_assert_int_eq(res, 1);
  res = s21_sscanf("-1e-10", "%le", &lf);
  ck_assert_int_eq(res, 1);
}
END_TEST
START_TEST(sscanf_length_long_double) {
  long double Lf = 0.0L;
  int res = s21_sscanf("3.141592653589793", "%Lf", &Lf);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsl(Lf - 3.141592653589793L) < 1e-15);
  res = s21_sscanf("-1.234567E+50", "%LE", &Lf);
  ck_assert_int_eq(res, 1);
  res = s21_sscanf("12345.67890123", "%Lg", &Lf);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsl(Lf - 12345.67890123L) < 1e-8);
}
END_TEST
START_TEST(sscanf_percent_sign) {
  int res = s21_sscanf("Check 50% done", "Check 50%% done");
  ck_assert_int_eq(res, 0);
  int a = 0;
  res = s21_sscanf("Value: 100%", "Value: %d%%", &a);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(a, 100);
  res = s21_sscanf("Value: 100 percent", "Value: %d%%", &a);
  ck_assert_int_eq(res, 1);
  res = s21_sscanf("ABC", "%%%d", &a);
  ck_assert_int_eq(res, 0);
}
END_TEST
START_TEST(sscanf_wide_char) {
  char *locale_ret = setlocale(LC_ALL, "");
  ck_assert_ptr_ne(locale_ret, NULL);
  wchar_t wc = 0;
  int res = 0;
  res = s21_sscanf("A", "%lc", &wc);
  ck_assert_int_eq(res, 1);
  ck_assert(wc == L'A');
  const char *input_cyrillic = "Я";
  res = s21_sscanf(input_cyrillic, "%lc", &wc);
  ck_assert_int_eq(res, 1);
  ck_assert(wc == L'Я');
  wchar_t wc_arr[5] = {0};
  res = s21_sscanf("Тест", "%3lc", wc_arr);
  ck_assert_int_eq(res, 1);
  ck_assert(wc_arr[0] == L'Т');
  ck_assert(wc_arr[1] == L'е');
  ck_assert(wc_arr[2] == L'с');
  ck_assert(wc_arr[3] == L'\0');
  res = s21_sscanf("Юникод", "%*lc%lc", &wc);
  ck_assert_int_eq(res, 1);
  ck_assert(wc == L'н');
}
END_TEST
START_TEST(sscanf_wide_string) {
  char *locale_ret = setlocale(LC_ALL, "");
  ck_assert_ptr_ne(locale_ret, NULL);
  wchar_t ws[100];
  memset(ws, 0, sizeof(ws));
  int res = 0;
  const char *input_cyr_str = "Привет мир";
  res = s21_sscanf(input_cyr_str, "%ls", ws);
  ck_assert_int_eq(res, 1);
  ck_assert(wcscmp(ws, L"Привет") == 0);
  memset(ws, 0, sizeof(ws));
  res = s21_sscanf("HelloWorld", "%5ls", ws);
  ck_assert_int_eq(res, 1);
  ck_assert(wcscmp(ws, L"Hello") == 0);
  wchar_t ws2[100];
  memset(ws, 0, sizeof(ws));
  memset(ws2, 0, sizeof(ws2));
  res = s21_sscanf("SkipMe KeepMe", "%*ls %ls", ws2);
  ck_assert_int_eq(res, 1);
  ck_assert(wcscmp(ws2, L"KeepMe") == 0);
}
END_TEST
START_TEST(sscanf_mixed_wide_and_narrow) {
  char *locale_ret = setlocale(LC_ALL, "");
  ck_assert_ptr_ne(locale_ret, NULL);
  wchar_t wc = 0;
  int num = 0;
  char str[100];
  memset(str, 0, 100);
  int res = s21_sscanf("Я42", "%lc%d", &wc, &num);
  ck_assert_int_eq(res, 2);
  ck_assert(wc == L'Я');
  ck_assert_int_eq(num, 42);
  wc = 0;
  res = s21_sscanf("TestЖ", "%s%lc", str, &wc);
  ck_assert_int_eq(res, 2);
  num = 0;
  res = s21_sscanf("SkipЮ 123", "%*s%*lc %d", &num);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(num, 123);
}
END_TEST
START_TEST(sscanf_no_match) {
  int d = 0;
  int res = s21_sscanf("abc", "%d", &d);
  ck_assert_int_eq(d, 0);
  res = s21_sscanf("123 xyz", "%d %d", &d, &d);
  res = s21_sscanf("Value: 100", "Val: %d", &d);
  ck_assert_int_eq(res, 0);
}
END_TEST
START_TEST(sscanf_whitespace_handling) {
  int a = 0, b = 0;
  char s[10];
  int res = s21_sscanf("123 \t\n456", "%d   %d", &a, &b);
  ck_assert_int_eq(res, 2);
  ck_assert_int_eq(a, 123);
  ck_assert_int_eq(b, 456);
  res = s21_sscanf("   abc", " %s", s);
  ck_assert_int_eq(res, 1);
  ck_assert_str_eq(s, "abc");
  char c1 = 0, c2 = 0;
  res = s21_sscanf("  X Y", " %c %c", &c1, &c2);
  ck_assert_int_eq(res, 2);
  ck_assert_int_eq(c1, 'X');
  ck_assert_int_eq(c2, 'Y');
  res = s21_sscanf(" Z", "%c", &c1);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(c1, ' ');
}
END_TEST
START_TEST(sscanf_trailing_chars) {
  int d = 0;
  int res = s21_sscanf("123xyz", "%d", &d);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(d, 123);
  float f = 0;
  res = s21_sscanf("3.14stop", "%f", &f);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(f - 3.14f) < 1e-6);
  char s[10];
  res = s21_sscanf("hello world", "%s", s);
  ck_assert_int_eq(res, 1);
  ck_assert_str_eq(s, "hello");
}
END_TEST
START_TEST(sscanf_numeric_edge_cases) {
  int d = 0;
  int res = s21_sscanf("2147483647", "%d", &d);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(d, INT_MAX);
  res = s21_sscanf("-2147483648", "%d", &d);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(d, INT_MIN);
  unsigned u = 0;
  res = s21_sscanf("4294967295", "%u", &u);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(u, UINT_MAX);
  res = s21_sscanf("0", "%d", &d);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(d, 0);
  res = s21_sscanf("0", "%u", &u);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(u, 0);
  res = s21_sscanf("000", "%o", &u);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(u, 0);
  res = s21_sscanf("0x0", "%x", &u);
  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(u, 0);
  float f = 1.0f;
  res = s21_sscanf("0.0", "%f", &f);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(f - 0.0f) < 1e-9);
  res = s21_sscanf("-0.0", "%f", &f);
  ck_assert_int_eq(res, 1);
  ck_assert(fabsf(f - 0.0f) < 1e-9);
}
END_TEST

START_TEST(test_s21_strcspn) {
  ck_assert_int_eq(s21_strcspn("hello", "lo"), strcspn("hello", "lo"));
  ck_assert_int_eq(s21_strcspn("abcde", "xyz"), strcspn("abcde", "xyz"));
  ck_assert_int_eq(s21_strcspn("abcde", "a"), strcspn("abcde", "a"));
  ck_assert_int_eq(s21_strcspn("abcde", "cba"), strcspn("abcde", "cba"));
  ck_assert_int_eq(s21_strcspn("12345", "abc"), strcspn("12345", "abc"));
  ck_assert_int_eq(s21_strcspn("123a45", "abc"), strcspn("123a45", "abc"));
  ck_assert_int_eq(s21_strcspn("", "abc"), strcspn("", "abc"));
  ck_assert_int_eq(s21_strcspn("abc", ""), strcspn("abc", ""));
  ck_assert_int_eq(s21_strcspn("", ""), strcspn("", ""));
}
END_TEST

START_TEST(test_s21_strrchr) {
  char str1[] = "Hello, World!";
  ck_assert_ptr_eq(s21_strrchr(str1, 'o'), strrchr(str1, 'o'));
  ck_assert_ptr_eq(s21_strrchr(str1, 'l'), strrchr(str1, 'l'));
  ck_assert_ptr_eq(s21_strrchr(str1, 'H'), strrchr(str1, 'H'));
  ck_assert_ptr_eq(s21_strrchr(str1, '!'), strrchr(str1, '!'));
  ck_assert_ptr_null(s21_strrchr(str1, 'z'));
  ck_assert_ptr_null(s21_strrchr("", 'a'));
}
END_TEST

START_TEST(test_s21_strpbrk) {
  char str1[] = "Hello, World!";
  ck_assert_ptr_eq(s21_strpbrk(str1, "ol"), strpbrk(str1, "ol"));
  ck_assert_ptr_eq(s21_strpbrk(str1, "W!"), strpbrk(str1, "W!"));
  ck_assert_ptr_eq(s21_strpbrk(str1, ",!"), strpbrk(str1, ",!"));
  ck_assert_ptr_null(s21_strpbrk(str1, "xyz"));
  ck_assert_ptr_null(s21_strpbrk("", "abc"));
  ck_assert_ptr_null(s21_strpbrk("abc", ""));
  ck_assert_ptr_null(s21_strpbrk("", ""));
  ck_assert_ptr_null(s21_strpbrk(NULL, "abc"));
}
END_TEST

START_TEST(test_s21_to_upper) {
  char *res1 = s21_to_upper("hello world");
  ck_assert_str_eq(res1, "HELLO WORLD");
  free(res1);

  char *res2 = s21_to_upper("HeLlO 123 WorLD!");
  ck_assert_str_eq(res2, "HELLO 123 WORLD!");
  free(res2);

  char *res3 = s21_to_upper("ALREADY UPPER");
  ck_assert_str_eq(res3, "ALREADY UPPER");
  free(res3);

  char *res4 = s21_to_upper("12345 !@#$");
  ck_assert_str_eq(res4, "12345 !@#$");
  free(res4);

  char *res5 = s21_to_upper("");
  ck_assert_str_eq(res5, "");
  free(res5);

  ck_assert_ptr_null(s21_to_upper(NULL));
}
END_TEST

START_TEST(test_s21_to_lower) {
  char *res1 = s21_to_lower("HELLO WORLD");
  ck_assert_str_eq(res1, "hello world");
  free(res1);

  char *res2 = s21_to_lower("HeLlO 123 WorLD!");
  ck_assert_str_eq(res2, "hello 123 world!");
  free(res2);

  char *res3 = s21_to_lower("already lower");
  ck_assert_str_eq(res3, "already lower");
  free(res3);

  char *res4 = s21_to_lower("12345 !@#$");
  ck_assert_str_eq(res4, "12345 !@#$");
  free(res4);

  char *res5 = s21_to_lower("");
  ck_assert_str_eq(res5, "");
  free(res5);

  ck_assert_ptr_null(s21_to_lower(NULL));
}
END_TEST
Suite *s21_sscanf_suite(void) {
  Suite *s;
  TCase *tc_core;
  TCase *tc_edge;
  TCase *tc_width_len;
  TCase *tc_wide;
  s = suite_create("s21_sscanf");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, sscanf_single_d);
  tcase_add_test(tc_core, sscanf_single_i);
  tcase_add_test(tc_core, sscanf_single_o);
  tcase_add_test(tc_core, sscanf_single_x);
  tcase_add_test(tc_core, sscanf_single_u);
  tcase_add_test(tc_core, sscanf_single_f);
  tcase_add_test(tc_core, sscanf_single_s);
  tcase_add_test(tc_core, sscanf_single_c);
  tcase_add_test(tc_core, sscanf_single_p);
  tcase_add_test(tc_core, sscanf_single_n);
  tcase_add_test(tc_core, sscanf_multiple_specifiers);
  tcase_add_test(tc_core, sscanf_percent_sign);
  suite_add_tcase(s, tc_core);
  tc_width_len = tcase_create("WidthLengthSkip");
  tcase_add_test(tc_width_len, sscanf_skip_with_asterisk);
  tcase_add_test(tc_width_len, sscanf_width);
  tcase_add_test(tc_width_len, sscanf_length_short);
  tcase_add_test(tc_width_len, sscanf_length_long);
  tcase_add_test(tc_width_len, sscanf_length_double);
  tcase_add_test(tc_width_len, sscanf_length_long_double);
  suite_add_tcase(s, tc_width_len);
  tc_wide = tcase_create("WideChars");
  tcase_add_test(tc_wide, sscanf_wide_char);
  tcase_add_test(tc_wide, sscanf_wide_string);
  tcase_add_test(tc_wide, sscanf_mixed_wide_and_narrow);
  suite_add_tcase(s, tc_wide);
  tc_edge = tcase_create("EdgeCases");
  tcase_add_test(tc_edge, sscanf_no_match);
  tcase_add_test(tc_edge, sscanf_whitespace_handling);
  tcase_add_test(tc_edge, sscanf_trailing_chars);
  tcase_add_test(tc_edge, sscanf_numeric_edge_cases);
  suite_add_tcase(s, tc_edge);
  return s;
}

Suite *create_string_suite() {
  Suite *suite = suite_create("s21_string");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strlen);
  tcase_add_test(tc_core, test_strncmp);
  tcase_add_test(tc_core, test_strncpy);
  tcase_add_test(tc_core, test_strncat);
  tcase_add_test(tc_core, test_strchr);
  tcase_add_test(tc_core, test_strstr);
  tcase_add_test(tc_core, test_strtok);

  tcase_add_test(tc_core, test_s21_strcspn);
  tcase_add_test(tc_core, test_s21_strrchr);
  tcase_add_test(tc_core, test_s21_strpbrk);

  suite_add_tcase(suite, tc_core);
  return suite;
}

Suite *create_memory_suite() {
  Suite *suite = suite_create("s21_memory");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_memchr);
  tcase_add_test(tc_core, test_memcmp);
  tcase_add_test(tc_core, test_memcpy);
  tcase_add_test(tc_core, test_memset);

  suite_add_tcase(suite, tc_core);
  return suite;
}

Suite *create_bonus_suite() {
  Suite *suite = suite_create("s21_bonus");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strerror);
  tcase_add_test(tc_core, test_insert);
  tcase_add_test(tc_core, test_trim);

  tcase_add_test(tc_core, test_s21_to_upper);
  tcase_add_test(tc_core, test_s21_to_lower);

  suite_add_tcase(suite, tc_core);
  return suite;
}

Suite *create_sprintf_suite() {
  Suite *suite = suite_create("s21_sprintf");
  TCase *tc_sprintf = tcase_create("Sprintf_Format");

  tcase_add_test(tc_sprintf, test_sprintf_c);
  tcase_add_test(tc_sprintf, test_sprintf_d);
  tcase_add_test(tc_sprintf, test_sprintf_s);
  tcase_add_test(tc_sprintf, test_sprintf_f);
  tcase_add_test(tc_sprintf, test_sprintf_u);
  tcase_add_test(tc_sprintf, test_sprintf_x);
  tcase_add_test(tc_sprintf, test_sprintf_o);
  tcase_add_test(tc_sprintf, test_sprintf_e);
  tcase_add_test(tc_sprintf, test_sprintf_g);
  tcase_add_test(tc_sprintf, test_sprintf_p);
  tcase_add_test(tc_sprintf, test_sprintf_percent);
  tcase_add_test(tc_sprintf, test_sprintf_length_h);
  tcase_add_test(tc_sprintf, test_sprintf_length_l);
  tcase_add_test(tc_sprintf, test_sprintf_length_L);
  tcase_add_test(tc_sprintf, test_sprintf_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_width_precision_star);

  suite_add_tcase(suite, tc_sprintf);
  return suite;
}

int main() {
  int failed = 0;
  Suite *s_string, *s_memory, *s_bonus, *s_sprintf, *s_sscanf;
  SRunner *runner;

  s_string = create_string_suite();
  s_memory = create_memory_suite();
  s_bonus = create_bonus_suite();
  s_sprintf = create_sprintf_suite();
  s_sscanf = s21_sscanf_suite();

  runner = srunner_create(s_string);
  srunner_add_suite(runner, s_memory);
  srunner_add_suite(runner, s_bonus);
  srunner_add_suite(runner, s_sprintf);
  srunner_add_suite(runner, s_sscanf);

  srunner_run_all(runner, CK_NORMAL);

  failed = srunner_ntests_failed(runner);

  srunner_free(runner);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}