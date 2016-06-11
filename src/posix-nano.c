#include <hcos/tmr.h>
#include <hcos/task.h>
#include <signal.h>
#include <string.h>

unsigned PLT_HZ = HZ;

static tmr_t at;

static unsigned alarm_ticks;

static int _alarm(void *p)
{
	alarm_ticks = 0;
	raise(SIGALRM);
	return 0;
}

unsigned alarm(unsigned seconds)
{
	unsigned remaining;
	tmr_init(&at, 0, _alarm);
	tmr_on(&at, seconds * HZ);
	if (!alarm_ticks)
		remaining = 0;
	else
		remaining = (alarm_ticks - tmr_ticks) / HZ;
	alarm_ticks = tmr_ticks + seconds * HZ;
	return remaining;
}

unsigned int sleep(unsigned int seconds)
{
	task_sleep(seconds * HZ);
	return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	//FIXME:
	return 0;
}

int daemon(int a, int b)
{
	return 0;
}

void gethostname(char* b, int sz)
{
	strncpy(b, "hc", sz);
}
