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
#include <hcos/io.h>
#include "uart.h"
#include <string.h>

#define UART_THR(_b)           ((_b)+0x0)
#define UART_DLL(_b)           ((_b)+0x0)
#define UART_DLH(_b)           ((_b)+0x4)
#define UART_IER(_b)           ((_b)+0x4)
#define UART_FCR(_b)           ((_b)+0x8)
#define UART_LCR(_b)           ((_b)+0xc)
#define UART_LSR(_b)           ((_b)+0x14)

#define UART_LSR_THRE     	    0x0020

void uart_init(uart_t * o, unsigned base, unsigned irq)
{
	memset(o, 0, sizeof(uart_t));
	o->base = base;
	o->irq = irq;
}

static void uart_put_ni(uart_t * o, char c)
{
	unsigned base = o->base;
	while (1) {
		unsigned short lsr;
		lsr = readb(UART_LSR(base));
		if (lsr & UART_LSR_THRE) {
			//FIXME:
			if (c == '\n')
				writeb('\r', (void *)UART_THR(base));
			writeb(c, (void *)UART_THR(base));
			break;
		}
	}
}

static int uart_get_ni(uart_t * o)
{
	unsigned base = o->base;
	unsigned char lsr;

	lsr = readb(base + 0x14);
	if ((lsr & 0x1) == 0)
		return -1;
	return readb(base + 0x00);
}

static int uart_get_i(uart_t * o);

void uart_put(uart_t * o, char c)
{
	uart_put_ni(o, c);
}

int uart_get(uart_t * o)
{
	return o->is_int ? uart_get_i(o) : uart_get_ni(o);
}

///< interrupt mode below
#include <hcos/irq.h>
#include <stdio.h>

static irq_handler(uart_irq)
{
	uart_t *o = irq_data();
	uart_b_t *in = &o->in;
	int c;
	while ((c = uart_get_ni(o)) >= 0) {
		if (!uartb_full(in)) {
			in->b[in->t++] = c;
			if (in->t >= in->sz)
				in->t = 0;
			sem_post(&in->sem);
		}
	}
	return 0;
}

static int uart_get_i(uart_t * o)
{
	int c = -1;
	uart_b_t *buf = &o->in;
	sem_get(&o->in.sem, WAIT);
	c = buf->b[buf->h++];
	if (buf->h >= buf->sz)
		buf->h = 0;
	return c;
}

static inline void uartb_init(uart_b_t * buf, char *b, int sz)
{
	buf->b = b;
	buf->sz = sz;
	buf->h = buf->t = 0;
	sem_init(&buf->sem, 0);
}

void uart_int_in(uart_t * o, char *b, int sz)
{
	o->is_int = 1;
	writeb(0x07, (void *)UART_FCR(o->base));	// FIFO
	writeb(readb(UART_IER(o->base)) | 0x01, (void *)UART_IER(o->base));
	irq_init(o->irq, uart_irq);
	uartb_init(&o->in, b, sz);
	irq_bind(o->irq, o);
}
