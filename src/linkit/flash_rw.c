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

#if _EXE_

#include <hcos/task.h>
#include <hcos/soc.h>
#include <string.h>
#include <stdio.h>
#include "hal_flash.h"
#include "term.h"

char *data = "1234567890abcdef";
unsigned char buf[16];

static void flash_rw(void *priv)
{
	hal_flash_init();
	hal_flash_erase(0, HAL_FLASH_BLOCK_4K);
	hal_flash_write(0, (void *)data, 16);
	memset(buf, 0, sizeof(buf));
	hal_flash_read(0, buf, sizeof(buf));
	_printf("buf %s\n", buf);
}

int main(void)
{
	core_init();
	task_new("flash_rw", flash_rw, 56, 1024, -1, (void *)50);
	core_start();
	return 0;
}

#endif
