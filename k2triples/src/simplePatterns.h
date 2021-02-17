#ifndef ___SIMPLEPATTERNS
#define ___SIMPLEPATTERNS

#include "kTree.h"
#include "k2triples-common.h"
#include "commons.h"



void initBuffer();

int spo(K2TRIPLES * k2triples,int s,int p,int o);

int* so(K2TRIPLES * k2triples,  int s,int o);

int* soIndex(K2TRIPLES * k2triples, int s,int o);

int* soDobleIndex(K2TRIPLES * k2triples,  int s, int o);

int *sp(K2TRIPLES * k2triples, int s,int p);

int *po(K2TRIPLES * k2triples, int p,int o);

int **s(K2TRIPLES * k2triples, int s);

int **sIndex(K2TRIPLES * k2triples,int s);

int **p(K2TRIPLES * k2triples, int p);

int **o(K2TRIPLES * k2triples,int o);

int **oIndex(K2TRIPLES * k2triples,int o);

int **pOrdenado(K2TRIPLES * k2triples,  int p);

int propagarSP(K2TRIPLES * k2triples, int s,int p);
#endif
