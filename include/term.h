#ifndef TERM0408
#define TERM0408

#include <stdarg.h>

void _print(const char *fmt, va_list ap, char *buf, unsigned sz);

void _printf(const char *fmt, ...);

#endif
