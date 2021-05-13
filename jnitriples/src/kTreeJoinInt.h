/*
 * kTreeJoin.h
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#ifndef KTREEJOININT_H_
#define KTREEJOININT_H_

#include "kTree.h"
#include "kTreeJoin.h"

// EVALUACIÓN INTERACTIVA PARA LOS JOINS: INTERFAZ

int joinAsim1(TREP ** trepFilas, int x, int y, int pred1, int pred2, int numPredicados, SET * pares,
		int parteDer, int parteIzq);

int joinAsim2(TREP ** trepFilas, INDICEDAC sp, INDICEDAC op, int x, int y, int pred1,
		int numPredicados, int condac, SET * pares, int parteDer, int parteIzq);

int joinAsim3(TREP ** trepFilas, INDICEDAC sp, INDICEDAC op, int x, int y, int numPredicados,
		int condac, SET * pares, int parteDer, int parteIzq);

int joinAsim4(TREP * trepFila, int x, TREP * trepCompleto, SET * pares, int parte1, int parte2);

int joinAsim5(TREP ** trepFilas, int x, TREP * trepCompleto, int numPredicados, int pred,
		INDICEDAC sp, INDICEDAC op, int condac, SET * pares, int parte1, int parte2);

int joinAsim52(TREP ** trepFilas, int x,int p, int numPredicados, INDICEDAC sp, INDICEDAC op, int condac,
		SET * pares, int parte1, int parte2);

int joinAsim6(TREP ** trepFilas, int x, int numPredicados, INDICEDAC sp, INDICEDAC op, int condac,
		SET * pares, int parte1, int parte2);

int joinAsim7(TREP * trep1, TREP * trep2, SET * pares, int parte1, int parte2);

int joinAsim8(TREP * trep1, int pred1, TREP ** treps, int numPredicados, SET * pares, int parte1, int parte2);

#endif /* KTREEJOIN_H_ */
