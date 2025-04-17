# String.h Analog Project
## Project Overview
This **group project** is an implementation of a custom string.h library in C, including memory manipulation functions, string operations, formatted input/output (sprintf/sscanf), and additional string utilities. 
The implementation closely follows the behavior of the standard C library functions.
## Project Structure
├── Makefile
├── s21_sprintf.c
├── s21_string.c
├── s21_string.h
├── scanf
│   ├── s21_scanf.c
│   └── s21_scanf.h
└── tests
|    └── test_string.c
## My Contributions
### Memory Manipulation Functions (mem)
- memchr - Locates first occurrence of character in memory block
- memcmp - Compares two memory blocks
- memcpy - Copies memory block
- memset - Fills memory block with constant byte
### Formatted Input/Output
sscanf - Implemented with support for:
| Specifier | Description |
|--|--|
| %c | Single character Signed decimal integer |
%d	| Signed decimal integer
%i	| Signed integer (detects base)
%e, %E, %f, %g, %G	| Floating-point numbers
%o	| Unsigned octal
%s	| String of characters
%u	| Unsigned decimal
%x, %X	| Unsigned hexadecimal
%p	| Pointer address
%n	| Stores character count
%%	|Literal %
### Supported Modifiers:
- Width: %10s (max characters to read)
- Suppression: %*d (read but don't store)
#### Length:
- %hd - short int
- %ld - long int
- %Lf - long double
### Error Handling
strerror - Implemented with OS-specific error messages for both Linux and macOS
### Build System
Created Makefile with targets for:
- Building static library (s21_string.a)
- Running tests
- Generating coverage reports
- Cleaning build artifacts
