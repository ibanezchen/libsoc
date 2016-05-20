/**
*	@file _malloc.c
*
*	Copyright 2016 [socware.net]
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*/
#include "_malloc.h"
#include <stdlib.h>
#include <string.h>
#include "term.h"

#define _DBG 1
#define CFG_ASSERT 2
#include <hcos/dbg.h>
#include <hcos/mut.h>

#define MSK_USED		(1U<<31)

#define MSK_ALIGN		0x7U

#define MIN_BLOCK_SIZE		((unsigned )(sizeof(fmb_t) * 2 ))

typedef struct _fmb {
	struct _fmb *next;
	unsigned sz;
} fmb_t;

static mut_t mut;

static fmb_t fmb_sta, *fEnd = 0;

static unsigned remaining, remaining_low;

static int no_clib;

static void fmb_add(fmb_t * new_fmb)
{
	fmb_t *fmb;
	void *p;

	for (fmb = &fmb_sta; fmb->next < new_fmb; fmb = fmb->next) ;

	p = fmb;
	if ((p + fmb->sz) == (void *)new_fmb) {
		fmb->sz += new_fmb->sz;
		new_fmb = fmb;
	}

	p = new_fmb;
	if ((p + new_fmb->sz) == (void *)fmb->next) {
		if (fmb->next != fEnd) {
			new_fmb->sz += fmb->next->sz;
			new_fmb->next = fmb->next->next;
		} else {
			new_fmb->next = fEnd;
		}
	} else {
		new_fmb->next = fmb->next;
	}

	if (fmb != new_fmb) {
		fmb->next = new_fmb;
	}
}

void *_malloc(unsigned sz)
{
	void *p = 0;
	fmb_t *fmb, *fprev, *fnew;

	if (!no_clib)
		return malloc(sz);

	mut_lock(&mut, WAIT);
	_assert(fEnd != 0);
	_assert((sz & MSK_USED) == 0);
	_assert(sz > 0);

	sz += sizeof(fmb_t);

	if ((sz & MSK_ALIGN) != 0x00) {
		sz = (sz + MSK_ALIGN) & ~MSK_ALIGN;
	}
	_assert((sz & MSK_ALIGN) == 0);

	if ((sz <= remaining)) {
		fprev = &fmb_sta;
		fmb = fmb_sta.next;
		while ((fmb->sz < sz) && (fmb->next != 0)) {
			fprev = fmb;
			fmb = fmb->next;
		}

		if (fmb == fEnd)
			goto out;
		p = ((void *)fprev->next) + sizeof(fmb_t);

		fprev->next = fmb->next;

		if ((fmb->sz - sz) > MIN_BLOCK_SIZE) {
			fnew = ((void *)(fmb)) + sz;
			_assert((((unsigned)fnew) & MSK_ALIGN) == 0);

			fnew->sz = fmb->sz - sz;
			fmb->sz = sz;

			fmb_add((fnew));
		}

		remaining -= fmb->sz;
		if (remaining < remaining_low) {
			remaining_low = remaining;
		}
		fmb->sz |= MSK_USED;
		fmb->next = 0;
	}
	_assert(((unsigned)p & MSK_ALIGN) == 0);
out:
	mut_unlock(&mut);
	dbg("malloc %x %d\r\n", p, sz);
	return p;
}

void _free(void *p)
{
	fmb_t *fmb;

	if (!no_clib) {
		free(p);
		return;
	}
	dbg("free %x\r\n", p);
	if (!p)
		return;
	fmb = (p - sizeof(fmb_t));

	_assert((fmb->sz & MSK_USED) != 0);
	_assert(fmb->next == 0);
	_assert(fmb->sz & MSK_USED);
	_assert(fmb->next == 0);
	fmb->sz &= ~MSK_USED;
	mut_lock(&mut, WAIT);
	remaining += fmb->sz;
	fmb_add(fmb);
	mut_unlock(&mut);
}

extern unsigned core_heap;

void malloc_init(void)
{
	unsigned addr, total_sz, ucHeap;
	fmb_t *fmb;
	void *p;

	no_clib = 1;
	mut_init(&mut);

	ucHeap = core_heap;
	total_sz = SOC_SBRK_MAX - SOC_CLIB_RESV - ucHeap;
	core_heap = SOC_SBRK_MAX - SOC_CLIB_RESV;

	addr = (unsigned)ucHeap;

	if ((addr & MSK_ALIGN) != 0) {
		addr += MSK_ALIGN;
		addr &= ~MSK_ALIGN;
		total_sz -= addr - ucHeap;
	}

	p = (void *)addr;

	fmb_sta.next = (void *)p;
	fmb_sta.sz = (unsigned)0;

	addr = ((unsigned)p) + total_sz;
	addr -= sizeof(fmb_t);
	addr &= ~MSK_ALIGN;
	fEnd = (void *)addr;
	fEnd->sz = 0;
	fEnd->next = 0;

	fmb = (void *)p;
	fmb->sz = addr - (unsigned)fmb;
	fmb->next = fEnd;

	remaining_low = fmb->sz;
	remaining = fmb->sz;
}

void *_calloc(unsigned n, unsigned sz)
{
	void *p = 0;
	if (!no_clib)
		return calloc(n, sz);
	p = _malloc(n * sz);
	if (p)
		memset(p, 0, n * sz);
	return p;
}

void *_realloc(void *pv, unsigned sz)
{
	unsigned osz = 0;
	fmb_t *fmb = 0;
	void *p = 0;

	if (!no_clib)
		return realloc(pv, sz);
	p = _calloc(sz, 1);
	if (pv) {
		fmb = pv - sizeof(fmb_t);
		_assert((fmb->sz & MSK_USED) != 0);
		_assert(fmb->next == 0);
		osz = (fmb->sz & ~MSK_USED);
		osz = (sz - sizeof(fmb_t));

		if (osz < sz)
			memcpy(p, pv, osz);
		else
			memcpy(p, pv, sz);
		_free(pv);
	}
	return p;
}
