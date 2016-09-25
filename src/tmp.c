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
#include "uart.h"
#include <string.h>
#include <stdio.h>
#include "term.h"

void tmp(void *priv)
{
	int i = 0;
	uart_imode(&u0);
	while(1){
		printf("%d\n", i++);
		task_sleep(tmr_hz);
	}
}

#if _EXE_
int main(int argc, char **argv)
{
	int i;
	core_init();
	for (i = 0; i < argc; i++)
		printf("argv[%d] = %s\r\n", i, argv[i]);
	task_new("slow", tmp, 56, 1024, -1, (void *)(HZ * 1));
	core_start();
	return 0;
}

#endif
