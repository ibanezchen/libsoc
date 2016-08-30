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
#ifndef CLK1605GG
#define CLK1605GG

#include <hcos/ll.h>

typedef enum { CLK_PRE, CLK_POST } clk_notify_t;

struct clk;

typedef struct clk clk_t;

typedef struct clk_listener {
	int setf(struct clk_listener *o, clk_t * clk, clk_notify_t type,
		 int idx);
	int en(struct clk_listener *o, clk_t * clk, clk_notify_t type,
	       int enabled);
	lle_t ll;
	void *priv;
} clk_listener_t;

struct clk {
	clk_t *parent;
	int setf(clk_t * o, int idx);
	int en(clk_t * o, int enabled);
	int is_en(clk_t * o);
	unsigned get_freq(clk_t * o);
	ll_t listeners;
	void *priv;
	unsigned short freq_n;
	unsigned short freq[0];
};

static inline void clk_subs(clk_t * o, clk_listener_t listener)
{
	ll_addt(&o->listeners, &listener->ll);
}

#endif
