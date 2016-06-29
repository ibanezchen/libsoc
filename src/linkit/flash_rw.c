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
