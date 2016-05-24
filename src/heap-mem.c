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
#include "heap-mem.h"
#include <stdlib.h>
#include <string.h>
#include "term.h"

#define _DBG 0
#include <hcos/dbg.h>

#define MSK_USED		(1U<<31)

#define MSK_ALIGN		0x7U

#define MIN_BLOCK_SIZE		((unsigned )(sizeof(hblk_t) * 2 ))

static void addblk(heap_t * o, hblk_t * new_fmb)
{
	hblk_t *fmb;
	void *p;

	for (fmb = &o->head; fmb->next < new_fmb; fmb = fmb->next) ;

	p = fmb;
	if ((p + fmb->sz) == (void *)new_fmb) {
		fmb->sz += new_fmb->sz;
		new_fmb = fmb;
	}

	p = new_fmb;
	if ((p + new_fmb->sz) == (void *)fmb->next) {
		if (fmb->next != o->end) {
			new_fmb->sz += fmb->next->sz;
			new_fmb->next = fmb->next->next;
		} else {
			new_fmb->next = o->end;
		}
	} else {
		new_fmb->next = fmb->next;
	}

	if (fmb != new_fmb) {
		fmb->next = new_fmb;
	}
}

void *heap_alloc(heap_t * o, unsigned sz)
{
	void *p = 0;
	hblk_t *fmb, *fprev, *fnew;

	if (o->mut)
		mut_lock(o->mut, WAIT);
	_assert(o->end != 0);
	_assert((sz & MSK_USED) == 0);
	_assert(sz > 0);

	sz += sizeof(hblk_t);

	if ((sz & MSK_ALIGN) != 0x00) {
		sz = (sz + MSK_ALIGN) & ~MSK_ALIGN;
	}
	_assert((sz & MSK_ALIGN) == 0);

	if ((sz <= o->remaining)) {
		fprev = &o->head;
		fmb = o->head.next;
		while ((fmb->sz < sz) && (fmb->next != 0)) {
			fprev = fmb;
			fmb = fmb->next;
		}

		if (fmb == o->end)
			goto out;
		p = ((void *)fprev->next) + sizeof(hblk_t);

		fprev->next = fmb->next;

		if ((fmb->sz - sz) > MIN_BLOCK_SIZE) {
			fnew = ((void *)(fmb)) + sz;
			_assert((((unsigned)fnew) & MSK_ALIGN) == 0);

			fnew->sz = fmb->sz - sz;
			fmb->sz = sz;

			addblk(o, (fnew));
		}

		o->remaining -= fmb->sz;
		if (o->remaining < o->remaining_low) {
			o->remaining_low = o->remaining;
		}
		fmb->sz |= MSK_USED;
		fmb->next = 0;
	}
	_assert(((unsigned)p & MSK_ALIGN) == 0);
out:
	if (o->mut)
		mut_unlock(o->mut);
	dbg("malloc %x %d\r\n", p, sz);
	return p;
}

void heap_free(heap_t * o, void *p)
{
	hblk_t *fmb;

	dbg("free %x\r\n", p);
	if (!p)
		return;
	fmb = (p - sizeof(hblk_t));

	_assert((fmb->sz & MSK_USED) != 0);
	_assert(fmb->next == 0);
	_assert(fmb->sz & MSK_USED);
	_assert(fmb->next == 0);
	fmb->sz &= ~MSK_USED;
	if (o->mut)
		mut_lock(o->mut, WAIT);
	o->remaining += fmb->sz;
	addblk(o, fmb);
	if (o->mut)
		mut_unlock(o->mut);
}

void *heap_realloc(heap_t * o, void *pv, unsigned sz)
{
	unsigned osz = 0;
	hblk_t *fmb = 0;
	void *p = 0;

	p = heap_alloc(o, sz);
	if (pv) {
		fmb = pv - sizeof(hblk_t);
		_assert((fmb->sz & MSK_USED) != 0);
		_assert(fmb->next == 0);
		osz = (fmb->sz & ~MSK_USED);
		osz = (sz - sizeof(hblk_t));

		if (osz < sz)
			memcpy(p, pv, osz);
		else
			memcpy(p, pv, sz);
		heap_free(o, pv);
	}
	return p;
}

void heap_init(heap_t * o, unsigned *buf, unsigned sz, mut_t * mut)
{
	unsigned addr, total_sz, heap;
	hblk_t *fmb;
	void *p;

	memset(o, 0, sizeof(heap_t));
	o->mut = mut;

	heap = (unsigned)buf;
	total_sz = sz;

	addr = (unsigned)heap;

	if ((addr & MSK_ALIGN) != 0) {
		addr += MSK_ALIGN;
		addr &= ~MSK_ALIGN;
		total_sz -= addr - heap;
	}

	p = (void *)addr;

	o->head.next = (void *)p;
	o->head.sz = (unsigned)0;

	addr = ((unsigned)p) + total_sz;
	addr -= sizeof(hblk_t);
	addr &= ~MSK_ALIGN;
	o->end = (void *)addr;
	o->end->sz = 0;
	o->end->next = 0;

	fmb = (void *)p;
	fmb->sz = addr - (unsigned)fmb;
	fmb->next = o->end;

	o->remaining_low = fmb->sz;
	o->remaining = fmb->sz;
}
