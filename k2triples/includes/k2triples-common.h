
#ifndef k2triples_H
#define k2triples_H

//#include <stdio.h>
//#include <math.h>
//#include <string.h>
#include <kTreeAux.h>
#include <commons.h>


typedef struct SK2TRIPLES {
	uint npreds;
	uint nso;
	TREP **trees;
	INDICEDAC * indiceSP;
	INDICEDAC * indiceOP;
} K2TRIPLES;

#endif
