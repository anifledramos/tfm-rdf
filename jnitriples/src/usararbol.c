#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#include <sys/time.h>
#include <time.h>
#include "directcodesI.h"
#include "commons.h"
#include "simplePatterns.h"
#include "k2triples-ops.h"
#define SEC_TIME_DIVIDER  ((double) 1.0    )     //1 sec = 1 sec
#define SEC_TIME_UNIT       "sec"


int main(int argc, char* argv[]) {
	int i,k;
	struct timeval tv1, tv2;
	FILE * queries;
	int * queriesDat[4];
	int tipoQuery;

	if (argc < 6) {
		fprintf(
				stderr,
				"USAGE: %s <GRAPH> <queries> <nQueries> <|SO|> <tipoQuery> [iter]\n",
				argv[0]);
		return (-1);
	}

	for (i = 0; i < 100; i++) {
		printf("FOO\n");
		fflush(stdout);
	}

	inicializaMemoria();

	int numSO=atoi(argv[4]);
	tipoQuery = atoi(argv[5]);
	int numPredicados = 0;
	int numQueries = atoi(argv[3]);


	int numIteraciones=atoi(argv[6]);

	if (argc==7){
		numIteraciones=atoi(argv[6]);
		fprintf(stderr,"Iteraciones:%d\n",numIteraciones);
	}
	else numIteraciones=1;

	FILE * input = fopen(argv[1], "r");
	K2TRIPLES * k2triples = loadK2TRIPLES(input);
	fclose(input);
	k2triples->nso = numSO;

	fprintf(stderr,"numpredicados: %d\n",k2triples->npreds);
	fprintf(stderr,"num queries:%d\n",numQueries);
	fprintf(stderr,"Indice SP:\n");	
	inicializarEstructuras(k2triples->npreds, k2triples->nso);
	
	for (i = 0; i < 4; i++) {
		queriesDat[i] = (int *) malloc(sizeof(int) * numQueries);
	}

	if ((queries = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "error: no se pudo abrir el fichero %s\n", argv[2]);
		return 1;
	} else {
		// fprintf(stderr, "%s\n", argv[2]);
	}



	for (i = 0; i < numQueries; i++){
		if (-1==fscanf(queries,"%d;%d;%d;%d\n",  &queriesDat[0][i],&queriesDat[1][i],
				&queriesDat[2][i],&queriesDat[3][i])){
			fprintf(stderr,"error al leer queries\n");exit(1);
		}
	}

	char nomFich[256];

	int totalAristasCalc=0;
	for (i=1;i<=numPredicados;i++){
		totalAristasCalc+=k2triples->trees[i]->numberOfEdges;
	}
	fprintf(stderr,"numero de aristas:%d\n",totalAristasCalc);
	fprintf(stderr,"TAM SUBM:%d\n",k2triples->trees[1]->tamSubm);
	int results=0;
	int errores = 0;

	gettimeofday(&tv1, NULL);
	startClock();
	for (i = 0; i < numQueries; i++) {
	  for (k=0;k<numIteraciones;k++){
		switch (tipoQuery) {
		case 1:
			results = spo(k2triples, queriesDat[0][i] - 1,
					queriesDat[1][i]-1, queriesDat[2][i] - 1);
			break;
		case 2:
			results = sp(k2triples, queriesDat[0][i] - 1,
					queriesDat[1][i]-1);
			break;
		case 3:
			results = po(k2triples, queriesDat[1][i],
					queriesDat[2][i] - 1);
			break;
		case 41:
			results = so(k2triples, queriesDat[0][i] - 1,
					queriesDat[2][i] - 1);
			break;
		case 42:
			results = soIndex(k2triples, queriesDat[0][i] - 1,
					queriesDat[2][i] - 1);
			break;
		case 43:
			results =soDobleIndex(k2triples,queriesDat[0][i]-1,queriesDat[2][i]-1);
			break;
		case 51:
			results = s(k2triples, queriesDat[0][i] - 1);
			break;
		case 52:
			results = sIndex(k2triples, queriesDat[0][i] - 1);
			break;
		case 6:
			results = pOrdenado(k2triples, queriesDat[1][i]-1);
			break;
		case 71:
			results = o(k2triples, queriesDat[2][i] - 1);
			break;
		case 72:
			results = oIndex(k2triples, queriesDat[2][i] - 1);
			break;
		default:
			fprintf(stderr, "error\n");
		}

		if (DEBUG2) {
			if (results != queriesDat[3][i]) {
				errores++;
				fprintf(stderr, "(%d)resultados:%d-reales:%d\n",i, results,
						queriesDat[3][i]);
			}
		}
	}
	}
	
	gettimeofday(&tv2, NULL);
	stopClock();
	printTime();
	fprintf(
					stderr,
					"Tiempo total: %ld en us\n",
					(tv2.tv_sec - tv1.tv_sec) * 1000000
							+ (tv2.tv_usec - tv1.tv_usec));

	if (DEBUG2) {
		if (!errores)
			fprintf(stderr, "TEST:OK(%d)\n", numQueries);
		else
			fprintf(stderr, "TEST:FAIL:%d/%d\n", errores, numQueries);
	}
	return 0;
}

