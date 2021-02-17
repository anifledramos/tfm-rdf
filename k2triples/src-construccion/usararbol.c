#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#include "rdfSimpleQueries.h"
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

#define SEC_TIME_DIVIDER  ((double) 1.0    )     //1 sec = 1 sec
#define SEC_TIME_UNIT       "sec"

#define TAMVECTORES 2048*1000
//#define TAMVECTORES 2048*1000

int main(int argc, char* argv[]) {
	double secs;
	int i;
	struct timeval tv1, tv2;
	FILE * queries;
	int * queriesDat[4];

	inicializaMemoria();

	if (argc < 3) {
		fprintf(stderr, "USAGE: %s <GRAPH> <nPred>\n",
				argv[0]);
		return (-1);
	}

	int numPredicados = atoi(argv[2]);


	TREP ** treps = (TREP *) mymalloc(sizeof(TREP) * numPredicados);

	char nomFich[256];
	long numBytes;
	
	for (i = 1; i <= numPredicados; i++) {
		sprintf(nomFich, "%s%d.txt", argv[1], i);
		treps[i] = loadTreeRepresentation(nomFich);
		if (!(i%1000)) fprintf(stderr, "%d\n", i);
	}

	numBytes=getMemoria();
	fprintf(stderr,"ESPACIO K2-TRIPLES EN MEMORIA:%ld bytes\n",numBytes);

	return 0;
}

