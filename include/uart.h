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
#ifndef UART0721
#define UART0721

#include <hcos/sem.h>

typedef struct {
	sem_t sem;
	unsigned short h, t;
	int sz;
	char *b;
} uart_b_t;

static inline int uartb_full(uart_b_t * buf)
{
	return buf->sem.val >= buf->sz;
}

static inline int uartb_empty(uart_b_t * buf)
{
	return !buf->sem.val;
}

typedef struct {
	unsigned base;
	unsigned short irq, is_int;
	uart_b_t in;
} uart_t;

void uart_init(uart_t * o, unsigned base, unsigned irq);

void uart_baud(uart_t * o, unsigned clk, unsigned baud);

void uart_put(uart_t * o, char c);

int uart_get(uart_t * o);

///< use interrupt on uart input
void uart_int_in(uart_t * o, char *b, int sz);

static inline int uart_w(uart_t * o, const char *buf, int n)
{
	while (n--) {
		char c = *buf++;
		uart_put(o, c);
	}
	return n;
}

#endif
