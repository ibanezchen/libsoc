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
#include <hcos/task.h>
#include <hcos/soc.h>
#include <hcos/io.h>
#include <hcos/cpu/nvic.h>
#include "_soc.h"

#include <string.h>
#include <stdio.h>

#include "term.h"

static volatile float g = 3.14;

void fast(void *priv)
{
	unsigned ts = (unsigned)priv;
	g = g + g;
	task_sleep(10);
	irq_sgi(12);
	task_sleep(10);
	while (1) {
		_printf("fast %d\r\n", ts);
		task_sleep(ts);
	}
}

void slow(void *priv)
{
	unsigned ts = (unsigned)priv;
	while (1) {
		_printf("slow %d %x\r\n", ts, soc_rtcs());
		task_sleep(ts);
	}

}

irq_handler(isr_use_float)
{
	g += 10.2;
	return IRQ_DONE;
}

#if _EXE_
int main(int argc, char** argv)
{
	int i;
	core_init();
	for( i = 0 ; i < argc; i++)
		printf("argv[%d] = %s\r\n", i, argv[i]);
	irq_init(12, isr_use_float);
	task_new("fast", fast, 56, 1024, -1, (void *)HZ);
	task_new("slow", slow, 56, 1024, -1, (void *)(HZ * 2));
	core_start();
	return 0;
}

#endif
