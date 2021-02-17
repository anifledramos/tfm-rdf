#include <stdio.h>
#include <math.h>
#include <string.h>
#include <k2triples-common.h>
/*
void storeK2TRIPLES(K2TRIPLES *k2triples, char * filename);

K2TRIPLES * loadK2TRIPLES(char * filename);
*/

void storeK2TRIPLES(K2TRIPLES *k2triples, FILE * f);

K2TRIPLES * loadK2TRIPLES(FILE * f);

size_t sizeK2TRIPLES(K2TRIPLES *k2triples);

void destroyK2TRIPLES(K2TRIPLES *k2triples);
