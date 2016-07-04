#if _EXE_

#include <hcos/task.h>
#include <hcos/soc.h>
#include <string.h>
#include <stdio.h>
#include "term.h"
#include "flash.h"

char *data = "1234567890abcdef";
unsigned char buf[16];

static void flash_io(void *priv)
{
	unsigned tar = 1 << 20;
	_flash_init();
	_flash_erase(tar, flash_page);
	_flash_write(tar, 16, (void *)data);
	memset(buf, 0, sizeof(buf));
	_flash_read(tar, sizeof(buf), buf);
	_printf("buf %s\n", buf);
}

int main(void)
{
	core_init();
	task_new("flash_io", flash_io, 56, 2048, -1, 0);
	core_start();
	return 0;
}

#endif
