#ifndef TERM0408
#define TERM0408

#include <stdarg.h>

typedef void (*_printc_f) (int c, void *priv);

int _print(const char *fmt, va_list ap, _printc_f _putchar, void *priv);

int _vprintf(const char *fmt, va_list ap);

int _printf(const char *fmt, ...);

int _vsnprintf(char *b, size_t sz, const char *fmt, va_list ap);

#endif
