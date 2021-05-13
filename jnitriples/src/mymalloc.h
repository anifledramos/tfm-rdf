#ifndef ___MYMALLOC
#define ___MYMALLOC

#include <stdlib.h>

long inicializaMemoria();

void * mymalloc(size_t size);

void * mymalloc2(size_t size);

long getMemoria();

#endif
