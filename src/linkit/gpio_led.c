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
