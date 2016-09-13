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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <hcos/sem.h>

#include "term.h"
#include "plt.h"
#include "heap-mem.h"
#include "uart.h"
#include "_soc.h"
#include "plt.h"

void plt_init(void)
{
}

static void _printu(int c, void *_uart)
{
	uart_t *u = (uart_t *) _uart;
	if (c == '\r')
		return;
	if (c == '\n')
		uart_put(u, '\r');
	uart_put(u, c);
}

int plt_print(const char *buf, int n)
{
	while (n--) {
		char c = *buf++;
		_printu(c, &u0);
	}
	return n;
}

int _vprintf(const char *fmt, va_list ap)
{
	return _print(fmt, ap, _printu, &u0);
}

int main(int argc, char **argv);

int _main(void)
{
	char *s, *d, *h;
	int i, l, argc = 0;
	char **argv = 0;

	h = (char *)(BASE_SRAM + SZ_SRAM - 256);
	if (*(unsigned *)h == 0xBEEFBEEF) {
		h += 4;
		argc = 1;
		for (s = h; *s;) {
			l = strlen(s);
			d = core_alloc(l + 1, 0);
			strcpy(d, s);
			s += (l + 1);
			argc++;
		}
		argv = (char **)core_alloc(sizeof(char *) * argc, 2);
		i = 0;
		argv[i++] = "app";
		for (s = h; *s; s += strlen(s) + 1)
			argv[i++] = s;
		argv[i] = 0;
	}
	return main(argc, argv);
}
