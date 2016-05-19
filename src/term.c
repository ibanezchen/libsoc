/*-****************************************************************************/
/*-                                                                           */
/*-            Copyright (c) 2013 by SOCware Inc.                             */
/*-                                                                           */
/*-  This software is copyrighted by and is the sole property of SOCware, Inc.*/
/*-  All rights, title, ownership, or other interests in the software remain  */
/*-  the property of SOCware, Inc. The source code is for FREE short-term     */
/*-  evaluation, educational or non-commercial research only. Any commercial  */
/*-  application may only be used in accordance with the corresponding license*/
/*-  agreement. Any unauthorized use, duplication, transmission, distribution,*/
/*-  or disclosure of this software is expressly forbidden.                   */
/*-                                                                           */
/*-  Knowledge of the source code may NOT be used to develop a similar product*/
/*-                                                                           */
/*-  This Copyright notice may not be removed or modified without prior       */
/*-  written consent of SOCware, Inc.                                         */
/*-                                                                           */
/*-  SOCWare, Inc. reserves the right to modify this software                 */
/*-  without notice.                                                          */
/*-                                                                           */
/*-             socware.help@gmail.com                                        */
/*-             http://socware.net                                            */
/*-                                                                           */
/*-****************************************************************************/
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "uart.h"
#include "_soc.h"
#include "term.h"

#define UDIV_R(N, D, R) (((R)=(N)%(D)), ((N)/(D)))

static void _putchar(int c)
{
	if( c == '\r')
		return;
	if( c == '\n')
		uart_put(&u0, '\r');
	uart_put(&u0, c);
}

static void itoa(char *p, unsigned i, unsigned base)
{
	char rev[20 + 1];
	char *s;
	unsigned rem, j;

	rev[20] = 0;
	s = &rev[20];
	j = 8;
	while (i || base == 16) {
		i = UDIV_R(i, base, rem);
		if (rem < 10) {
			*--s = rem + '0';
		} else if (base == 16) {
			*--s = "abcdef"[rem - 10];
		}
		if (base == 16 && --j <= 0)
			break;
	}
	if (s == &rev[20]) {
		p[0] = '0';
		p[1] = 0;
		return;
	}
	while (*s) {
		p[0] = *s++;
		++p;
	}
	*p = 0;
}

static void _print(const char *fmt, va_list ap)
{
	char tmp[20 + 1];
	int ival;
	const char *p, *sval;
	char cval;
	char* np;
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			_putchar(*p);
			continue;
		}
		++p;
		while (isdigit((int)(*p))) {
			p++;
		}
		switch (*p) {
		case 'd':
			ival = va_arg(ap, int);
			if (ival < 0) {
				_putchar('-');
				ival = -ival;
			}
			itoa(tmp, ival, 10);
			for(np = tmp; *np; )
				_putchar(*np++);
			break;
		case 'u':
		case 'U':
		case 'x':
		case 'X':
			ival = va_arg(ap, int);
			itoa(tmp, ival, 16);
			for(np = tmp; *np; )
				_putchar(*np++);
			break;
		case 'c':
			cval = va_arg(ap, int);
			_putchar(cval);
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++) {
				_putchar(*sval);
			}
			break;
		default:
			break;
		}
	}
}

void _printf(const char *str, ...)
{
	if (str == 0)
		return;
	va_list ap;
	va_start(ap, str);
	_print(str, ap);
	va_end(ap);
}
