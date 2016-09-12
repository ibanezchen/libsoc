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
#ifndef SOC130408
#define SOC130408

#include "uart.h"

enum {
	BASE_TOPCFG = 0x83008000,
	BASE_UART0 = 0x83030000,
	BASE_GPT = 0x83050000,
	BASE_TCM = 0x100000,
	BASE_SRAM = 0x20000000,
	BASE_FLASH = 0x10000000,
	BASE_PINMUX = 0x81023000,
};

enum {
	IRQ_TIME = 0xffffffff,
	IRQ_UART0 = 0,
	IRQ_GPT = 24,
};

static inline unsigned tmr_ticks2ms(unsigned ticks)
{
	return ticks * 10;
}

static inline unsigned tmr_ms2ticks(unsigned ms)
{
	return ms / 10;
}

extern uart_t u0, u1;

#endif
