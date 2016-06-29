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
#ifndef MALLOC0520
#define MALLOC0520

#include <hcos/mut.h>

typedef struct _fmb {
	struct _fmb *next;
	unsigned sz;
} hblk_t;

typedef struct {
	hblk_t head;
	hblk_t *end;
	unsigned remaining;
	unsigned remaining_low;
	mut_t *mut;
} heap_t;

void *heap_alloc(heap_t * o, unsigned sz);

void heap_free(heap_t * o, void *p);

void *heap_realloc(heap_t * o, void *pv, unsigned sz);

void heap_init(heap_t * o, unsigned *buf, unsigned sz, mut_t * mut);

#endif
