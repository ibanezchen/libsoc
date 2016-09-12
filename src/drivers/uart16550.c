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
	o->base = base;
	o->irq = irq;
}

void uart_put(uart_t * o, char c)
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

int uart_get(uart_t * o)
{
	unsigned base = o->base;
	unsigned char lsr;

	lsr = readb(base + 0x14);
	if ((lsr & 0x1) == 0)
		return -1;
	return readb(base + 0x00);
}
