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
