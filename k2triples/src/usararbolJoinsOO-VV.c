#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#include "joinsVV.h"

#include <sys/time.h>
#include <time.h>
#include "directcodesI.h"
#include "commons.h"

#define SEC_TIME_DIVIDER  ((double) 1.0    )     //1 sec = 1 sec
#define SEC_TIME_UNIT       "sec"
#define TAMVECTORES 2048*1000*10

int main(int argc, char* argv[]) {
	int i;
	struct timeval tv1, tv2;
	FILE * queries;
	int * queriesDat[9];
	int tipoQuery;


	if (argc < 9) {
		fprintf(
				stderr,
				"USAGE: %s <GRAPH> <nPred> <queries> <nQueries> <dacCodeS> <dacCodeO> <|SO|> <tipoQuery> [iter]\n",
				argv[0]);
		return (-1);
	}
	inicializaMemoria();


	int numSO=atoi(argv[7]);
	tipoQuery = atoi(argv[8]);
	int numPredicados = atoi(argv[2]);
	int numQueries = atoi(argv[4]);
	int numIteraciones=1;
	if (argc==10){
		numIteraciones=atoi(argv[9]);
		fprintf(stderr,"Iteraciones:%d\n",numIteraciones);
	}

	INDICEDAC * indiceSP = cargarDAC(argv[5],numPredicados);
	INDICEDAC * indiceOP = cargarDAC(argv[6],numPredicados);
	inicializarEstructuras(numPredicados, numSO);


	for (i = 0; i < 9; i++) {
		queriesDat[i] = (int *) malloc(sizeof(int) * numQueries);
	}

	if ((queries = fopen(argv[3], "r")) == NULL) {
		fprintf(stderr, "error: no se pudo abrir el fichero %s\n", argv[3]);
		return 1;
	} else {
		fprintf(stderr, "%s\n", argv[3]);
	}



	for (i = 0; i < numQueries; i++)
		if (-1==fscanf(queries,"%d;%d;%d;%d;%d;%d;%d;%d;%d\n",  &queriesDat[0][i],&queriesDat[1][i],
				&queriesDat[2][i],&queriesDat[3][i],&queriesDat[4][i],&queriesDat[5][i],&queriesDat[6][i],&queriesDat[7][i],&queriesDat[8][i])){
			fprintf(stderr,"error al leer queries\n");exit(1);
		}


	TREP ** treps = (TREP **) mymalloc(sizeof(TREP *) * (numPredicados+1));

	char nomFich[256];

	for (i = 1; i <= numPredicados; i++) {
		sprintf(nomFich, "%s%d.txt", argv[1], i);
		treps[i] = loadTreeRepresentation(nomFich);
	}
	int results=0;
	int errores = 0;

	gettimeofday(&tv1, NULL);
	startClock();
		int m;
		for (m=0;m<numIteraciones;m++)



	for (i = 0; i < numQueries; i++) {
		switch (tipoQuery) {

			/*  Join 7: ?Y P2 ?x >< ?Z P1 ?x */
			case 71:
				results = _pp_Izquierda(treps, queriesDat[1][i], queriesDat[4][i],POROBJETO,POROBJETO);
				break;

			case 72:
				results = _pp_Derecha(treps, queriesDat[1][i], queriesDat[4][i],POROBJETO,POROBJETO);
				break;

			case 73:
				results = _pp_Interactiva(treps, queriesDat[1][i],
						queriesDat[4][i], POROBJETO,POROBJETO);
				break;

			case 74:
				results = _pp_Independiente(treps, queriesDat[1][i],
						queriesDat[4][i],POROBJETO,POROBJETO);
				break;


			/*  Join 8: ?Y P2 ?x >< ?x ?P ?Z */
			case 811:
				results = _p__Izquierda(treps, queriesDat[1][i], numPredicados,*indiceSP,*indiceOP,1,POROBJETO,POROBJETO);
				break;
			case 812:
				results = _p__Izquierda(treps, queriesDat[1][i], numPredicados,*indiceSP,*indiceOP,0,POROBJETO,POROBJETO);
				break;
			case 82:
				results = _p__Interactiva(treps, queriesDat[1][i],numPredicados, POROBJETO,POROBJETO);
				break;

			default:
				fprintf(stderr, "error\n");
		}

		if (DEBUG2){
			if (tipoQuery == 1) {
				if (results != 1)
					errores++;
			} else {
				if (results != queriesDat[8][i]) {
					errores++;
					fprintf(stdout, "query %d-( %d / %d)\n", i, results,
							queriesDat[8][i]);
				}
			}

		}
	}

	gettimeofday(&tv2, NULL);

	fprintf(stderr, "Tiempo total: %ld en us\n",
			(tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec));

	if (DEBUG2){
		if (!errores)
			fprintf(stderr, "TEST:OK(%d)\n", numQueries);
		else
			fprintf(stderr, "TEST:FAIL:%d/%d\n", errores, numQueries);
	}
	return 0;
}

