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
#include <hcos/dbg.h>
#include <hcos/cpu.h>
#include <hcos/task.h>
#include <hcos/cpu/reg.h>
#include <stdio.h>
#include "term.h"

void _exit(int status)
{
	unsigned *p = (unsigned *)0xB0100004;
	*p = status;
	while (1) ;
}

void _fail(void *addr, const char *f, unsigned line)
{
	_printf("_fail 0x%08x %s:%d\r\n", (unsigned)addr, f, line);
	_exit(-1);
}

void _abt(void *_ctx)
{
	while (1) ;
}

void _stackov(task_t * t)
{
	_printf("stack ov\r\n");
	while (1) ;
}
