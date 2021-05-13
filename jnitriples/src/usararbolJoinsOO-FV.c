#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#include "joinsFV.h"

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

		/*  Join 4: ?Y P2 ?x >< S1 P1 ?x */
		case 41:
			results = _ppoDerecha(treps, queriesDat[1][i], queriesDat[4][i],
					queriesDat[3][i] - 1,POROBJETO,POROBJETO);
			break;
		case 42:
			results = _ppoInteractiva(treps, queriesDat[1][i], queriesDat[4][i],
					queriesDat[3][i] - 1,POROBJETO,POROBJETO);
			break;
		case 43:
			results = _ppoIndependiente(treps, queriesDat[1][i],
					queriesDat[4][i], queriesDat[3][i] - 1,POROBJETO,POROBJETO);
			break;
		/*  Join 52: ?Y ?P2 ?x >< ?x P1 O1 */
		case 501:
			results = __poDerecha(treps, numPredicados, *indiceSP,*indiceOP,
								queriesDat[0][i]-1, queriesDat[1][i],POROBJETO,POROBJETO);
			break;
		case 502:
			results = __poInteractiva(treps, numPredicados, *indiceSP,*indiceOP,
								queriesDat[0][i]-1, queriesDat[1][i],POROBJETO,POROBJETO);
			break;


			/*  Join 5: ?Y P2 ?x >< S1 ?P1 ?x */
		case 511:
			results = _p_oDerecha(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 1,POROBJETO,POROBJETO);
			break;
		case 512:
			results = _p_oDerecha(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 0,POROBJETO,POROBJETO);
			break;
		case 521:
			results = _p_oIzquierda(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 1, POROBJETO,POROBJETO);
			break;
		case 522:
			results = _p_oIzquierda(treps, numPredicados,*indiceSP, *indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 0,POROBJETO,POROBJETO);
			break;
		case 531:
			results = _p_oInteractiva(treps, numPredicados, *indiceSP,*indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 1,POROBJETO,POROBJETO);
			break;
		case 532:
			results = _p_oInteractiva(treps, numPredicados, *indiceSP,*indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 0, POROBJETO,POROBJETO);
			break;
		case 541:
			results = _p_oIndependiente(treps, numPredicados, *indiceSP,*indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 1,POROBJETO,POROBJETO);
			break;
		case 542:
			results = _p_oIndependiente(treps, numPredicados, *indiceSP,*indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 0,POROBJETO,POROBJETO);
			break;
		case 551:
			results = _p_oMergeIndependiente(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 1, POROBJETO, POROBJETO);
			break;
		case 552:
			results = _p_oMergeIndependiente(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[1][i], queriesDat[3][i] - 1, 0, POROBJETO, POROBJETO);
			break;

		/* Join 6: ?Y ?P2 ?x >< ?x ?P1 O1 */
		case 611:
			results = ___oDerecha(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[3][i] - 1, 1,POROBJETO,POROBJETO);
			break;
		case 612:
			results = ___oDerecha(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[3][i] - 1, 0,POROBJETO,POROBJETO);
			break;

		case 621:
			results = ___oInteractiva(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[3][i] - 1, 1,POROBJETO,POROBJETO);
			break;
		case 622:
			results = ___oInteractiva(treps, numPredicados, *indiceSP, *indiceOP,
					queriesDat[3][i] - 1, 0,POROBJETO,POROBJETO);
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
			stopClock();
			printTime();
	
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

