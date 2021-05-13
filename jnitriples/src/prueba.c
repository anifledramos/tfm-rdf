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
															"overflow!\n");
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
														trep2->columna,
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
