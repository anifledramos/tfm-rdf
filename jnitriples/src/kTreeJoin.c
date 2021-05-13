/*
 * kTreeJoin.c
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#include "kTreeJoin.h"

uint columnasvsfilas2(TREP ** trepFilas, int x, int y, int indiceRel,
		int xrelatIn, int yrelatIn, int numPredicados, int xtot, int ytot,
		int pred1, int pred2, SET * pares, int parteDer, int parteIzq) {

	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	int indexRel = 0;
	TREP * trepIndex = trepFilas[1];

	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level;

	MREP * repFila1 = obtenerRep(trepFilas[pred1], xrelatIn, indiceRel,
			parteDer);

	if (trepFilas[pred1]->numberOfEdges > 0 && repFila1 != NULL
			&& repFila1->numberOfEdges) {
		CommonInitItemJoin2(trepIndex, 0, x, 0, pred1, 1, parteDer);
	}

	if (trepIndex->iniq == -1)
		return infoTOTAL[0][0];

	MREP * repFila2 = obtenerRep(trepFilas[pred2], yrelatIn, indiceRel,
			parteIzq);
	if (trepFilas[pred2]->numberOfEdges > 0 && repFila2 != NULL
			&& repFila2->numberOfEdges) {
		CommonInitItemJoin2(trepIndex, 0, y, 0, pred2, 0, parteIzq);
	} else {
		return infoTOTAL[0][0];
	}

	queuecont = 1;

	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola

	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;

				//COLUMNAS

				while (esFilas[getPQ(indexRel)] && !(indexRel > trepIndex->finq)) {

					node = getNode(K1, j, div_level, getPQ(indexRel), parteDer);
					if (isBitSet(repFila1->bt, node)) {
						//Si en la fila el valor es positivo
						if (i != trepIndex->maxLevel1 - 1) {
							commonAddItem2(trepIndex,
									rank(repFila1->bt, node) * K1K1, j,
									div_level, indexRel, 1, parteDer);

						} else {
							commonAddItem2(
									trepIndex,
									repFila1->cutBt
											+ (rank(repFila1->bt, node) * K1K1
													- repFila1->cutBt)
													/ factorAdjust, j,
									div_level, indexRel, 1, parteDer);
						}
						insertados++;
					}
					indexRel++;
				}
				int success = 0;

				if (insertados) {
					// Mientras no nos encontremos el siguiente elemento de la fila,
					// implica que todos los demás son de ese elemento
					//Mientras vengan elementos procedentes de la segunda parte del join
					//FILAS

					while (!(indexRel > trepIndex->finq)
							&& !(esFilas[getPQ(indexRel)])) {

						node = getNode(K1, j, div_level, getPQ(indexRel),
								parteIzq);

						if (isBitSet(repFila2->bt, node)) {
							//Si en la fila el valor es positivo
							//conttmp++;
							if (i != trepIndex->maxLevel1 - 1) {
								commonAddItem2(trepIndex,
										rank(repFila2->bt, node) * K1K1, j,
										div_level, indexRel, 0, parteIzq);
							} else {
								commonAddItem2(
										trepIndex,
										repFila2->cutBt
												+ (rank(repFila2->bt, node)
														* K1K1 - repFila2->cutBt)
														/ factorAdjust, j,
										div_level, indexRel, 0, parteIzq);
							}
							success++;
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}

				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	uint cutPreRank1 = rank(repFila1->bt, repFila1->lastBt1_len - 1);
	uint cutPreRank2 = rank(repFila2->bt, repFila2->lastBt1_len - 1);
	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)] && !(indexRel > trepIndex->finq)) {
					node = getNode(K2, j, div_level, getPQ(indexRel), parteDer);

					if (isBitSet(repFila1->bt, node)) {
						commonAddItem2(
								trepIndex,
								repFila1->cutBt
										+ (rank(repFila1->bt, node - 1)
												- cutPreRank1) * K2K2, j,
								div_level, indexRel, 1, parteDer);
						insertados++;
					}
					indexRel++;
				}

				int success = 0;
				if (insertados) {
					node = getNode(K2, j, div_level, getPQ(indexRel), parteIzq);
					if (isBitSet(repFila2->bt, node)) {
						commonAddItem2(
								trepIndex,
								repFila2->cutBt
										+ (rank(repFila2->bt, node - 1)
												- cutPreRank2) * K2K2, j,
								div_level, indexRel, 0, parteIzq);
						success++;
					}
					indexRel++;
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}
				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index;
	while (trepIndex->iniq <= trepIndex->finq) {

		int newIndexRel = trepIndex->iniq;

		while (esFilas[getPQ(newIndexRel)] && !(indexRel > trepIndex->finq))
			newIndexRel++;
		for (j = 0; j < K2; j++) {

			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;
			//Comprobacion de las filas
			while (esFilas[getPQ(indexRel)] && !(indexRel > trepIndex->finq)) {
				nleafrelat = getNode(K2, j, K2_2, getPQ(indexRel), parteDer)
						- repFila1->bt_len;
				//Es positivo para la fila
				if (isBitSet(repFila1->bn, nleafrelat)) {
					posInf = rank(repFila1->bn, nleafrelat);
					realvalue = accessFT(repFila1->compressIL, posInf);
					realFilas[insertados] = realvalue;
					for (i = 0; i < K2_2; i++) {

						if (bitgetchar(&(trepFilas[indicesRep[getPQ(indexRel)]]->words[realvalue*trepFilas[indicesRep[getPQ(indexRel)]]->lenWords]),getPosRel(i,x,K2_2,parteDer))) {

							int indexRelF = newIndexRel;
							while (!esFilas[getPQ(indexRelF)]
									&& !(indexRelF > trepIndex->finq)) {
								nleafrelat = getNode(K2, j, K2_2,
										getPQ(indexRelF), parteIzq)
										- repFila2->bt_len;
								if (isBitSet(repFila2->bn, nleafrelat)) {
									posInf = rank(repFila2->bn, nleafrelat);
									realvalueAux = accessFT(
											repFila2->compressIL, posInf);
									validos[index] = indexRel;
									validos2[index] = i;
									realValues[index++] = realvalueAux;

									if (bitgetchar(&(trepFilas[indicesRep[getPQ(indexRelF)]]->words[realvalueAux*trepFilas[indicesRep[getPQ(indexRelF)]]->lenWords]),getPosRel(i,y,K2_2,parteIzq))) {
										infoTOTAL[0][0]++;
										if (infoTOTAL[0][0]
												== NUM_ELEMENTOS * 2) {
											fprintf(stderr, "numelementos_overflow!\n");
											exit(1);
										}
										infoTOTAL[0][infoTOTAL[0][0]] =
												getPosAbs(K2_2, i, j,
														getPQ(indexRel),
														indiceRel,
														trepIndex->tamSubm,
														parteDer);
									}
								}
								indexRelF++;
							}
						}
					}
				}
				indexRel++;

			}
		}
		while (!(indexRel > trepIndex->finq) && !esFilas[getPQ(indexRel)])
			indexRel++;
		trepIndex->iniq = indexRel;
	}
	return infoTOTAL[0][0];
}

uint columnasvsfilas3(TREP ** trepFilas, int x, int y, int indiceRel,
		int xrelatIn, int yrelatIn, int numPredicados, int xtot, int ytot,
		int pred1, int condac, SET * pares, int parteDer, int parteIzq) {
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	int indexRel = 0;
	int indexRelF;
	TREP * trepIndex = trepFilas[1];

	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level;

	MREP * repFila2;

	MREP * repFila1 = obtenerRep(trepFilas[pred1], yrelatIn, indiceRel,
			parteIzq);

	//PARTE IZQUIERDA
	if (trepFilas[pred1]->numberOfEdges > 0 && repFila1 != NULL
			&& repFila1->numberOfEdges) {
		CommonInitItemJoin2(trepIndex, 0, y, 0, pred1, 1, parteIzq);
	}

	if (trepIndex->iniq == -1)
		return infoTOTAL[0][0];
	int vacio = 1;

	//PARTE DERECHA
	if (!condac) {

		for (i = 1; i <= numPredicados; i++) {
			repFila2 = obtenerRep(trepFilas[i], xrelatIn, indiceRel, parteDer);
			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			if (trepFilas[i]->numberOfEdges > 0 && repFila2 != NULL
					&& repFila2->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, i, 0, parteDer);
				vacio = 0;
			}
		}
	} else {
		for (i = 0; i < resDAC.numeroPredicados; i++) {
			MREP * repFila2 = obtenerRep(trepFilas[resDAC.predicados[i]],
					xrelatIn, indiceRel, parteDer);
			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			if (trepFilas[resDAC.predicados[i]]->numberOfEdges > 0
					&& repFila2 != NULL && repFila2->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, resDAC.predicados[i], 0,
						parteDer);
				vacio = 0;
			}
		}
	}
	//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar

	if (vacio)
		return infoTOTAL[0][0];

	queuecont = 1;
	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola
	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {

			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;

				//PARTE IZQUIERDA
				while (!(indexRel > trepIndex->finq) && esFilas[getPQ(indexRel)]) {

					node = getNode(K1, j, div_level, getPQ(indexRel), parteIzq);

					// Dentro de la fila correcta:
					// Valen todos log de la fila

					if (isBitSet(repFila1->bt, node)) {
						//Si en la fila el valor es positivo
						//conttmp++;
						if (i != trepIndex->maxLevel1 - 1)
							commonAddItem2(trepIndex,
									rank(repFila1->bt, node) * K1K1, j,
									div_level, indexRel, 1, parteIzq);

						else {
							commonAddItem2(
									trepIndex,
									repFila1->cutBt
											+ (rank(repFila1->bt, node) * K1K1
													- repFila1->cutBt)
													/ factorAdjust, j,
									div_level, indexRel, 1, parteIzq);
						}
						insertados++;
					}

					indexRel++;
				}
				int success = 0;

				if (insertados) {
					// Mientras no nos encontremos el siguiente elemento de la fila,
					// implica que todos los demás son de ese elemento
					//int indexINIQ = trepIndex->iniq + 1;
					//Mientras vengan elementos procedentes de la segunda parte del join
					//FILAS
					while (!(indexRel > trepIndex->finq)
							&& !(esFilas[getPQ(indexRel)])) {

						node = getNode(K1, j, div_level, getPQ(indexRel),
								parteDer);
						repFila2 = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								xrelatIn, indiceRel, parteDer);

						if (isBitSet(repFila2->bt, node)) {
							//Si en la fila el valor es positivo
							//conttmp++;
							if (i != trepIndex->maxLevel1 - 1)
								commonAddItem2(trepIndex,
										rank(repFila2->bt, node) * K1K1, j,
										div_level, indexRel, 0, parteDer);

							else {
								commonAddItem2(
										trepIndex,
										repFila2->cutBt
												+ (rank(repFila2->bt, node)
														* K1K1 - repFila2->cutBt)
														/ factorAdjust, j,
										div_level, indexRel, 0, parteDer);
							}
							success++;
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}

				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		//MIRAR ESTE
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	uint cutPreRank1 = rank(repFila1->bt, repFila1->lastBt1_len - 1);

	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;

				while (!(indexRel > trepIndex->finq) && esFilas[getPQ(indexRel)]) {

					node = getNode(K2, j, div_level, getPQ(indexRel), parteIzq);

					if (isBitSet(repFila1->bt, node)) {

						//Invertimos de la misma forma
						commonAddItem2(
								trepIndex,
								repFila1->cutBt
										+ (rank(repFila1->bt, node - 1)
												- cutPreRank1) * K2K2, j,
								div_level, indexRel, 1, parteIzq);
						insertados++;
					}

					indexRel++;
				}

				int success = 0;
				if (insertados) {

					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)]) {
						//Invertimos la indexacion

						node = getNode(K2, j, div_level, getPQ(indexRel),
								parteDer);
						repFila2 = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								xrelatIn, indiceRel, parteDer);
						uint cutPreRank2 = rank(repFila2->bt,
								repFila2->lastBt1_len - 1);
						if (isBitSet(repFila2->bt, node)) {
							//conttmp++;
							commonAddItem2(
									trepIndex,
									repFila2->cutBt
											+ (rank(repFila2->bt, node - 1)
													- cutPreRank2) * K2K2, j,
									div_level, indexRel, 0, parteDer);
							//Exito en la de fila, se consulta el resto
							success++;
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}
				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index;

	//TERCERA PARTE
	while (trepIndex->iniq <= trepIndex->finq) {

		int newIndexRel = trepIndex->iniq;

		while (esFilas[getPQ(newIndexRel)])
			newIndexRel++;

		for (j = 0; j < K2; j++) {

			indexRelF = trepIndex->iniq;
			int insertados = 0;
			index = 0;

			//Comprobacion de las filas
			while (!(indexRelF > trepIndex->finq) && esFilas[getPQ(indexRelF)]) {

				nleafrelat = getNode(K2, j, K2_2, getPQ(indexRelF), parteIzq)
						- repFila1->bt_len;

				if (repFila1->bn_len > 0
						&& isBitSet(repFila1->bn, nleafrelat)) {
					posInf = rank(repFila1->bn, nleafrelat);
					realvalueAux = accessFT(repFila1->compressIL, posInf);
					validos[index] = indexRel;
					validos2[index] = i;
					realValues[index++] = realvalueAux;
					//Cambiado el for
					for (i = 0; i < K2_2; i++) {
						if (bitgetchar(&(trepFilas[indicesRep[getPQ(indexRelF)]]->words[realvalueAux*trepFilas[indicesRep[getPQ(indexRelF)]]->lenWords]),getPosRel(i,y,K2_2,parteIzq))) {
							indexRel = newIndexRel;
							while (!esFilas[getPQ(indexRel)]
									&& !(indexRel > trepIndex->finq)) {

								repFila2 = obtenerRep(
										trepFilas[indicesRep[getPQ(indexRel)]],
										xrelatIn, indiceRel, parteDer);
								nleafrelat = getNode(K2, j, K2_2,
										getPQ(indexRel), parteDer)
										- repFila2->bt_len;

								//Es positivo para la fila
								if (isBitSet(repFila2->bn, nleafrelat)) {

									posInf = rank(repFila2->bn, nleafrelat);
									realvalue = accessFT(repFila2->compressIL,
											posInf);
									realFilas[insertados] = realvalue;

									if (bitgetchar(&(trepFilas[indicesRep[getPQ(indexRel)]]->words[realvalue*trepFilas[indicesRep[getPQ(indexRel)]]->lenWords]),getPosRel(i,x,K2_2,parteDer))) {
										infoTOTAL[0][0]++;

										if (infoTOTAL[0][0]
												== NUM_ELEMENTOS * 2) {
											fprintf(stderr, "numelementos_overflow!\n");
											exit(1);
										}
										infoTOTAL[0][infoTOTAL[0][0]] =
												indicesRep[getPQ(indexRel)];
										infoTOTAL[1][infoTOTAL[0][0]] =
												getPosAbs(K2_2, i, j,
														getPQ(indexRel),
														indiceRel,
														trepIndex->tamSubm,
														parteDer);
									}
								}
								indexRel++;
							}
						}
					}
				}
				indexRelF++;

			}
		}
		while (!(indexRelF > trepIndex->finq) && !esFilas[getPQ(indexRelF)])
			indexRelF++;
		trepIndex->iniq = indexRelF;
	}

	return infoTOTAL[0][0];

}

uint columnasvsfilas(TREP ** trepFilas, INDICEDAC sp, INDICEDAC op, int x,
		int y, int indiceRel, int xrelatIn, int yrelatIn, int numPredicados,
		int xtot, int ytot, int condac, SET * pares, int parteDer, int parteIzq) {

	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;

	MREP * repFila;
	int indexRel;

	TREP * trepIndex = trepFilas[1];

	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, xrelat;

	int insertado = 0;
	if (!condac) {
		for (i = 1; i <= numPredicados; i++) {
			MREP * repFila1 = obtenerRep(trepFilas[i], xrelatIn, indiceRel,
					parteDer);
			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			if (trepFilas[i]->numberOfEdges > 0 && repFila1 != NULL
					&& repFila1->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, i, 1, parteDer);
				insertado++;
			}
		}
	} else {
		for (i = 0; i < resDAC.numeroPredicados; i++) {
			MREP * repFila1 = obtenerRep(trepFilas[resDAC.predicados[i]],
					xrelatIn, indiceRel, parteDer);
			if (trepFilas[resDAC.predicados[i]]->numberOfEdges > 0
					&& repFila1 != NULL && repFila1->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, resDAC.predicados[i], 1,
						parteDer);
				insertado++;
			}
		}

	}
	//Del mismo modo, se añaden numpredicados filas a analizar
	if (!insertado)
		return pares[0].x;
	insertado = 0;
	if (!condac) {
		for (i = 1; i <= numPredicados; i++) {
			MREP * repFila2 = obtenerRep(trepFilas[i], yrelatIn, indiceRel,
					parteIzq);

			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			if (trepFilas[i]->numberOfEdges > 0 && repFila2 != NULL
					&& repFila2->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, y, 0, i, 0, parteIzq);
				insertado++;
			}
		}
	} else {
//		obtenerPredicados(sp,ytot);
		for (i = 0; i < resDAC2.numeroPredicados; i++) {
			MREP * repFila2 = obtenerRep(trepFilas[resDAC2.predicados[i]],
					yrelatIn, indiceRel, parteIzq);
			if (trepFilas[resDAC2.predicados[i]]->numberOfEdges > 0
					&& repFila2 != NULL && repFila2->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, y, 0, resDAC2.predicados[i],
						0, parteIzq);
				insertado++;
			}
		}
	}
	if (!insertado)
		return pares[0].x;

	queuecont = 1;

	if (trepIndex->iniq == -1)
		return pares[0].x;

	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola
	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;

				//COLUMNAS
				while (esFilas[getPQ(indexRel)]) {
					node = getNode(K1, j, div_level, getPQ(indexRel), parteDer);

					repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
							xrelatIn, indiceRel, parteDer);

					if (isBitSet(repFila->bt, node)) {
						//Si en la fila el valor es positivo
						//conttmp++;
						if (i != trepIndex->maxLevel1 - 1)
							commonAddItem2(trepIndex,
									rank(repFila->bt, node) * K1K1, j,
									div_level, indexRel, 1, parteDer);

						else {
							commonAddItem2(
									trepIndex,
									repFila->cutBt
											+ (rank(repFila->bt, node) * K1K1
													- repFila->cutBt)
													/ factorAdjust, j,
									div_level, indexRel, 1, parteDer);
						}
						insertados++;
					}

					indexRel++;
				}
				int success = 0;

				if (insertados) {
					//Parte izquierda
					while (!(indexRel > trepIndex->finq)
							&& !(esFilas[getPQ(indexRel)])) {
						xrelat = basey[getPQ(indexRel)];
						// Dentro de la fila correcta:
						// Valen todos los de la fila
						node = getNode(K1, j, div_level, getPQ(indexRel),
								parteIzq);

						repFila = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								yrelatIn, indiceRel, parteIzq);

						if (isBitSet(repFila->bt, node)) {
							//Si en la fila el valor es positivo
							//conttmp++;
							if (i != trepIndex->maxLevel1 - 1)
								commonAddItem2(trepIndex,
										rank(repFila->bt, node) * K1K1, j,
										div_level, indexRel, 0, parteIzq);

							else {
								commonAddItem2(
										trepIndex,
										repFila->cutBt
												+ (rank(repFila->bt, node)
														* K1K1 - repFila->cutBt)
														/ factorAdjust, j,
										div_level, indexRel, 0, parteIzq);
							}
							success++;
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}

				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)]) {
					node = getNode(K2, j, div_level, getPQ(indexRel), parteDer);

					repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
							xrelatIn, indiceRel, parteDer);

					if (isBitSet(repFila->bt, node)) {
						//conttmp++;
						uint cutPreRank = rank(repFila->bt,
								repFila->lastBt1_len - 1);

						commonAddItem2(
								trepIndex,
								repFila->cutBt
										+ (rank(repFila->bt, node - 1)
												- cutPreRank) * K2K2, j,
								div_level, indexRel, 1, parteDer);
						//Exito en la de fila, se consulta el resto
						insertados++;
					}
					indexRel++;
				}

				int success = 0;
				if (insertados) {
					while (!(indexRel > trepIndex->finq) && !esFilas[getPQ(indexRel)]) {
						//Invertimos la indexacion
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								yrelatIn, indiceRel, parteIzq);

						node = getNode(K2, j, div_level, getPQ(indexRel),
								parteIzq);

						if (isBitSet(repJoin->bt, node)) {
							uint cutPreRank = rank(repJoin->bt,
									repJoin->lastBt1_len - 1);
							//Invertimos de la misma forma
							commonAddItem2(
									trepIndex,
									repJoin->cutBt
											+ (rank(repJoin->bt, node - 1)
													- cutPreRank) * K2K2, j,
									div_level, indexRel, 0, parteIzq);
							success++;
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}
				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index;

	while (trepIndex->iniq <= trepIndex->finq) {

		int newIndexRel = trepIndex->iniq;

		while (esFilas[getPQ(newIndexRel)])
			newIndexRel++;

		for (j = 0; j < K2; j++) {

			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;

			//Comprobacion de las filas
			while (esFilas[getPQ(indexRel)]) {

				repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
						xrelatIn, indiceRel, parteDer);

				nleafrelat = getNode(K2, j, K2_2, getPQ(indexRel), parteDer)
						- repFila->bt_len;

				//Es positivo para la fila
				if (isBitSet(repFila->bn, nleafrelat)) {

					posInf = rank(repFila->bn, nleafrelat);
					realvalue = accessFT(repFila->compressIL, posInf);
					realFilas[insertados] = realvalue;
					for (i = 0; i < K2_2; i++) {
						if (bitgetchar(&(trepFilas[indicesRep[getPQ(indexRel)]]->words[realvalue*trepFilas[indicesRep[getPQ(indexRel)]]->lenWords]),getPosRel(i,x,K2_2,parteDer))) {
							int indexRelF = newIndexRel;
							while (!esFilas[getPQ(indexRelF)]
									&& !(indexRelF > trepIndex->finq)) {
								MREP * repJoin = obtenerRep(
										trepFilas[indicesRep[getPQ(indexRelF)]],
										yrelatIn, indiceRel, parteIzq);
								nleafrelat = getNode(K2, j, K2_2,
										getPQ(indexRelF), parteIzq)
										- repJoin->bt_len;

								if (isBitSet(repJoin->bn, nleafrelat)) {
									posInf = rank(repJoin->bn, nleafrelat);
									realvalueAux = accessFT(repJoin->compressIL,
											posInf);
									validos[index] = indexRel;
									validos2[index] = i;
									realValues[index++] = realvalueAux;

									if (bitgetchar(&(trepFilas[indicesRep[getPQ(indexRelF)]]->words[realvalueAux*trepFilas[indicesRep[getPQ(indexRelF)]]->lenWords]),getPosRel(i,y,K2_2,parteIzq))) {

										pares[0].x++;

										if (pares[0].x == NUM_ELEMENTOS) {
											fprintf(stderr, "numelementos_overflow!\n");
											exit(1);
										}
										pares[pares[0].x].x =
												indicesRep[getPQ(indexRelF)];
										pares[pares[0].x].y = getPosAbs(K2_2, i,
												j, getPQ(indexRel), indiceRel,
												trepIndex->tamSubm, parteDer);
										pares[pares[0].x].z =
												indicesRep[getPQ(indexRel)];

									}
								}
								indexRelF++;
							}
						}
					}
				}
				indexRel++;

			}
		}
		while (!(indexRel > trepIndex->finq) && !esFilas[getPQ(indexRel)])
			indexRel++;
		trepIndex->iniq = indexRel;
	}
	return pares[0].x;
}

uint columnaCompletoCruzado(TREP * trepFila, MREP * repFila, int x,
		TREP * trepCompleto, int indiceRel, SET * pares, int parte1, int parte2) {
	//TODO: revisar los trepFila->columna, no deberia haber ninguno, deberian ser todos de fila.
	int node2;
	int AJUSTEK1K2 = 4;
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;

	if (repFila == NULL || repFila->numberOfEdges == 0) {
		return infoTOTAL[0][0];
	}

	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepFila->iniq = -1;
	trepFila->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, xrelat;

	CommonInitItemJoin(trepFila, 0, x, 0, -1, parte2);

	for (i = 0; i < trepCompleto->part; i++) {
		CommonInitItemJoin(trepFila, 0, x, 0, i, parte1);
	}

	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola
	queuecont = 1;
	for (i = 0; i < trepFila->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepFila->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			xrelat = basex[getPQ(trepFila->iniq)];
			for (j = 0; j < K1; j++) {
				// Dentro de la fila correcta:
				// Valen todos los de la fila

				/*node = K1 * j + xrelat / div_level;
				 node += element[trepFila->iniq];*/
				node = getNode(K1, j, div_level, getPQ(trepFila->iniq), parte2);

				if (isBitSet(repFila->bt, node)) {
					//Si en la fila el valor es positivo
					conttmp++;
					if (i != trepFila->maxLevel1 - 1) {
						commonAddItem(trepFila, rank(repFila->bt, node) * K1K1,
								j, div_level, trepFila->iniq, parte2);
					} else {
						commonAddItem(
								trepFila,
								repFila->cutBt
										+ (rank(repFila->bt, node) * K1K1
												- repFila->cutBt) / factorAdjust,
								j, div_level, trepFila->iniq, parte2);
					}

					int success = 0;

					// Mientras no nos encontremos el siguiente elemento de la fila,
					// implica que todos los demás son de ese elemento
					int indexINIQ = trepFila->iniq + 1;
					//Mientras vengan elementos procedentes de la segunda parte del join
					while (indicesRep[getPQ(indexINIQ)] != -1) {
						if (indexINIQ > trepFila->finq)
							exit(1);

						MREP * repJoin = obtenerRep(trepCompleto,
								indicesRep[getPQ(indexINIQ)], indiceRel,
								parte1);

						int l;

						if (repJoin != NULL)
							for (l = 0; l < K1; l++) {
								//La j es en la que estamos en la fila
								//Invertimos j y l

								node2 = getNodeXY(K1, l, j, getPQ(indexINIQ),
										parte1);

								if ((repJoin->bt_len > 0)
										&& isBitSet(repJoin->bt, node2)) {
									//conttmp++;
									if (i != trepFila->maxLevel1 - 1) {
										commonAddItemXY(trepFila,
												rank(repJoin->bt, node2) * K1K1,
												l, j, div_level, indexINIQ,
												parte1);
									} else {
										commonAddItemXY(
												trepFila,
												repJoin->cutBt
														+ (rank(repJoin->bt,
																node2) * K1K1
																- repJoin->cutBt)
																/ AJUSTEK1K2, l,
												j, div_level, indexINIQ,
												parte1);
									}
									success++;
								}
							}
						indexINIQ++;
					}
					//Si no hay ninguno que coincida en la segunda parte del join,
					//se deshace la inserción
					if (!success) {
						conttmp--;
						trepFila->finq--;
					}
				}
			}
			RemoveItem2(trepFila);
			while (indicesRep[getPQ(trepFila->iniq)] != -1)
				RemoveItem2(trepFila);
		}
		queuecont = conttmp;
		checkLimites(trepFila);
	}

	for (i = 0; i < trepFila->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepFila->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {

				node = getNode(K2, j, div_level, getPQ(trepFila->iniq), parte2);

				if (isBitSet(repFila->bt, node)) {
					conttmp++;
					uint cutPreRank = rank(repFila->bt,
							repFila->lastBt1_len - 1);

					commonAddItem(
							trepFila,
							repFila->cutBt
									+ (rank(repFila->bt, node - 1) - cutPreRank)
											* K2K2, j, div_level,
							trepFila->iniq, parte2);

					//Exito en la de fila, se consulta el resto
					int indexINIQ = trepFila->iniq + 1;
					int success = 0;

					while (indicesRep[getPQ(indexINIQ)] != -1) {

						//Invertimos la indexacion
						MREP * repJoin = obtenerRep(trepCompleto,
								indicesRep[getPQ(indexINIQ)], indiceRel,
								parte1);

						int l;
						for (l = 0; l < K2; l++) {
							//La j es en la que estamos en la fila
							//Invertimos de la misma forma

							node2 = getNodeXY(K2, l, j, getPQ(indexINIQ),
									parte1);
							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								//conttmp++;
								uint cutPreRank = rank(repJoin->bt,
										repJoin->lastBt1_len - 1);
								//Invertimos de la misma forma

								commonAddItemXY(
										trepFila,
										repJoin->cutBt
												+ (rank(repJoin->bt, node2 - 1)
														- cutPreRank) * K2K2, l,
										j, div_level, indexINIQ, parte1);

								success++;
							}
						}
						indexINIQ++;
					}
					if (!success) {
						trepFila->finq--;
						conttmp--;
					}
				}

			}
			RemoveItem2(trepFila);
			while (trepFila->iniq <= trepFila->finq
					&& indicesRep[getPQ(trepFila->iniq)] != -1)
				RemoveItem2(trepFila);
		}
		queuecont = conttmp;
		checkLimites(trepFila);
	}

	int index, jj;

	while (trepFila->iniq <= trepFila->finq) {

		for (j = 0; j < K2; j++) {

			nleafrelat = getNode(K2, j, K2_2, getPQ(trepFila->iniq), parte2)
					- repFila->bt_len;

			//Es positivo para la fila
			if (isBitSet(repFila->bn, nleafrelat)) {
				posInf = rank(repFila->bn, nleafrelat);
				realvalue = accessFT(repFila->compressIL, posInf);
				index = 0;

				int indexINIQ = trepFila->iniq + 1;

				while (indicesRep[getPQ(indexINIQ)] != -1
						&& indexINIQ <= trepFila->finq) {
					for (i = 0; i < K2; i++) {

						MREP * repJoin = obtenerRep(trepCompleto,
								indicesRep[getPQ(indexINIQ)], indiceRel,
								parte1);

						nleafrelat = getNodeXY(K2, i, j, getPQ(indexINIQ),
								parte1) - repJoin->bt_len;

						if (isBitSet(repJoin->bn, nleafrelat)) {
							posInf = rank(repJoin->bn, nleafrelat);
							realvalueAux = accessFT(repJoin->compressIL,
									posInf);
							validos[index] = indexINIQ;
							validos2[index] = i;
							realValues[index++] = realvalueAux;
						}
					}
					indexINIQ++;
				}
				if (index) {
					for (i = 0; i < K2_2; i++) {
						//Es valido fila

						if (bitgetchar(&(trepFila->words[realvalue*trepFila->lenWords]),getPosRel(i,x,K2_2,parte2))) {
							for (k = 0; k < index; k++) {
								for (jj = 0; jj < K2_2; jj++) {

									if (bitgetchar(&(trepCompleto->words[realValues[k]*trepCompleto->lenWords]),getPosRelXY(jj,i,K2_2,parte1))) {
										infoTOTAL[0][0]++;
										if (infoTOTAL[0][0]
												== NUM_ELEMENTOS * 2) {
											fprintf(stderr, "numelementos_overflow!\n");
											exit(1);
										}

										infoTOTAL[0][infoTOTAL[0][0]] =
												getPosAbs(K2_2, i, j,
														getPQ(trepFila->iniq),
														trepFila->fila,
														trepFila->tamSubm,
														parte2);
										infoTOTAL[1][infoTOTAL[0][0]] =
												getPosAbs(
														K2_2,
														jj,
														validos2[k],
														validos[k],
														indicesRep[getPQ(validos[k])],
														trepCompleto->tamSubm,
														parte1);
									}
								}
							}
						}
					}
				}
			}
		}
		RemoveItem2(trepFila);
		while (trepFila->iniq <= trepFila->finq
				&& indicesRep[getPQ(trepFila->iniq)] != -1)
			RemoveItem2(trepFila);
	}
	return infoTOTAL[0][0];
}

uint columnasvsMatriz(TREP ** trepFilas, int x, TREP * trepCompleto,
		int indiceRel, int xrelatIn, int numPredicados, int xtot, INDICEDAC sp,
		INDICEDAC op, int condac, SET * pares, int parte1, int parte2) {
	//TODO: revisar los trepFila->columna, no deberia haber ninguno, deberian ser todos de fila.
	int node2;
	int AJUSTEK1K2 = 4;
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	MREP * repFila;
	int indexRel;

	TREP * trepIndex = trepCompleto;

	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, xrelat;

	if (!condac) {
		for (i = 1; i <= numPredicados; i++) {
			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			MREP * repFila = obtenerRep(trepFilas[i], xrelatIn, indiceRel,
					parte2);

			if (trepFilas[i]->numberOfEdges > 0 && repFila != NULL
					&& repFila->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, i, 1, parte2);
			}
		}
	} else {
		commonObtenerPredicados(sp, op, xtot, parte2);
		for (i = 0; i < resDAC.numeroPredicados; i++) {
			MREP * repFila = obtenerRep(trepFilas[resDAC.predicados[i]],
					xrelatIn, indiceRel, parte2);
			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			if (trepFilas[resDAC.predicados[i]]->numberOfEdges > 0
					&& repFila != NULL && repFila->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, resDAC.predicados[i], 1,
						parte2);
			}
		}
	}

	queuecont = 1;
	if (trepIndex->iniq == -1)
		return infoTOTAL[0][0];

	int vacio = 1;

	for (i = 0; i < trepCompleto->part; i++) {
		//Se añade una matriz

		if (obtenerRep(trepCompleto, i, indiceRel, parte1) != NULL) {
			vacio = 0;
			CommonInitItemJoin2(trepIndex, 0, 0, 0, i, 0, parte1);
		}
	}

	if (vacio)
		return infoTOTAL[0][0];

	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola
	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];

		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;

				while (esFilas[getPQ(indexRel)]) {

					xrelat = basex[getPQ(indexRel)];
					// Dentro de la fila correcta:
					// Valen todos log de la fila
					node = getNode(K1, j, div_level, getPQ(indexRel), parte2);
					//node = K1 * j + xrelat / div_level;
					//node += element[indexRel];

					repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
							xrelatIn, indiceRel, parte2);

					if (isBitSet(repFila->bt, node)) {
						//Si en la fila el valor es positivo
						//conttmp++;
						if (i != trepIndex->maxLevel1 - 1) {
							commonAddItem2(trepIndex,
									rank(repFila->bt, node) * K1K1, j,
									div_level, indexRel, 1, parte2);

						} else {
							commonAddItem2(
									trepIndex,
									repFila->cutBt
											+ (rank(repFila->bt, node) * K1K1
													- repFila->cutBt)
													/ factorAdjust, j,
									div_level, indexRel, 1, parte2);
						}
						insertados++;
					}

					indexRel++;
				}
				int success = 0;

				if (insertados) {
					//Mientras vengan elementos procedentes de la segunda parte del join
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)]) {
						MREP * repJoin = obtenerRep(trepCompleto,
								indicesRep[getPQ(indexRel)], indiceRel, parte1);
						int l;
						for (l = 0; l < K1; l++) {
							//La j es en la que estamos en la fila
							//Invertimos j y l
							node2 = getNodeXY(K1, l, j, getPQ(indexRel),
									parte1);
							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								//conttmp++;
								if (i != trepIndex->maxLevel1 - 1) {
									commonAddItemXY2(trepIndex,
											rank(repJoin->bt, node2) * K1K1, l,
											j, div_level, indexRel, 0, parte1);

								} else {
									commonAddItemXY2(
											trepIndex,
											repJoin->cutBt
													+ (rank(repJoin->bt, node2)
															* K1K1
															- repJoin->cutBt)
															/ AJUSTEK1K2, l, j,
											div_level, indexRel, 0, parte1);
								}
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}

				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		//MIRAR ESTE
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)]) {
					node = getNode(K2, j, div_level, getPQ(indexRel), parte2);

					repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
							xrelatIn, indiceRel, parte2);

					if (isBitSet(repFila->bt, node)) {
						//conttmp++;
						uint cutPreRank = rank(repFila->bt,
								repFila->lastBt1_len - 1);
						commonAddItem2(
								trepIndex,
								repFila->cutBt
										+ (rank(repFila->bt, node - 1)
												- cutPreRank) * K2K2, j,
								div_level, indexRel, 1, parte2);

						//Exito en la de fila, se consulta el resto
						insertados++;
					}
					indexRel++;
				}

				int success = 0;
				if (insertados) {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)]) {
						//Invertimos la indexacion
						MREP * repJoin = obtenerRep(trepCompleto,
								indicesRep[getPQ(indexRel)], indiceRel, parte1);
						int l;
						for (l = 0; l < K2; l++) {
							node2 = getNodeXY(K2, l, j, getPQ(indexRel),
									parte1);
							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								//conttmp++;
								uint cutPreRank = rank(repJoin->bt,
										repJoin->lastBt1_len - 1);
								//Invertimos de la misma forma
								commonAddItemXY2(
										trepIndex,
										repJoin->cutBt
												+ (rank(repJoin->bt, node2 - 1)
														- cutPreRank) * K2K2, l,
										j, div_level, indexRel, 0, parte1);
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}
				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index, jj;

	while (trepIndex->iniq <= trepIndex->finq) {
		for (j = 0; j < K2; j++) {

			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;
			//Comprobacion de las filas
			while (esFilas[getPQ(indexRel)]) {
				repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
						xrelatIn, indiceRel, parte2);
				nleafrelat = getNode(K2, j, K2_2, getPQ(indexRel), parte2)
						- repFila->bt_len;

				//Es positivo para la fila
				if (isBitSet(repFila->bn, nleafrelat)) {
					posInf = rank(repFila->bn, nleafrelat);
					realvalue = accessFT(repFila->compressIL, posInf);
					realFilas[insertados] = realvalue;
					validosFilas[insertados++] = indexRel;
				}
				indexRel++;
			}

			if (insertados) {

				while (!esFilas[getPQ(indexRel)] && indexRel <= trepIndex->finq) {
					for (i = 0; i < K2; i++) {
						MREP * repJoin = obtenerRep(trepCompleto,
								indicesRep[getPQ(indexRel)], indiceRel, parte1);
						nleafrelat = getNodeXY(K2, i, j, getPQ(indexRel),
								parte1) - repJoin->bt_len;
						if (isBitSet(repJoin->bn, nleafrelat)) {
							posInf = rank(repJoin->bn, nleafrelat);
							realvalueAux = accessFT(repJoin->compressIL,
									posInf);
							validos[index] = indexRel;
							validos2[index] = i;
							realValues[index++] = realvalueAux;
						}
					}
					indexRel++;
				}

				if (index) {
					for (i = 0; i < K2_2; i++) {
						//Es valido fila
						int m;
						for (m = 0; m < insertados; m++) {
							if (bitgetchar(&(trepFilas[indicesRep[getPQ(validosFilas[m])]]->words[realFilas[m]*trepFilas[indicesRep[validosFilas[m]]]->lenWords]),getPosRel(i,x,K2_2,parte2))) {
								for (k = 0; k < index; k++) {
									for (jj = 0; jj < K2_2; jj++) {
										if (bitgetchar(&(trepCompleto->words[realValues[k]*trepCompleto->lenWords]),getPosRelXY(jj,i,K2_2,parte1))) {
											pares[0].x++;
											if (pares[0].x == NUM_ELEMENTOS) {
												fprintf(stderr, "numelementos_overflow!\n");
												exit(1);
											}

											pares[pares[0].x].x =
													getPosAbs(
															K2_2,
															jj,
															validos2[k],
															validos[k],
															indicesRep[getPQ(validos[k])],
															trepCompleto->tamSubm,
															parte1);
											/*basex[validos[k]] + validos2[k] * K2_2 + jj
											 + indicesRep[validos[k]]
											 * trepCompleto->tamSubm;*/
											pares[pares[0].x].y = getPosAbs(
													K2_2, i, j,
													getPQ(validosFilas[m]),
													indiceRel,
													trepIndex->tamSubm, parte2);

											/*basey[validosFilas[m]] + i
											 + K2_2 * j + indiceRel * trepIndex->tamSubm;*/
											pares[pares[0].x].z =
													indicesRep[getPQ(validosFilas[m])];
											/*	fprintf(
											 stderr,
											 "<%d,%d,%d>-<%d,%d,%d>\n",
											 infoTOTAL[1][infoTOTAL[0][0]]
											 + 1,

											 pred,

											 infoTOTAL[0][infoTOTAL[0][0]]
											 + 1,
											 infoTOTAL[0][infoTOTAL[0][0]]
											 + 1,
											 indicesRep[validosFilas[m]],
											 xtot + 1);
											 */
										}
									}
								}
							}
						}
					}
				}
			} else {
				while (!(indexRel > trepIndex->finq)
						&& !esFilas[getPQ(indexRel)])
					indexRel++;
			}
		}
		trepIndex->iniq = indexRel;
	}
	return pares[0].x;
}

// A partir de columnasvsMatrices
uint columnavsMatrices(TREP ** trepFilas, int x, int p1, int indiceRel,
		int xrelatIn, int numPredicados, int xtot, INDICEDAC sp, INDICEDAC op,
		int condac, SET * pares, int parte1, int parte2) {
	int node2;
	int AJUSTEK1K2 = 4;
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;

	int indexRel;

	TREP * trepIndex = trepFilas[1];
	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level;

	MREP * repFila = obtenerRep(trepFilas[p1], xrelatIn, indiceRel, parte1);

	if (trepFilas[p1]->numberOfEdges > 0 && repFila != NULL && repFila->numberOfEdges) {
		CommonInitItemJoin2(trepIndex, 0, x,0,p1, -1, parte1);
	}

	queuecont = 1;
	if (trepIndex->iniq == -1)
		return infoTOTAL[0][0];

	for (j = 1; j <= numPredicados; j++) {
		for (i = 0; i < trepIndex->part; i++) {
			//Se añade una matriz
			//j->predicado i->columna dentro de la matriz
			CommonInitItemJoin2(trepIndex, 0, 0, 0, j, i, parte2);
		}
	}

	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {

			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)] == -1) {

					node = getNode(K1, j, div_level, getPQ(indexRel), parte1);

					if (isBitSet(repFila->bt, node)) {
						//Si en la fila el valor es positivo
						//conttmp++;
						if (i != trepIndex->maxLevel1 - 1) {
							commonAddItem2(trepIndex,
									rank(repFila->bt, node) * K1K1, j,
									div_level, indexRel, -1, parte1);
						} else {
							commonAddItem2(
									trepIndex,
									repFila->cutBt
											+ (rank(repFila->bt, node) * K1K1
													- repFila->cutBt)
													/ factorAdjust, j,
									div_level, indexRel, -1, parte1);
						}
						insertados++;
					}

					indexRel++;
				}
				int success = 0;

				if (insertados) {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)] != -1) {

						//Modificamos la matriz donde se trabaja:ahora se escoge el predicado en funcion de esFilas.
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								esFilas[getPQ(indexRel)], indiceRel, parte2);
						if (repJoin != NULL) {
							int l;
							for (l = 0; l < K1; l++) {
								//La j es en la que estamos en la fila
								//Invertimos j y l
								node2 = getNodeXY(K1, l, j, getPQ(indexRel),
										parte2);
								if ((repJoin->bt_len > 0)
										&& isBitSet(repJoin->bt, node2)) {
									//conttmp++;
									if (i != trepIndex->maxLevel1 - 1) {
										commonAddItemXY2(trepIndex,
												rank(repJoin->bt, node2) * K1K1,
												l, j, div_level, indexRel,
												esFilas[getPQ(indexRel)],
												parte2);
									} else {
										commonAddItemXY2(
												trepIndex,
												repJoin->cutBt
														+ (rank(repJoin->bt,
																node2) * K1K1
																- repJoin->cutBt)
																/ AJUSTEK1K2, l,
												j, div_level, indexRel,
												esFilas[getPQ(indexRel)],
												parte2);
									}
									success++;
								}
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)] != -1)
						indexRel++;
				}

				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		//MIRAR ESTE
		queuecont = conttmp;
		checkLimites(trepIndex);
	}



	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)] == -1) {
					node = getNode(K2, j, div_level, getPQ(indexRel), parte1);
				
					if (isBitSet(repFila->bt, node)) {
						//conttmp++;
						uint cutPreRank = rank(repFila->bt,
								repFila->lastBt1_len - 1);
						
						commonAddItem2(
								trepIndex,
								repFila->cutBt
										+ (rank(repFila->bt, node - 1)
												- cutPreRank) * K2K2, j,
								div_level, indexRel, -1, parte1);
						//Exito en la de fila, se consulta el resto
						insertados++;
					}
					indexRel++;
				}
				
				int success = 0;
				if (insertados) {

					while (!(indexRel > trepIndex->finq)
							&& (esFilas[getPQ(indexRel)] != -1)) {
						//Invertimos la indexacion
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								esFilas[getPQ(indexRel)], indiceRel, parte2);
						int l;
						for (l = 0; l < K2; l++) {
							//La j es en la que estamos en la fila
							//Invertimos de la misma forma
							node2 = getNodeXY(K2, l, j, getPQ(indexRel),
									parte2);
							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								//conttmp++;
								uint cutPreRank = rank(repJoin->bt,
										repJoin->lastBt1_len - 1);
								//Invertimos de la misma forma
								commonAddItemXY2(
										trepIndex,
										repJoin->cutBt
												+ (rank(repJoin->bt, node2 - 1)
														- cutPreRank) * K2K2, l,
										j, div_level, indexRel,
										esFilas[getPQ(indexRel)], parte2);
								success++;
							}

						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& (esFilas[getPQ(indexRel)] != -1)){
						indexRel++;

					}

				}
				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}

		queuecont = conttmp;

		checkLimites(trepIndex);

	}

	int index, jj;

	while (trepIndex->iniq <= trepIndex->finq) {



		for (j = 0; j < K2; j++) {

			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;
			//Comprobacion de las filas
			while (esFilas[getPQ(indexRel)] == -1) {

				nleafrelat = getNode(K2, j, K2_2, getPQ(indexRel), parte1)
						- repFila->bt_len;

				//Es positivo para la fila
				if (isBitSet(repFila->bn, nleafrelat)) {
					posInf = rank(repFila->bn, nleafrelat);
					realvalue = accessFT(repFila->compressIL, posInf);
					realFilas[insertados] = realvalue;
					validosFilas[insertados++] = indexRel;
				}

				indexRel++;
			}

			if (insertados) {

				while (esFilas[getPQ(indexRel)] != -1
						&& indexRel <= trepIndex->finq) {
					for (i = 0; i < K2; i++) {
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								esFilas[getPQ(indexRel)], indiceRel, parte2);
						nleafrelat = getNodeXY(K2, i, j, getPQ(indexRel),
								parte2) - repJoin->bt_len;

						if (isBitSet(repJoin->bn, nleafrelat)) {
							posInf = rank(repJoin->bn, nleafrelat);
							realvalueAux = accessFT(repJoin->compressIL,
									posInf);
							validos[index] = indexRel;
							validos2[index] = i;
							realValues[index++] = realvalueAux;
						}
					}
					indexRel++;
				}

				if (index) {
					for (i = 0; i < K2_2; i++) {
						//Es valido fila
						int m;
						for (m = 0; m < insertados; m++) {
							if (bitgetchar(&(trepFilas[p1]->words[realFilas[m]*trepFilas[p1]->lenWords]),getPosRel(i,x,K2_2,parte1))) {
								for (k = 0; k < index; k++) {
									TREP * trepCompleto =
											trepFilas[indicesRep[getPQ(validos[k])]];
									for (jj = 0; jj < K2_2; jj++) {
										if (bitgetchar(&(trepCompleto->words[realValues[k]*trepCompleto->lenWords]),getPosRelXY(jj,i,K2_2,parte2))) {
											pares[0].x++;
											if (pares[0].x == NUM_ELEMENTOS) {
												fprintf(stderr, "numelementos_overflow!\n");
												exit(1);
											}
											pares[pares[0].x].x = getPosAbs(
													K2_2, jj, validos2[k],
													getPQ(validos[k]),
													esFilas[getPQ(validos[k])],
													trepCompleto->tamSubm,
													parte2);
											pares[pares[0].x].y =
													indicesRep[getPQ(validos[k])];
											pares[pares[0].x].z = getPosAbs(
													K2_2, i, j,
													getPQ(validosFilas[m]),
													indiceRel,
													trepIndex->tamSubm, parte1);
											/*pares[pares[0].x].w =
													indicesRep[getPQ(validosFilas[m])];*/
										}
									}
								}
							}
						}
					}
				}
			} else {
				while (!(indexRel > trepIndex->finq)
						&& esFilas[getPQ(indexRel)] != -1){
					indexRel++;
				}
			}
		}
		trepIndex->iniq = indexRel;
	}
	return pares[0].x;
}

// A partir de columnasvsMatriz
uint columnasvsMatrices(TREP ** trepFilas, int x, int indiceRel, int xrelatIn,
		int numPredicados, int xtot, INDICEDAC sp, INDICEDAC op, int condac,
		SET * pares, int parte1, int parte2) {
	int node2;
	int AJUSTEK1K2 = 4;
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	MREP * repFila;
	int indexRel;

	TREP * trepIndex = trepFilas[1];
	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level;

	if (!condac) {
		for (i = 1; i <= numPredicados; i++) {
			MREP * rep = obtenerRep(trepFilas[i], xrelatIn, indiceRel, parte2);
			//Se aniaden tantas matrices como numeros de predicados: son numpredicados columnas a analizar
			if (trepFilas[i]->numberOfEdges > 0 && rep != NULL
					&& rep->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, i, -1, parte2);
			}
		}
	} else {
		commonObtenerPredicados(sp, op, xtot, parte2);

		for (i = 0; i < resDAC.numeroPredicados; i++) {
			MREP * rep = obtenerRep(trepFilas[resDAC.predicados[i]], xrelatIn,
					indiceRel, parte2);

			if (trepFilas[resDAC.predicados[i]]->numberOfEdges > 0
					&& rep != NULL && rep->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, x, 0, resDAC.predicados[i],
						-1, parte2);
			}
		}
	}

	queuecont = 1;
	if (trepIndex->iniq == -1)
		return infoTOTAL[0][0];

	for (j = 1; j <= numPredicados; j++) {
		for (i = 0; i < trepIndex->part; i++) {
			//Se añade una matriz
			//j->predicado i->columna dentro de la matriz
			CommonInitItemJoin2(trepIndex, 0, 0, 0, j, i, parte1);
		}
	}

	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];

		for (k = 0; k < queuecont; k++) {

			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)] == -1) {

					node = getNode(K1, j, div_level, getPQ(indexRel), parte2);
					repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
							xrelatIn, indiceRel, parte2);

					if (isBitSet(repFila->bt, node)) {
						//Si en la fila el valor es positivo
						//conttmp++;
						if (i != trepIndex->maxLevel1 - 1) {
							commonAddItem2(trepIndex,
									rank(repFila->bt, node) * K1K1, j,
									div_level, indexRel, -1, parte2);
						} else {
							commonAddItem2(
									trepIndex,
									repFila->cutBt
											+ (rank(repFila->bt, node) * K1K1
													- repFila->cutBt)
													/ factorAdjust, j,
									div_level, indexRel, -1, parte2);
						}
						insertados++;
					}

					indexRel++;
				}
				int success = 0;

				if (insertados) {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)] != -1) {

						//Modificamos la matriz donde se trabaja:ahora se escoge el predicado en funcion de esFilas.
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								esFilas[getPQ(indexRel)], indiceRel, parte1);
						if (repJoin != NULL) {
							int l;
							for (l = 0; l < K1; l++) {
								//La j es en la que estamos en la fila
								//Invertimos j y l
								node2 = getNodeXY(K1, l, j, getPQ(indexRel),
										parte1);
								if ((repJoin->bt_len > 0)
										&& isBitSet(repJoin->bt, node2)) {
									//conttmp++;
									if (i != trepIndex->maxLevel1 - 1) {
										commonAddItemXY2(trepIndex,
												rank(repJoin->bt, node2) * K1K1,
												l, j, div_level, indexRel,
												esFilas[getPQ(indexRel)],
												parte1);
									} else {
										commonAddItemXY2(
												trepIndex,
												repJoin->cutBt
														+ (rank(repJoin->bt,
																node2) * K1K1
																- repJoin->cutBt)
																/ AJUSTEK1K2, l,
												j, div_level, indexRel,
												esFilas[getPQ(indexRel)],
												parte1);
									}
									success++;
								}
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)] != -1)
						indexRel++;
				}

				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		//MIRAR ESTE
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)] == -1) {
					node = getNode(K2, j, div_level, getPQ(indexRel), parte2);
					repFila = obtenerRep(trepFilas[indicesRep[getPQ(indexRel)]],
							xrelatIn, indiceRel, parte2);

					if (isBitSet(repFila->bt, node)) {
						//conttmp++;
						uint cutPreRank = rank(repFila->bt,
								repFila->lastBt1_len - 1);

						commonAddItem2(
								trepIndex,
								repFila->cutBt
										+ (rank(repFila->bt, node - 1)
												- cutPreRank) * K2K2, j,
								div_level, indexRel, -1, parte2);
						//Exito en la de fila, se consulta el resto
						insertados++;
					}
					indexRel++;
				}

				int success = 0;
				if (insertados) {

					while (!(indexRel > trepIndex->finq)
							&& (esFilas[getPQ(indexRel)] != -1)) {
						//Invertimos la indexacion
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								esFilas[getPQ(indexRel)], indiceRel, parte1);
						int l;
						for (l = 0; l < K2; l++) {
							//La j es en la que estamos en la fila
							//Invertimos de la misma forma
							node2 = getNodeXY(K2, l, j, getPQ(indexRel),
									parte1);
							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								//conttmp++;
								uint cutPreRank = rank(repJoin->bt,
										repJoin->lastBt1_len - 1);
								//Invertimos de la misma forma
								commonAddItemXY2(
										trepIndex,
										repJoin->cutBt
												+ (rank(repJoin->bt, node2 - 1)
														- cutPreRank) * K2K2, l,
										j, div_level, indexRel,
										esFilas[getPQ(indexRel)], parte1);
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& (esFilas[getPQ(indexRel)] != -1))
						indexRel++;
				}
				//Si no hay ninguno que coincida en la segunda parte del join,
				//se deshace la inserción
				if (!success) {
					//conttmp -= insertados;
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			//Hay que eliminar todas las filas y todas las columnas
			//Se borran las filas
			trepIndex->iniq = indexRel;
		}
		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index, jj;

	while (trepIndex->iniq <= trepIndex->finq) {
		for (j = 0; j < K2; j++) {

			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;
			//Comprobacion de las filas
			while (esFilas[getPQ(indexRel)] == -1) {
				MREP * repFila = obtenerRep(
						trepFilas[indicesRep[getPQ(indexRel)]], xrelatIn,
						indiceRel, parte2);

				nleafrelat = getNode(K2, j, K2_2, getPQ(indexRel), parte2)
						- repFila->bt_len;

				//Es positivo para la fila
				if (isBitSet(repFila->bn, nleafrelat)) {
					posInf = rank(repFila->bn, nleafrelat);
					realvalue = accessFT(repFila->compressIL, posInf);
					realFilas[insertados] = realvalue;
					validosFilas[insertados++] = indexRel;
				}
				indexRel++;
			}

			if (insertados) {

				while (esFilas[getPQ(indexRel)] != -1
						&& indexRel <= trepIndex->finq) {
					for (i = 0; i < K2; i++) {
						MREP * repJoin = obtenerRep(
								trepFilas[indicesRep[getPQ(indexRel)]],
								esFilas[getPQ(indexRel)], indiceRel, parte1);
						nleafrelat = getNodeXY(K2, i, j, getPQ(indexRel),
								parte1) - repJoin->bt_len;

						if (isBitSet(repJoin->bn, nleafrelat)) {
							posInf = rank(repJoin->bn, nleafrelat);
							realvalueAux = accessFT(repJoin->compressIL,
									posInf);
							validos[index] = indexRel;
							validos2[index] = i;
							realValues[index++] = realvalueAux;
						}
					}
					indexRel++;
				}

				if (index) {
					for (i = 0; i < K2_2; i++) {
						//Es valido fila
						int m;
						for (m = 0; m < insertados; m++) {
							if (bitgetchar(&(trepFilas[indicesRep[getPQ(validosFilas[m])]]->words[realFilas[m]*trepFilas[indicesRep[validosFilas[m]]]->lenWords]),getPosRel(i,x,K2_2,parte2))) {
								for (k = 0; k < index; k++) {
									TREP * trepCompleto =
											trepFilas[indicesRep[getPQ(validos[k])]];
									for (jj = 0; jj < K2_2; jj++) {
										if (bitgetchar(&(trepCompleto->words[realValues[k]*trepCompleto->lenWords]),getPosRelXY(jj,i,K2_2,parte1))) {
											pares[0].x++;
											if (pares[0].x == NUM_ELEMENTOS) {
												fprintf(stderr, "numelementos_overflow!\n");
												exit(1);
											}
											pares[pares[0].x].x = getPosAbs(
													K2_2, jj, validos2[k],
													getPQ(validos[k]),
													esFilas[getPQ(validos[k])],
													trepCompleto->tamSubm,
													parte1);
											pares[pares[0].x].y =
													indicesRep[getPQ(validos[k])];
											pares[pares[0].x].z = getPosAbs(
													K2_2, i, j,
													getPQ(validosFilas[m]),
													indiceRel,
													trepIndex->tamSubm, parte2);
											pares[pares[0].x].w =
													indicesRep[getPQ(validosFilas[m])];
										}
									}
								}
							}
						}
					}
				}
			} else {
				while (!(indexRel > trepIndex->finq)
						&& esFilas[getPQ(indexRel)] != -1)
					indexRel++;
			}
		}
		trepIndex->iniq = indexRel;
	}
	return pares[0].x;
}

uint matrizvsMatrices(TREP * trep1, int pred1, TREP ** treps, int numPredicados,
		int indiceRel, SET * pares, int parte1a, int parte2a) {

	int node2;
	int AJUSTEK1K2 = 4;
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	MREP * repFila;
	int indexRel;
	int m, r;
	TREP * trepIndex = trep1;

	int parte1 = parte1a;
	int parte2 = parte2a;
	if (parte1 == parte2) {
		if (parte1 == PORSUJETO) {
			parte1 = POROBJETO;
			parte2 = POROBJETO;
		} else {
			parte1 = PORSUJETO;
			parte2 = PORSUJETO;
		}
	}

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level;

	int vacio = 1;
	//PARTE 1-->FILAS
	for (i = 0; i < trep1->part; i++) {
		//Se añade una matriz
		MREP * rep = obtenerRep(trep1, indiceRel, i, parte2);
		if (trep1->numberOfEdges > 0 && rep != NULL && rep->numberOfEdges) {

			//esFilas==0: implica filas
			CommonInitItemJoin2(trepIndex, 0, 0, 0, i, 0, parte2);
			vacio = 0;
		}
	}
	if (vacio)
		return 0;
	vacio = 1;

	//PARTE 2--> COLUMNAS
	for (j = 1; j <= numPredicados; j++) {
		for (i = 0; i < trep1->part; i++) {
			//esFilas>0 almacena el predicado
			MREP * rep = obtenerRep(treps[j], indiceRel, i, parte1);
			if (treps[j]->numberOfEdges > 0 && rep != NULL
					&& rep->numberOfEdges) {
				CommonInitItemJoin2(trepIndex, 0, 0, 0, i, j, parte1);
				vacio = 0;
			}
		}
	}

	if (vacio)
		return 0;

	queuecont = 1;

	// ------------------------------------------------------------
	// ------------ PRIMERA PARTE ---------------------------------
	// ------------------------------------------------------------

	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola
	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			//INSPECCION DE LAS FILAS
			//Esta j tiene q ser la columna
			//de la primera parte
			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;

				int insertados = 0;

				while (!esFilas[getPQ(indexRel)]
						&& !(indexRel > trepIndex->finq)) {

					//Aniadido bucle para manejar las columnas tb.
					//m->representan las filas de esa primera parte
					for (m = 0; m < K1; m++) {
						node = getNodeXY(K1, j, m, getPQ(indexRel), parte2);
						repFila = obtenerRep(trep1, indiceRel,
								indicesRep[getPQ(indexRel)], parte2);

						if (repFila->bt_len > 0
								&& isBitSet(repFila->bt, node)) {
							//Si en la fila el valor es positivo
							if (i != trepIndex->maxLevel1 - 1) {
								commonAddItemXY2(trepIndex,
										rank(repFila->bt, node) * K1K1, j, m,
										div_level, indexRel, 0, parte2);
							} else {
								commonAddItemXY2(
										trepIndex,
										repFila->cutBt
												+ (rank(repFila->bt, node)
														* K1K1 - repFila->cutBt)
														/ AJUSTEK1K2, j, m,
										div_level, indexRel, 0, parte2);
							}
							insertados++;
						}
					}
					indexRel++;
				}

				int success = 0;

				if (insertados) {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)]) {
						MREP * repJoin = obtenerRep(
								treps[esFilas[getPQ(indexRel)]], indiceRel,
								indicesRep[getPQ(indexRel)], parte1);
						int l;
						for (l = 0; l < K1; l++) {
							node2 = getNodeXY(K1, j, l, getPQ(indexRel),
									parte1);
							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {

								if (i != trepIndex->maxLevel1 - 1) {
									commonAddItemXY2(trepIndex,
											rank(repJoin->bt, node2) * K1K1, j,
											l, div_level, indexRel,
											esFilas[getPQ(indexRel)], parte1);
								} else {
									commonAddItemXY2(
											trepIndex,
											repJoin->cutBt
													+ (rank(repJoin->bt, node2)
															* K1K1
															- repJoin->cutBt)
															/ AJUSTEK1K2, j, l,
											div_level, indexRel,
											esFilas[getPQ(indexRel)], parte1);
								}
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)])
						indexRel++;
				}

				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			trepIndex->iniq = indexRel;
		}

		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	// ------------------------------------------------------------
	// ------------ SEGUNDA PARTE ---------------------------------
	// ------------------------------------------------------------

	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (!esFilas[getPQ(indexRel)]
						&& !(indexRel > trepIndex->finq)) {
					for (m = 0; m < K2; m++) {
						node = getNodeXY(K2, j, m, getPQ(indexRel), parte2);
						repFila = obtenerRep(trep1, indiceRel,
								indicesRep[getPQ(indexRel)], parte2);

						if (isBitSet(repFila->bt, node)) {

							uint cutPreRank = rank(repFila->bt,
									repFila->lastBt1_len - 1);

							commonAddItemXY2(
									trepIndex,
									repFila->cutBt
											+ (rank(repFila->bt, node - 1)
													- cutPreRank) * K2K2, j, m,
									div_level, indexRel, 0, parte2);
							insertados++;
						}
					}
					indexRel++;
				}

				int success = 0;
				if (insertados) {
					while (!(indexRel > trepIndex->finq) && esFilas[getPQ(indexRel)]) {
						MREP * repJoin = obtenerRep(
								treps[esFilas[getPQ(indexRel)]], indiceRel,
								indicesRep[getPQ(indexRel)], parte1);

						int l;
						for (l = 0; l < K2; l++) {
							node2 = getNodeXY(K2, j, l, getPQ(indexRel),
									parte1);

							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								uint cutPreRank = rank(repJoin->bt,
										repJoin->lastBt1_len - 1);
								commonAddItemXY2(
										trepIndex,
										repJoin->cutBt
												+ (rank(repJoin->bt, node2 - 1)
														- cutPreRank) * K2K2, j,
										l, div_level, indexRel,
										esFilas[getPQ(indexRel)], parte1);
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& esFilas[getPQ(indexRel)])
						indexRel++;
				}
				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			trepIndex->iniq = indexRel;
		}

		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index, jj;

	// ------------------------------------------------------------
	// ------------ TERCERA PARTE ---------------------------------
	// ------------------------------------------------------------

	while (trepIndex->iniq <= trepIndex->finq) {

		for (j = 0; j < K2; j++) {
			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;

			//Comprobacion de las filas
			while (!esFilas[getPQ(indexRel)] && !(indexRel > trepIndex->finq)) {
				//Aniadido bucle
				for (m = 0; m < K2; m++) {
					repFila = obtenerRep(trep1, indiceRel,
							indicesRep[getPQ(indexRel)], parte2);
					nleafrelat = getNodeXY(K2, j, m, getPQ(indexRel), parte2)
							- repFila->bt_len;
					//Es positivo para la fila
					if (isBitSet(repFila->bn, nleafrelat)) {
						posInf = rank(repFila->bn, nleafrelat);
						realvalue = accessFT(repFila->compressIL, posInf);
						realFilas[insertados] = realvalue;
						validosFilas2[insertados] = m;
						validosFilas[insertados++] = indexRel;
					}
				}
				indexRel++;
			}

			if (insertados) {
				while (esFilas[getPQ(indexRel)] && indexRel <= trepIndex->finq) {
					for (i = 0; i < K2; i++) {
						MREP * repJoin = obtenerRep(
								treps[esFilas[getPQ(indexRel)]], indiceRel,
								indicesRep[getPQ(indexRel)], parte1);
						nleafrelat = getNodeXY(K2, j, i, getPQ(indexRel),
								parte1) - repJoin->bt_len;

						if (isBitSet(repJoin->bn, nleafrelat)) {
							posInf = rank(repJoin->bn, nleafrelat);
							realvalueAux = accessFT(repJoin->compressIL,
									posInf);
							validos[index] = indexRel;
							validos2[index] = i;
							realValues[index++] = realvalueAux;
						}
					}
					indexRel++;
				}

				if (index) {
					for (i = 0; i < K2_2; i++) {
						//Es valido fila
						for (r = 0; r < K2_2; r++) {
							int m;
							for (m = 0; m < insertados; m++) {
								if (bitgetchar(
										&(trep1->words[realFilas[m]
												* trep1->lenWords]),
										getPosRelXY(i,r,K2_2,parte2))) {
									for (k = 0; k < index; k++) {
										for (jj = 0; jj < K2_2; jj++) {
											TREP * trep2 =
													treps[esFilas[getPQ(validos[k])]];
											if (bitgetchar(
													&(trep2->words[realValues[k]
															* trep2->lenWords]),
													getPosRelXY(i,jj,K2_2,parte1))) {

												pares[0].x++;
												if (pares[0].x
														== NUM_ELEMENTOS) {
													fprintf(stderr,
															"numelementos_overflow!\n");
													exit(1);
												}

												pares[pares[0].x].x =
														getPosAbs2(
																K2_2,
																r,
																validosFilas2[m],
																getPQ(validosFilas[m]),
																indicesRep[getPQ(validosFilas[m])],
																trep1->tamSubm,
																parte2);
												pares[pares[0].x].y = getPosAbs(
														K2_2, i, j,
														getPQ(validos[k]),
														indiceRel,
														trep2->tamSubm, parte1);

												pares[pares[0].x].z =
														esFilas[getPQ(validos[k])];
												pares[pares[0].x].w =
														getPosAbs2(
																K2_2,
																jj,
																validos2[k],
																getPQ(validos[k]),/*trep2->fila*/
																indicesRep[getPQ(validos[k])],
																trep2->tamSubm,
																parte1);

												/*
												 pares[pares[0].x].x = getPosAbs(K2_2,jj,validos2[k],validos[k],trep2->fila,trep2->tamSubm,parte1);


												 pares[pares[0].x].y =
												 getPosAbs2(K2_2,i,j,validos[k],indicesRep[validos[k]],trep2->tamSubm,parte1);
												 //getPosAbs(K2_2,i,j,validosFilas[m],indicesRep[validosFilas[m]],trep2->tamSubm,parte2);

												 pares[pares[0].x].z = getPosAbs2(K2_2,r,validosFilas2[m],validosFilas[m],indicesRep[validosFilas[m]],trep1->tamSubm,parte2);
												 */

											}
										}
									}
								}
							}
						}
					}
				}
			} else {
				while (!(indexRel > trepIndex->finq) && esFilas[getPQ(indexRel)])
					indexRel++;
			}
		}
		trepIndex->iniq = indexRel;
	}
	return pares[0].x;
}

uint matrizvsMatriz(TREP * trep1, TREP * trep2, SET * pares, int parte1,
		int parte2) {
	int node2;
	int AJUSTEK1K2 = 4;
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	MREP * repFila;
	int indexRel;
	int m, r;
	int indiceRel = trep1->columna;

	TREP * trepIndex = trep1;

	uint factorAdjust = K1K1 / K2K2;

	//Estos índices van a gestionar la cola completa
	trepIndex->iniq = -1;
	trepIndex->finq = -1;
	uint posInf, realvalue, realvalueAux, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, xrelat;

	for (i = 0; i < trep1->part; i++) {

		MREP * rep = obtenerRep(trep1, i, indiceRel, parte2);
		if (trep1->numberOfEdges > 0 && rep != NULL && rep->numberOfEdges) {
			CommonInitItemJoin2(trepIndex, 0, 0, 0, i, 1, parte2);
		}
	}

	queuecont = 1;
	if (trepIndex->iniq == -1)
		return infoTOTAL[0][0];

	int vacio = 1;
	for (i = 0; i < trep2->part; i++) {
		//Se añade una matriz
		MREP * rep = obtenerRep(trep2, i, indiceRel, parte1);
		if (trep2->numberOfEdges > 0 && rep != NULL && rep->numberOfEdges) {
			CommonInitItemJoin2(trepIndex, 0, 0, 0, i, 0, parte1);
			vacio = 0;
		}
	}

	if (vacio)
		return 0;

	//Queuecont va a marcar los elementos de trepFila que tenemos en la cola
	for (i = 0; i < trepIndex->maxLevel1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {

			for (j = 0; j < K1; j++) {

				indexRel = trepIndex->iniq;
				int insertados = 0;

				while (esFilas[getPQ(indexRel)] && !(indexRel > trepIndex->finq)) {

					//Aniadido bucle para manejar las columnas tb.
					for (m = 0; m < K1; m++) {
						// Dentro de la fila correcta:
						// Valen todos log de la fila
						node = getNodeXY(K1, m, j , getPQ(indexRel), parte2);

						repFila = obtenerRep(trep1, indicesRep[getPQ(indexRel)],
								indiceRel, parte2);

						if (repFila->bt_len > 0
								&& isBitSet(repFila->bt, node)) {
							//Si en la fila el valor es positivo
							//conttmp++;
							if (i != trepIndex->maxLevel1 - 1) {

								commonAddItemXY2(trepIndex,
										rank(repFila->bt, node) * K1K1, m,j,
										div_level, indexRel, 1, parte2);

							} else {
								commonAddItemXY2(
										trepIndex,
										repFila->cutBt
												+ (rank(repFila->bt, node)
														* K1K1 - repFila->cutBt)
														/ factorAdjust, m,j,
										div_level, indexRel, 1, parte2);
							}
							insertados++;
						}
					}
					indexRel++;
				}
				int success = 0;

				if (insertados) {
					while (!(indexRel > trepIndex->finq) && !esFilas[getPQ(indexRel)]) {

						MREP * repJoin = obtenerRep(trep2,
								indicesRep[getPQ(indexRel)], indiceRel, parte1);

						int l;
						for (l = 0; l < K1; l++) {

							node2 = getNodeXY(K1, l, j, getPQ(indexRel),
									parte1);

							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								if (i != trepIndex->maxLevel1 - 1) {
									commonAddItemXY2(trepIndex,
											rank(repJoin->bt, node2) * K1K1, l,
											j, div_level, indexRel, 0, parte1);
								} else {

									commonAddItemXY2(
											trepIndex,
											repJoin->cutBt
													+ (rank(repJoin->bt, node2)
															* K1K1
															- repJoin->cutBt)
															/ AJUSTEK1K2, l, j,
											div_level, indexRel, 0, parte1);

								}
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}

				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			trepIndex->iniq = indexRel;
		}

		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	for (i = 0; i < trepIndex->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trepIndex->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				indexRel = trepIndex->iniq;
				int insertados = 0;
				while (esFilas[getPQ(indexRel)]) {
					for (m = 0; m < K2; m++) {
						/*xrelat = basex[getPQ(indexRel)];
						node = K2 * j + m;
						node += element[getPQ(indexRel)];
						*/
						node = getNodeXY(K2, m,j, getPQ(indexRel), parte2);

						repFila = obtenerRep(trep1, indicesRep[getPQ(indexRel)],
								indiceRel, parte2);

						if (isBitSet(repFila->bt, node)) {
							uint cutPreRank = rank(repFila->bt,
									repFila->lastBt1_len - 1);

							commonAddItemXY2(
									trepIndex,
									repFila->cutBt
											+ (rank(repFila->bt, node - 1)
													- cutPreRank) * K2K2, m,j ,
									div_level, indexRel, 1, parte2);
							insertados++;
						}
					}
					indexRel++;
				}

				int success = 0;
				if (insertados) {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)]) {
						MREP * repJoin = obtenerRep(trep2,
								indicesRep[getPQ(indexRel)], indiceRel, parte1);
						int l;
						for (l = 0; l < K2; l++) {

							node2 = getNodeXY(K2, l, j, getPQ(indexRel),
									parte1);

							if ((repJoin->bt_len > 0)
									&& isBitSet(repJoin->bt, node2)) {
								uint cutPreRank = rank(repJoin->bt,
										repJoin->lastBt1_len - 1);
								commonAddItemXY2(
										trepIndex,
										repJoin->cutBt
												+ (rank(repJoin->bt, node2 - 1)
														- cutPreRank) * K2K2, l,
										j, div_level, indexRel, 0, parte1);
								success++;
							}
						}
						indexRel++;
					}
				} else {
					while (!(indexRel > trepIndex->finq)
							&& !esFilas[getPQ(indexRel)])
						indexRel++;
				}
				if (!success) {
					trepIndex->finq -= insertados;
				} else {
					conttmp++;
				}
			}
			trepIndex->iniq = indexRel;
		}

		queuecont = conttmp;
		checkLimites(trepIndex);
	}

	int index, jj;

	while (trepIndex->iniq <= trepIndex->finq) {

		for (j = 0; j < K2; j++) {
			indexRel = trepIndex->iniq;
			int insertados = 0;
			index = 0;

			while (esFilas[getPQ(indexRel)]) {
				for (m = 0; m < K2; m++) {
					repFila = obtenerRep(trep1, indicesRep[getPQ(indexRel)],
							indiceRel, parte2);

					nleafrelat = getNodeXY(K2, m,j, getPQ(indexRel), parte2)
							- repFila->bt_len;

					//Es positivo para la fila
					if (isBitSet(repFila->bn, nleafrelat)) {
						posInf = rank(repFila->bn, nleafrelat);
						realvalue = accessFT(repFila->compressIL, posInf);
						realFilas[insertados] = realvalue;
						validosFilas2[insertados] = m;
						validosFilas[insertados++] = indexRel;
					}
				}
				indexRel++;
			}

			if (insertados) {
				while (!esFilas[getPQ(indexRel)] && indexRel <= trepIndex->finq) {
					for (i = 0; i < K2; i++) {

						MREP * repJoin = obtenerRep(trep2,
								indicesRep[getPQ(indexRel)], indiceRel, parte1);
						nleafrelat = getNodeXY(K2, i, j, getPQ(indexRel),
								parte1) - repJoin->bt_len;

						if (isBitSet(repJoin->bn, nleafrelat)) {
							posInf = rank(repJoin->bn, nleafrelat);
							realvalueAux = accessFT(repJoin->compressIL,
									posInf);
							validos[index] = indexRel;
							validos2[index] = i;
							realValues[index++] = realvalueAux;
						}
					}
					indexRel++;
				}

				if (index) {
					for (i = 0; i < K2_2; i++) {
						//Es valido fila
						for (r = 0; r < K2_2; r++) {
							int m;
							for (m = 0; m < insertados; m++) {

								if (bitgetchar(
										&(trep1->words[realFilas[m]
												* trep1->lenWords]),
										getPosRelXY(r,i,K2_2,parte2))) {

									for (k = 0; k < index; k++) {
										for (jj = 0; jj < K2_2; jj++) {


											if (bitgetchar(
													&(trep2->words[realValues[k]
															* trep2->lenWords]),
													getPosRelXY(jj,i,K2_2,parte1))) {
												pares[0].x++;
												if (pares[0].x
														== NUM_ELEMENTOS) {
													fprintf(stderr,
															"numelementos_overflow!\n");
													exit(1);
												}
												pares[pares[0].x].x = getPosAbs(
														K2_2, jj, validos2[k],
														getPQ(validos[k]),
														trep2->fila,
														trep2->tamSubm, parte1);
												
												pares[pares[0].x].y =
														getPosAbs2(
																K2_2,
																i,
																j,
																getPQ(validos[k]),
																indiceRel,
																trep2->tamSubm,
																parte1);
												//getPosAbs(K2_2,i,j,validosFilas[m],indicesRep[validosFilas[m]],trep2->tamSubm,parte2);

												pares[pares[0].x].z =
														getPosAbs(
																K2_2,
																r,
																validosFilas2[m],
																getPQ(validosFilas[m]),
																indicesRep[getPQ(validosFilas[m])],
																trep1->tamSubm,
																parte2);
					
											}
											else{
											}
										}
									}
								}
							}
						}
					}
				}
			} else {
				while (!(indexRel > trepIndex->finq)
						&& !esFilas[getPQ(indexRel)])
					indexRel++;
			}
		}
		trepIndex->iniq = indexRel;
	}
	return pares[0].x;
}
