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
#ifndef PLT0419
#define PLT0419

#include <lwip/netif.h>
#include "heap-mem.h"
#include "sntp.h"

extern unsigned PLT_HZ;
/// @return system clock
unsigned freq_init(void);

extern heap_t plt_tcm;

void plt_init(void);

typedef void (*net_ipchange_t) (const struct netif * netif);

void net_init(char *mac_addr, ...);

void ip_static(char *ip, char *msk, char *gw, char *dns);

void ip_dhcp();

void _settime(long ts);

#define plt_sntp(_servers...)		sntp_init(_settime, ##_servers, 0)

#define PRINTF_FLOAT	asm (".global _printf_float")
#define SCANF_FLOAT	asm (".global _scanf_float")

#endif
