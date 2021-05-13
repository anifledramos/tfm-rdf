/*
 * kTreeJoin.c
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#include "kTreeJoinInt.h"

int joinAsim1(TREP ** trepFilas, int x, int y, int pred1, int pred2, int numPredicados, SET * pares,
		int parteDer, int parteIzq) {

	infoTOTAL[0][0] = 0;
	uint  i;
	uint xrelatIn = x / trepFilas[1]->tamSubm;
	uint yrelatIn = y / trepFilas[1]->tamSubm;
	int xtot = x;
	int ytot = y;
	x = x % trepFilas[1]->tamSubm;
	y = y % trepFilas[1]->tamSubm;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	int resultados=0;
	for (i = 0; i < trepFilas[1]->part; i++) {
		resultados=columnasvsfilas2(trepFilas, x, y, i, xrelatIn, yrelatIn, numPredicados, xtot,
				ytot, pred1, pred2, pares, parteDer, parteIzq);
	}

	return resultados;
}

int joinAsim2(TREP ** trepFilas, INDICEDAC sp, INDICEDAC op, int x, int y, int pred1,
		int numPredicados, int condac, SET * pares, int parteDer, int parteIzq) {

	infoTOTAL[0][0] = 0;
	uint  i;
	uint xrelatIn = x / trepFilas[1]->tamSubm;
	uint yrelatIn = y / trepFilas[1]->tamSubm;
	int xtot = x;
	int ytot = y;
	x = x % trepFilas[1]->tamSubm;
	y = y % trepFilas[1]->tamSubm;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	int resultados=0;
	if (condac){
		commonObtenerPredicados(sp, op, xtot, parteDer);
	}

	for (i = 0; i < trepFilas[1]->part; i++) {
		resultados= columnasvsfilas3(trepFilas, x, y, i, xrelatIn, yrelatIn,
			numPredicados, xtot, ytot, pred1,condac,pares, parteDer,parteIzq);
	}

	return resultados;
}

int joinAsim3(TREP ** trepFilas, INDICEDAC sp, INDICEDAC op, int x, int y, int numPredicados,
		int condac, SET * pares, int parteDer, int parteIzq) {

	pares[0].x = 0;
	uint  i;
	uint xrelatIn = x / trepFilas[1]->tamSubm;
	uint yrelatIn = y / trepFilas[1]->tamSubm;
	int xtot = x;
	int ytot = y;
	x = x % trepFilas[1]->tamSubm;
	y = y % trepFilas[1]->tamSubm;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	if (condac) {
		commonObtenerPredicados(sp,op,xtot,parteDer);
		commonObtenerPredicados2(sp,op,ytot,parteIzq);
	}

	for (i = 0; i < trepFilas[1]->part; i++) {
		resultados= columnasvsfilas(trepFilas, sp, op, x, y, i, xrelatIn, yrelatIn, numPredicados,
				xtot, ytot, condac, pares, parteDer,parteIzq);
	}

	return resultados;
}

int joinAsim4(TREP * trepFila, int x, TREP * trepCompleto, SET * pares, int parte1, int parte2) {
	infoTOTAL[0][0] = 0;
	MREP * repFila;
	uint  i;
	uint xrelatIn = x / trepFila->tamSubm;
	x = x % trepFila->tamSubm;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	for (i = 0; i < trepFila->part; i++) {
		trepFila->columna = i;
		trepFila->fila = i;

		repFila = obtenerRep(trepFila,xrelatIn,i,parte2);

		resultados =columnaCompletoCruzado(trepFila, repFila, x, trepCompleto, i, pares,parte1,parte2);
	}
	return resultados;
}

int joinAsim5(TREP ** trepFilas, int x, TREP * trepCompleto, int numPredicados, int pred,
		INDICEDAC sp, INDICEDAC op, int condac, SET * pares, int parte1, int parte2) {

	pares[0].x = 0;
	uint  i;
	uint xrelatIn = x / trepFilas[1]->tamSubm;
	int xtot = x;
	x = x % trepFilas[1]->tamSubm;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	for (i = 0; i < trepFilas[1]->part; i++) {

		resultados= columnasvsMatriz(trepFilas, x, trepCompleto, i, xrelatIn, numPredicados, /*pred,*/
		xtot,sp, op, condac, pares,parte1,parte2);
	}
	return resultados;
}

int joinAsim52(TREP ** trepFilas, int x,int p1, int numPredicados, INDICEDAC sp, INDICEDAC op, int condac,
		SET * pares, int parte1, int parte2) {

	pares[0].x = 0;
	uint  i;
	uint xrelatIn = x / trepFilas[1]->tamSubm;
	int xtot = x;
	x = x % trepFilas[1]->tamSubm;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	for (i = 0; i < trepFilas[1]->part; i++) {
		resultados= columnavsMatrices(trepFilas, x, p1,i, xrelatIn, numPredicados, xtot, sp, op, condac,
				pares, parte1, parte2);
	}
	return resultados;
}

int joinAsim6(TREP ** trepFilas, int x, int numPredicados, INDICEDAC sp, INDICEDAC op, int condac,
		SET * pares, int parte1, int parte2) {

	pares[0].x = 0;
	uint  i;
	uint xrelatIn = x / trepFilas[1]->tamSubm;
	int xtot = x;
	x = x % trepFilas[1]->tamSubm;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	for (i = 0; i < trepFilas[1]->part; i++) {
		resultados= columnasvsMatrices(trepFilas, x, i, xrelatIn, numPredicados, xtot, sp, op, condac,
				pares, parte1, parte2);
	}
	return resultados;
}

int joinAsim7(TREP * trep1, TREP * trep2, SET * pares, int parte1, int parte2) {
	pares[0].x = 0;
	uint  i;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	for (i = 0; i < trep1->part; i++) {
	//	for (i = 1; i <= 1; i++) {
		trep1->fila = i;
		trep2->columna = i;
		//OJO: invertido el orden respecto a los parametros recibidos
		resultados = matrizvsMatriz(trep2, trep1, pares, parte1, parte2);
	}
	return resultados;
}

int joinAsim8(TREP * trep1, int pred1, TREP ** treps, int numPredicados, SET * pares, int parte1, int parte2) {
	pares[0].x = 0;
	uint  i;
	int resultados=0;
	//Por cada submatriz de la fila tenemos que introducir part submatrices de la completa
	for (i = 0; i < trep1->part; i++) {
		resultados=matrizvsMatrices(trep1, pred1, treps, numPredicados, i, pares, parte1, parte2);
	}
	return resultados;
}
