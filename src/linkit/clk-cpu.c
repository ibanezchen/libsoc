/*-****************************************************************************/
/*-                                                                           */
/*-            Copyright (c) of hyperCOS.                                     */
/*-                                                                           */
/*-  This software is copyrighted by and is the sole property of socware.net. */
/*-  All rights, title, ownership, or other interests in the software remain  */
/*-  the property of socware.net. The source code is FREE for short-term      */
/*-  evaluation, educational or non-commercial research only. Any commercial  */
/*-  application may only be used in accordance with the corresponding license*/
/*-  agreement. Any unauthorized use, duplication, transmission, distribution,*/
/*-  or disclosure of this software is expressly forbidden.                   */
/*-                                                                           */
/*-  Knowledge of the source code may NOT be used to develop a similar product*/
/*-                                                                           */
/*-  This Copyright notice may not be removed or modified without prior       */
/*-  written consent of socware.net.                                          */
/*-                                                                           */
/*-  socware.net reserves the right to modify this software                   */
/*-  without notice.                                                          */
/*-                                                                           */
/*-  To contact socware.net:                                                  */
/*-                                                                           */
/*-             socware.help@gmail.com                                        */
/*-                                                                           */
/*-****************************************************************************/
#define _DBG 0
#include <hcos/dbg.h>
#include <hcos/core.h>
#include <hcos/io.h>
#include <hcos/clk-cpu.h>
#include "_soc.h"
#include <string.h>

#define CTR	0x830081B0
#define h(_v)	(unsigned short)((1<<6)|(_v))
#define l(_v)	(unsigned short)((2<<6)|(_v))

static unsigned short priv[] = {
	l(52), l(46), l(14), h(30), h(22), h(18), h(14), h(10), h(8),
};

static unsigned short freq[] = {
	12, 20, 40, 60, 80, 96, 120, 160, 196
};

static unsigned short pwr[] = {
	5, 26, 313, 530, 724, 878, 1123, 1481, 1794
};

static int setf(clk_t * o, int idx)
{
	lle_t *lle;
	unsigned h, l, v;
	unsigned short *div = (unsigned short *)o->priv;

	if (idx >= o->freq_n)
		return -1;
	ll_for_each(&o->listeners, lle) {
		clk_listener_t *l = lle_get(lle, clk_listener_t, ll);
		l->evt(l, o, CLK_SETF);
	}
	v = reg(CTR);
	dbg("v = %x\n", v);
	h = ((div[idx] >> 6) & 0x3);
	dbg("h = %x\n", h);
	//v&=~(0x3<<14);
	//v|=  h<< 14;
	v = BI_R_FLD(v, 15, 14, h);
	reg(CTR) = v;
	dbg("v = %x\n", v);
	while (v != reg(CTR)) ;
	v = reg(CTR);
	dbg("v = %x\n", v);
	l = ((div[idx]) & 0x3f);
	dbg("l = %x\n", l);
	///v&=~(0x3f<<4);
	//v|=  l<< 4;
	v = BI_R_FLD(v, 9, 4, l);
	reg(CTR) = v;
	dbg("v = %x\n", v);
	o->idx = idx;
	ll_for_each(&o->listeners, lle) {
		clk_listener_t *l = lle_get(lle, clk_listener_t, ll);
		l->evt(l, o, CLK_SETF_POST);
	}
	return 0;
}

static unsigned getf(clk_t * o)
{
	return o->freq[o->idx] * 1000000;
}

static clk_t clk_cpu = {
	"cpu", 0, setf, 0, 0, getf, {0, 0}, priv, 0,
	sizeof(freq) / sizeof(unsigned short), freq, pwr
};

static void stick(struct clk_listener *o, clk_t * clk, clk_evt_t type)
{
	if (type == CLK_SETF_POST)
		cpu_stick_init(clk_getf(clk) / HZ);
	linkit_freq = clk_getf(clk);
}

static clk_listener_t clk_listener_systick = { stick, {0, 0}, 0 };

clk_t *clk_cpu_init(int cpu)
{
	if (!clk_cpu.listeners.n) {
		ll_init(&clk_cpu.listeners);
		clk_listen(&clk_cpu, &clk_listener_systick);
	}
	return &clk_cpu;
}
