#ifndef MALLOC0520
#define MALLOC0520

void _free(void *pv);

void *_malloc(unsigned sz);

void *_calloc(unsigned n, unsigned sz);

void *_realloc(void *pv, unsigned sz);

void malloc_init(unsigned *buf, unsigned sz);

#endif
