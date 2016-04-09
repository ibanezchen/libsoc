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

#include <hcos/irq.h>
#include <hcos/soc.h>
#include <hcos/io.h>
#include <hcos/core.h>
#include <hcos/cpu/cache.h>
#include <hcos/cpu/nvic.h>
#include <hcos/cpu/_cpu.h>
#include <hcos/cfg.h>
#include "uart.h"
#include "_soc.h"
#include "plt.h"

void top_xtal_init(void);

unsigned top_xtal_freq_get(void);

uart_t u0;

static unsigned freq;

unsigned freq_init()
{
	top_xtal_init();
	return top_xtal_freq_get();
}

void soc_init(void)
{
	freq = freq_init();
	uart_init(&u0, BASE_UART0, -1);
}

unsigned irq_mask(unsigned irq)
{
	return nvic_irq_mask(irq);
}

void irq_unmask(unsigned irq)
{
	nvic_irq_unmask(irq);
}

void irq_sgi(unsigned irq)
{
	nvic_sgi(irq);
}

void tmr_enable(int on)
{
	cpu_stick_en(on);
}

int tmr_init_soc(unsigned *rtcs2tick)
{
	cpu_stick_init(freq / HZ);
	//set Priority for Systick Interrupt
	cpu_pri(E_STICK, 5);
	// FIXME: RTC init
	return IRQ_TIME;
}

unsigned soc_rtcs()
{
	return 0;
	//FIXME: return readl(BASE_RTC + 0x4);
}

void tmr_tickless_soc(int next_expire)
{
	//FIXME:
	//if (!tmr_off)
	//      writel(next_expire << 2, (void *)BASE_RTC + 0x0);
}
