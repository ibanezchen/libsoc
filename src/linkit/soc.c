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
#define _DBG 0
#include <hcos/dbg.h>
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
#include "term.h"

void top_xtal_init(void);

unsigned top_xtal_freq_get(void);

void cmnCpuClkConfigureTo192M(void);

void cmnSerialFlashClkConfTo64M(void);

uart_t u0;

static unsigned freq;

unsigned clk_init()
{
	top_xtal_init();
	// Enable MCU clock to 192MHz
	cmnCpuClkConfigureTo192M();

	//Enable flash clock to 64MHz
	cmnSerialFlashClkConfTo64M();

	return 192 * 1000000;
//      return top_xtal_freq_get();
}

void soc_idle(int next_expire)
{
	cpu_idle();
}

void soc_init(void)
{
	freq = clk_init();
	uart_init(&u0, BASE_UART0, -1);
}

unsigned irq_mask(unsigned irq)
{
	if (irq == IRQ_TIME) {
		cpu_stick_en(0);
		return 1;
	}
	return nvic_irq_mask(irq);
}

#include "mt7687.h"
void irq_unmask(unsigned irq)
{
	if (irq == IRQ_TIME) {
		cpu_stick_en(1);
		return;
	}
	NVIC_ClearPendingIRQ(irq);
	asm_mb();
	asm_mb();
	asm_mb();
	NVIC_DisableIRQ(irq);
	asm_mb();
	NVIC_ClearPendingIRQ(irq);
	asm_mb();
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

irq_handler(rtc_irq)
{
	reg(BASE_GPT + 0x0) = 0x2;
	dbg("rtc= %x\r\n", soc_rtcs());
	return IRQ_DONE;
}

int tmr_init_soc(unsigned *rtcs2tick, unsigned *hz)
{
	cpu_stick_init(freq / HZ);
	//set Priority for Systick Interrupt
	cpu_pri(E_STICK, 5);
	//enable tickless timer
	nvic_set_pri(IRQ_GPT, 5);
	irq_init(IRQ_GPT, rtc_irq);
	// HZ=128, gpt=1024
	*rtcs2tick = 3;
	irq_unmask(IRQ_GPT);

	//enable gpt0 for count
	reg(BASE_GPT + 0x14) = 0xffffffff;
	reg(BASE_GPT + 0x10) = 1;

	//enable gpt1 for interrupt
	reg(BASE_GPT + 0x04) = 2;
	*hz = HZ;
	return IRQ_TIME;
}

unsigned soc_rtcs()
{
	return 0xffffffff - readl(BASE_GPT + 0x40);
}

void tmr_tickless_soc(unsigned next_expire)
{
	unsigned max = (0xffffffff >> 3);
	unsigned dur = next_expire > max ? max : next_expire;
	dbg("rtc_book= %x ++ %x %x %x\r\n", soc_rtcs(), dur << 3, max,
	    next_expire);
	reg(BASE_GPT + 0x24) = dur << 3;
	reg(BASE_GPT + 0x20) = 0x1;
}

void cache_i_inv(unsigned sta, unsigned sz)
{
}

void cache_d_flu(unsigned sta, unsigned sz)
{
}
