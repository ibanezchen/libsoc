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

#include "plt.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/time.h>
#include <hcos/tmr.h>

int _close(int fd)
{
	return -1;
}

int _stat(const char *filename, struct stat *st)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	return -1;
}

int _getpid(void)
{
	return 1;
}

static unsigned toff;

void _settime(long ts)
{
	toff = ts - (long)(tmr_ticks / HZ);
}

int _gettimeofday(struct timeval *tp, void *tzp)
{
	tp->tv_sec = toff + (long)(tmr_ticks / HZ);
	tp->tv_usec = 0;
	return 0;
}

int _isatty(int fd)
{
	return (fd ^ STDOUT_FILENO) ^ (fd ^ STDERR_FILENO);
}

int _kill(int pid, int sig)
{
	return 0;
}

off_t _lseek(int fd, off_t off, int whence)
{
	return -1;
}

int _open(const char *buf, int flags, int mode)
{
	return -1;
}

int _write(int fd, const char *buf, int nbytes)
{
	switch (fd) {
	case 0:
	case 1:
		return plt_print(buf, nbytes);
	}
	return 0;
}

int _read(int fd, char *buf, int nbytes)
{
	return 0;
}

void *core_alloc(unsigned sz, int align_bits);

char *_sbrk(int nbytes)
{
	void *p = core_alloc(nbytes, 0);
	if ((unsigned)p > SOC_SBRK_MAX) {
		_printf("sbrk over flow\r\n");
		while (1) ;
	}
	return p;
}

int _rename(const char *oldpath, const char *newpath)
{
	return -1;
}

int _unlink(const char *pathname)
{
	return -1;
}

clock_t _times(struct tms * buf)
{
	memset(buf, 0, sizeof(struct tms));
	return 0;
}
