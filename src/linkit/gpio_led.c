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
#include "hal_gpio.h"
#include "term.h"

static void gpio_led(void *priv)
{
	int i = 0;
	int pin = 24;

	hal_gpio_init(pin);
	hal_pinmux_set_function(pin, 8);
	hal_gpio_set_direction(pin, HAL_GPIO_DIRECTION_OUTPUT);
	while (1) {
		_printf("i = %d\r\n", i++);
		task_sleep(1500);
		hal_gpio_set_output(pin, HAL_GPIO_DATA_LOW);
		task_sleep(1500);
		hal_gpio_set_output(pin, HAL_GPIO_DATA_HIGH);
	}
}

int main(void)
{
	core_init();
	task_new("gpio_led", gpio_led, 56, 1024, -1, (void *)50);
	core_start();
	return 0;
}

#endif
