#ifndef __SNTP_H__
#define __SNTP_H__

typedef void (*sntp_set_time_t) (long t);

///<    @param vararg list of sntp server, terminated with 0
///<    e.g. sntp_init(_settime, "pool.ntp.org", 0)
void sntp_init(sntp_set_time_t set_time, ...);

///<    wait until at lease one response
void sntp_wait();

#endif /* __SNTP_H__ */
