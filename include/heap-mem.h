/**
*	@file heap-mem.h
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
