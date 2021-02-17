/*
 * kTreeJoin.h
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#ifndef KTREEJOIN_H_
#define KTREEJOIN_H_
#include "kTree.h"
#include "interfazSO.h"

//JOIN 1
uint columnasvsfilas2(TREP ** trepFilas, int x, int y, int indiceRel, int xrelatIn, int yrelatIn,
		int numPredicados, int xtot, int ytot, int pred1, int pred2, SET * pares, int parteDer,
		int parteIzq);

//JOIN 2

uint columnasvsfilas3(TREP ** trepFilas, int x, int y, int indiceRel, int xrelatIn, int yrelatIn,
		int numPredicados, int xtot, int ytot, int pred1, int condac, SET * pares, int parteDer,
		int parteIzq);

//JOIN 3
uint columnasvsfilas(TREP ** trepFilas, INDICEDAC sp, INDICEDAC op, int x, int y, int indiceRel,
		int xrelatIn, int yrelatIn, int numPredicados, int xtot, int ytot, int condac, SET * pares,
		int parteDer, int parteIzq);
//JOIN 4
uint columnaCompletoCruzado(TREP * trepFila, MREP * repFila, int x, TREP * trepCompleto,
		int indiceRel, SET * pares, int parte1, int parte2);

//JOIN 5
uint  columnasvsMatriz(TREP ** trepFilas, int x, TREP * trepCompleto, int indiceRel, int xrelatIn,
		int numPredicados, int xtot,INDICEDAC sp, INDICEDAC op, int condac, SET * pares, int parte1, int parte2);

//JOIN 6
uint  columnasvsMatrices(TREP ** trepFilas, int x, int indiceRel, int xrelatIn, int numPredicados,
		int xtot, INDICEDAC sp, INDICEDAC op, int condac, SET * pares, int parte1, int parte2);

//JOIN 7
uint  matrizvsMatriz(TREP * trep1, TREP * trep2, SET * pares, int parte1, int parte2);

//JOIN 8
uint  matrizvsMatrices(TREP * trep1, int pred1, TREP ** treps, int numPredicados, int indiceRel,
		SET * pares, int parte1, int parte2);

#endif /* KTREEJOIN_H_ */
