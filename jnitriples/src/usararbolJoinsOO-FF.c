#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#include "joinsFF.h"
#include <sys/time.h>
#include <time.h>
#include "directcodesI.h"
#include "commons.h"

#define SEC_TIME_DIVIDER  ((double) 1.0    )     //1 sec = 1 sec
#define SEC_TIME_UNIT       "sec"

int main(int argc, char* argv[]) {
	int i;
	struct timeval tv1, tv2;
	FILE * queries;
	int * queriesDat[9];
	int tipoQuery;

	if (argc < 9) {
		fprintf(
				stderr,
				"USAGE: %s <GRAPH> <nPred> <queries> <nQueries> <dacCodeS> <dacCodeO> <|SO|> <tipoQuery> \n",
				argv[0]);
		return (-1);
	}
	inicializaMemoria();
	int numSO=atoi(argv[7]);
	tipoQuery = atoi(argv[8]);
	int numPredicados = atoi(argv[2]);
	int numQueries = atoi(argv[4]);
	inicializarEstructuras(numPredicados, numSO);
	INDICEDAC * indiceSP = cargarDAC(argv[5],numPredicados);
	INDICEDAC * indiceOP = cargarDAC(argv[6],numPredicados);
		int numIteraciones=1;
	if (argc==10){
		numIteraciones=atoi(argv[9]);
		fprintf(stderr,"Iteraciones:%d\n",numIteraciones);
	}


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
		/* JOIN 1 */
		case 11:
			results = sppo(treps, numPredicados, queriesDat[0][i] - 1, queriesDat[1][i],
					queriesDat[4][i], queriesDat[3][i] - 1, POROBJETO, POROBJETO);
			break;
		case 12:
			results = sppoParalelo(treps, numPredicados, queriesDat[0][i] - 1, queriesDat[1][i],
					queriesDat[4][i], queriesDat[3][i] - 1, POROBJETO, POROBJETO);
			break;
		case 13:
			results = sppoIzquierda(treps, numPredicados, queriesDat[0][i] - 1, queriesDat[1][i],
					queriesDat[4][i], queriesDat[3][i] - 1, POROBJETO, POROBJETO);
			break;
		case 14:
			results = sppoDerecha(treps, numPredicados, queriesDat[0][i] - 1, queriesDat[1][i],
					queriesDat[4][i], queriesDat[3][i] - 1, POROBJETO, POROBJETO);
			break;

			/* JOIN 2 */
		case 211:
			results = sp_o(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[1][i], queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 221:
			results = sp_oParalelo(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[1][i], queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 231:
			results = sp_oInterseccion(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[0][i] - 1, queriesDat[1][i], queriesDat[3][i] - 1, 1, POROBJETO,
					POROBJETO);
			break;
		case 212:
			results = sp_o(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[1][i], queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
			break;
		case 222:
			results = sp_oParalelo(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[1][i], queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
			break;

		case 232:
			results = sp_oInterseccion(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[0][i] - 1, queriesDat[1][i], queriesDat[3][i] - 1, 0, POROBJETO,
					POROBJETO);
			break;

			/* join 3 */
		case 311:
			results = s__o(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 312:
			results = s__o(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
			break;
		case 321:
			results = s__oParalelo(treps, *indiceSP, *indiceOP, numPredicados, queriesDat[0][i] - 1,
					queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 322:
			results = s__oParalelo(treps, *indiceSP, *indiceOP, numPredicados, queriesDat[0][i] - 1,
					queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
			break;
		case 331:
			results = s__oMerge(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 332:
			results = s__oMerge(treps, numPredicados, *indiceSP, *indiceOP, queriesDat[0][i] - 1,
					queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
			break;
		case 341:
			results = s__oMergeParalelo(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[0][i] - 1, queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 342:
			results = s__oMergeParalelo(treps, numPredicados,*indiceSP, *indiceOP,
					queriesDat[0][i] - 1, queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
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
				fprintf(stdout, "query %d-( %d / %d)\n", i, results, queriesDat[8][i]);
				}
			}
		}
	}

	gettimeofday(&tv2, NULL);

	fprintf(stderr, "Tiempo total: %ld en us\n",
			(tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec));

	stopClock();
	printTime();

	if (DEBUG2){
		if (!errores)
			fprintf(stderr, "TEST:OK(%d)\n", numQueries);
		else
			fprintf(stderr, "TEST:FAIL:%d/%d\n", errores, numQueries);
	}
	return 0;
}

