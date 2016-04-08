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

#include "uart.h"
#include "_soc.h"

#define UDIV_R(N, D, R) (((R)=(N)%(D)), ((N)/(D)))

static void itoa(char **buf, unsigned i, unsigned base)
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
		(*buf)[0] = '0';
		++(*buf);
		return;
	}
	while (*s) {
		(*buf)[0] = *s++;
		++(*buf);
	}
}

void _print(char *fmt, va_list ap)
{
	char buf[160];
	int ival;
	char *p, *sval;
	char *bp, cval;

	bp = buf;
	*bp = 0;

	for (p = fmt; *p; p++) {
		if (*p != '%') {
			*bp++ = *p;
			continue;
		}
		switch (*++p) {
		case 'd':
			ival = va_arg(ap, int);
			if (ival < 0) {
				*bp++ = '-';
				ival = -ival;
			}
			itoa(&bp, ival, 10);
			break;

		case 'x':
			ival = va_arg(ap, int);
			*bp++ = '0';
			*bp++ = 'x';
			itoa(&bp, ival, 16);
			break;
		case 'c':
			cval = va_arg(ap, int);
			*bp++ = cval;
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++) {
				*bp++ = *sval;
			}
			break;
		default:
			break;
		}
	}

	*bp = 0;
	for (bp = buf; *bp; bp++) {
		uart_put(&u0, *bp);
	}
}

void _printf(char *str, ...)
{
	if (str == 0) {
		return;
	}

	va_list ap;
	va_start(ap, str);
	_print(str, ap);
	va_end(ap);
}

