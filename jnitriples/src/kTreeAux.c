/*
 * kTreeAux.c
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#include "kTreeAux.h"

//Cambiado hasAnytBitSetÑantes unsihned char
int hasAnyBitSet(char * b) {
	int i = 0, bvalue = 0;
	if (b != NULL)
		for (i = 0; i < K2_3_char; i++)
			bvalue = bvalue || (b[i] != 0);
	return bvalue;
}

QUEUE * AddItem(QUEUE * listpointer, NODE * elem, int cantx, int canty) {
	if (listpointer != NULL) {
		QUEUE * lp = (QUEUE *) mymalloc(sizeof(struct QUEUE));
		finalQUEUE -> link = lp;
		lp -> link = NULL;
		lp -> element = elem;
		lp -> basex = cantx;
		lp -> basey = canty;
		finalQUEUE = lp;
		return listpointer;
	} else {
		listpointer = (QUEUE *) mymalloc(sizeof(struct QUEUE));
		listpointer -> link = NULL;
		listpointer -> element = elem;
		listpointer -> basex = cantx;
		listpointer -> basey = canty;
		finalQUEUE = listpointer;
		return listpointer;
	}
}

void AddItem2(TREP *trep, int elem, int cantx, int canty) {

	if (trep->finq - trep->iniq + 1 >= MAX_INFO ) {
			// fprintf(stderr,"(%d)\n",trep->finq-trep->iniq);
			fprintf(stderr, "overflow!\n");
			exit(1);
	}

	int finq;

	if (trep->iniq != -1) {
		trep->finq++;
		finq = getPQ(trep->finq);
		element[finq] = elem;
		basex[finq] = cantx;
		basey[finq] = canty;
	} else {
		trep->iniq = 0;
		trep->finq = 0;
		element[trep->iniq] = elem;
		basex[trep->iniq] = cantx;
		basey[trep->iniq] = canty;
	}

}

QUEUE * RemoveItem(QUEUE * listpointer) {
	QUEUE * tempp;
	tempp = listpointer -> link;
	free(listpointer);
	return tempp;
}

void ClearQueue(QUEUE * listpointer) {
	while (listpointer != NULL) {
		listpointer = (QUEUE *) RemoveItem(listpointer);
	}
}

void AddItemJoin(TREP *trep, int elem, int cantx, int canty, int indice) {
	int finq;

	if (trep->finq - trep->iniq + 1 >= MAX_INFO ) {
			// fprintf(stderr,"(%d)\n",trep->finq-trep->iniq);

			fprintf(stderr, "overflow!\n");
			exit(1);
	}

	if (trep->iniq != -1) {
		trep->finq++;
		finq = getPQ(trep->finq);
		element[finq] = elem;
		basex[finq] = cantx;
		basey[finq] = canty;
		indicesRep[finq] = indice;
	} else {
		trep->iniq = 0;
		trep->finq = 0;
		element[trep->iniq] = elem;
		basex[trep->iniq] = cantx;
		basey[trep->iniq] = canty;
		indicesRep[trep->iniq] = indice;
	}






}

void AddItemJoin2(TREP *trep, int elem, int cantx, int canty, int indice,
		int esFila) {
	int finq;
	if (trep->finq - trep->iniq + 1 >= MAX_INFO ) {
			// fprintf(stderr,"(%d)\n",trep->finq-trep->iniq);

			fprintf(stderr, "overflow!\n");
			exit(1);
	}
	if (trep->iniq != -1) {
		trep->finq++;
		finq = getPQ(trep->finq);
		element[finq] = elem;
		basex[finq] = cantx;
		basey[finq] = canty;
		indicesRep[finq] = indice;
		esFilas[finq] = esFila;
	} else {
		trep->iniq = 0;
		trep->finq = 0;
		element[trep->iniq] = elem;
		basex[trep->iniq] = cantx;
		basey[trep->iniq] = canty;
		indicesRep[trep->iniq] = indice;
		esFilas[trep->iniq] = esFila;
	}
}

void RemoveItem2(TREP * trep) {

	trep->iniq++;
}

QUEUECONS * AddItemCONS(QUEUECONS * listpointer, NODE * elem) {

	if (listpointer != NULL) {
		QUEUECONS * lp = (QUEUECONS *) mymalloc(sizeof(struct QUEUECONS));
		finalQUEUECONS -> link = lp;
		lp -> link = NULL;
		lp -> element = elem;
		finalQUEUECONS = lp;
		return listpointer;
	} else {
		listpointer = (QUEUECONS *) mymalloc(sizeof(struct QUEUECONS));
		listpointer -> link = NULL;
		listpointer -> element = elem;
		finalQUEUECONS = listpointer;
		return listpointer;
	}
}

QUEUECONS * RemoveItemCONS(QUEUECONS * listpointer) {
	QUEUECONS * tempp;
	//    printf ("Element removed is %d\n", listpointer -> dataitem);
	tempp = listpointer -> link;
	free(listpointer);
	return tempp;
}

uint exp_pow(uint base, uint pow) {
	uint i, result = 1;
	for (i = 0; i < pow; i++)
		result *= base;
	return result;
}

void RemoveItem3(TREP * trep) {
	trep->iniq++;
}

/*------------------------------------------------------------------
 Function used by qsort to compare two elements a and b
 ------------------------------------------------------------------*/
int comparaFrecListaDesc(const void *a, const void *b) {
	unsigned int *left, *right;
	left = (unsigned int *) a;
	right = (unsigned int *) b;
	if (hash[*left].weight < hash[*right].weight)
		return 1;
	if (hash[*left].weight > hash[*right].weight)
		return -1;

	return 0;
}


int checkLimites(TREP * trep) {

	if (trep->iniq > MAX_INFO){
		// fprintf(stderr,"modulando\n");
		trep->iniq = trep->iniq - MAX_INFO;
		trep->finq = trep->finq - MAX_INFO;
	}
}
