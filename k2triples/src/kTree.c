/*
 k-ary tree
 creation
 traversal
 Notes:
 1.  both creation and traversal follow BFS
 2.  parent pointer is stored just to show that the tree is created
 correctly, its not used in any way to aid in creation and
 traversal of the tree
 */

#include "kTree.h"
#include "kTreeAux.h"

uint * compactAdjacencyList(TREP * trep, MREP * rep, int x, uint * resultados) {
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	if (rep == NULL || rep->numberOfEdges == 0) {
		return infoTOTAL[0];
	}
	uint factorAdjust = K1K1 / K2K2;
	trep->iniq = -1;
	trep->finq = -1;
	uint nleaf, posInf, realvalue, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, xrelat;
	AddItem2(trep, 0, 0, x);
	queuecont = 1;

	for (i = 0; i < trep->maxLevel1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {
				xrelat = basey[trep->iniq];
				node = xrelat / div_level * K1 + j;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					if (i != trep->maxLevel1 - 1)
						AddItem2(trep, rank(rep->bt, node) * K1K1,
								basex[trep->iniq] + j * div_level,
								basey[trep->iniq] % div_level);
					else {
						AddItem2(
								trep,
								rep->cutBt
										+ (rank(rep->bt, node) * K1K1
												- rep->cutBt) / factorAdjust,
								basex[trep->iniq] + j * div_level,
								basey[trep->iniq] % div_level);

					}
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}

	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);
	for (i = 0; i < trep->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				xrelat = basey[trep->iniq];
				node = xrelat / div_level * K2 + j;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					AddItem2(
							trep,
							rep->cutBt
									+ (rank(rep->bt, node - 1) - cutPreRank)
											* K2K2,
							basex[trep->iniq] + j * div_level,
							basey[trep->iniq] % div_level);
				}
			}
			RemoveItem2(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}

	while (trep->iniq <= trep->finq) {
		nleaf = element[trep->iniq] - rep->bt_len;
		for (j = 0; j < K2; j++) {
			nleafrelat = nleaf + (basey[trep->iniq] / K2_2) * K2 + j;
			if (isBitSet(rep->bn, nleafrelat)) {
				posInf = rank(rep->bn, nleafrelat);
				realvalue = accessFT(rep->compressIL, posInf);
				for (i = 0; i < K2_2; i++) {
					if (bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i+(x%K2_2)*K2_2))) {
						resultados[0]++;
						resultados[resultados[0]] = basex[trep->iniq] + i
								+ K2_2 * j + trep->columna * trep->tamSubm;
					}
				}
			}
		}
		RemoveItem2(trep);
	}
	return resultados;
}

uint * compactInverseList(TREP * trep, MREP * rep, int y, uint * resultados) {
	if (rep == NULL || rep->numberOfEdges == 0)
		return infoTOTAL[0];
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	uint factorAdjust = K1K1 / K2K2;
	trep->iniq = -1;
	trep->finq = -1;
	uint nleaf, posInf, realvalue, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, yrelat;
	AddItem2(trep, 0, y, 0);
	queuecont = 1;
	for (i = 0; i < trep->maxLevel1 - 1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {
				yrelat = basex[trep->iniq];
				node = K1 * j + yrelat / div_level;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					AddItem2(trep, rank(rep->bt, node) * K1K1,
							basex[trep->iniq] % div_level,
							basey[trep->iniq] + j * div_level);
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}
	for (i = trep->maxLevel1 - 1; i < trep->maxLevel1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {
				yrelat = basex[trep->iniq];
				node = K1 * j + yrelat / div_level;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					AddItem2(
							trep,
							rep->cutBt
									+ (rank(rep->bt, node) * K1K1 - rep->cutBt)
											/ factorAdjust,
							basex[trep->iniq] % div_level,
							basey[trep->iniq] + j * div_level);
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}

	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);
	for (i = 0; i < trep->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				yrelat = basex[trep->iniq];
				node = K2 * j + yrelat / div_level;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					AddItem2(
							trep,
							rep->cutBt
									+ (rank(rep->bt, node - 1) - cutPreRank)
											* K2K2,
							basex[trep->iniq] % div_level,
							basey[trep->iniq] + j * div_level);
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}

	while (trep->iniq <= trep->finq) {
		nleaf = element[trep->iniq] - rep->bt_len;
		for (j = 0; j < K2; j++) {
			nleafrelat = nleaf + (basex[trep->iniq] / K2_2) + K2 * j;
			if (isBitSet(rep->bn, nleafrelat)) {
				posInf = rank(rep->bn, nleafrelat);
				realvalue = accessFT(rep->compressIL, posInf);
				for (i = 0; i < K2_2; i++) {
					if (bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i*K2_2+(y%K2_2)))) {
						resultados[0]++;
						resultados[resultados[0]] = basey[trep->iniq] + i
								+ K2_2 * j + trep->fila * trep->tamSubm;
					}
				}
			}
		}
		RemoveItem2(trep);
	}
	return resultados;
}

int compactTreeAdjacencyList(TREP * trep, int x, uint * resultados) {
	resultados[0] = 0;
	MREP * rep;
	uint  i;
	uint xrelatIn = x / trep->tamSubm; //*trep->part;
	uint * listady;
	x = x % trep->tamSubm;
	for (i = 0; i < trep->part; i++) {
		rep = trep->submatrices[xrelatIn][i];
		trep->columna = i;
		listady = compactAdjacencyList(trep, rep, x,resultados);
	}
	return resultados[0];

}



int compactTreeInverseList(TREP * trep, int y, uint * resultados) {
	resultados[0] = 0;
	MREP * rep;
	uint  i;
	uint yrelatIn = y / trep->tamSubm;
	uint * listady;

	y = y % trep->tamSubm;
	for (i = 0; i < trep->part; i++) {
		rep = trep->submatrices[i][yrelatIn];
		trep->fila = i;
		if (rep!=NULL && rep->numberOfEdges>0){
			listady = compactInverseList(trep, rep, y, resultados);
		}
	}
	return resultados[0];
}


uint compactCheckLinkQuery(TREP * trep, MREP * rep, uint p, uint q) {
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;

	if (rep == NULL || rep->numberOfEdges == 0)
		return 0;

	uint factorAdjust = K1K1 / K2K2;

	uint nleaf, posInf, realvalue, nleafrelat;
	uint prelat, qrelat;
	int i, node, div_level;

	prelat = p;
	qrelat = q;

	node = 0;

	for (i = 0; i < trep->maxLevel1 - 1; i++) {

		div_level = trep->div_level_table1[i];

		node += prelat / div_level * K1 + qrelat / div_level;

		prelat = prelat % div_level;
		qrelat = qrelat % div_level;

		if (isBitSet(rep->bt, node) == 0)
			return 0;

		node = rank(rep->bt, node) * K1K1;

	}

	for (i = trep->maxLevel1 - 1; i < trep->maxLevel1; i++) {

		div_level = trep->div_level_table1[i];

		node += prelat / div_level * K1 + qrelat / div_level;
		prelat = prelat % div_level;
		qrelat = qrelat % div_level;

		if (isBitSet(rep->bt, node) == 0)
			return 0;

		node = rep->cutBt
				+ (rank(rep->bt, node) * K1K1 - rep->cutBt) / factorAdjust;
	}

	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);

	for (i = 0; i < trep->maxLevel2 - 1; i++) {

		div_level = trep->div_level_table2[i];

		node += prelat / div_level * K2 + qrelat / div_level;

		prelat = prelat % div_level;
		qrelat = qrelat % div_level;

		if (isBitSet(rep->bt, node) == 0)
			return 0;

		node = rep->cutBt + (rank(rep->bt, node - 1) - cutPreRank) * K2K2;

	}

	nleaf = node - rep->bt_len;

	nleafrelat = nleaf + (prelat / K2_2) * K2 + qrelat / K2_2;

	if (isBitSet(rep->bn, nleafrelat)) {

		posInf = rank(rep->bn, nleafrelat);

		realvalue = accessFT(rep->compressIL, posInf);

		if (bitgetchar(&(trep->words[realvalue*trep->lenWords]),(qrelat%K2_2+(prelat%K2_2)*K2_2)))
			return 1;
	}

	return 0;

}

uint compactTreeCheckLinkQuery(TREP * trep, uint p, uint q) {
	MREP * rep;
	
	rep = trep->submatrices[p / trep->tamSubm][q / trep->tamSubm];
	if (rep!=NULL && rep->numberOfEdges>0){
		return compactCheckLinkQuery(trep, rep, p % trep->tamSubm,
			q % trep->tamSubm);
	}
	else 
		return 0;
}

void compactAll(TREP * trep, MREP * rep,SET * pares) {
	int i, ii, j;
	//FIXME:ELIMINAR!!!!
	uint K1K1 = 16, K2K2 = 4, AJUSTEK1K2 = 4;
	uint k, divlevel;
	uint realvalue, leaf, posInf;
	trep->iniq = -1;
	trep->finq = -1;
	uint nleaf, dp, dq;

	int queuecont, conttmp, node, node2;

	if (rep->numberOfEdges == 0)
		return;
	AddItem2(trep, 0, 0, 0);

	queuecont = 1;
	for (ii = 0; ii < trep->maxLevel1; ii++) {
		conttmp = 0;
		divlevel = trep->div_level_table1[ii];
		for (k = 0; k < queuecont; k++) {
			node = element[getPQ(trep->iniq)];
			dp = basex[getPQ(trep->iniq)];
			dq = basey[getPQ(trep->iniq)];
			for (i = 0; i < K1; i++) {
				for (j = 0; j < K1; j++) {
					node2 = node + K1 * i + j;
					if ((rep->bt_len > 0) && isBitSet(rep->bt, node2)) {
						conttmp++;
						if (ii != trep->maxLevel1 - 1) {
							AddItem2(trep, rank(rep->bt, node2) * K1K1,
									dp + divlevel * i, dq + divlevel * j);
						} else {
							AddItem2(
									trep,
									rep->cutBt
											+ (rank(rep->bt, node2) * K1K1
													- rep->cutBt) / AJUSTEK1K2,
									dp + divlevel * i, dq + divlevel * j);
						}

					}

				}
			}
			RemoveItem3(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}
	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);

	for (ii = 0; ii < trep->maxLevel2 - 1; ii++) {
		conttmp = 0;
		divlevel = trep->div_level_table2[ii];
		for (k = 0; k < queuecont; k++) {
			node = element[getPQ(trep->iniq)];
			dp = basex[getPQ(trep->iniq)];
			dq = basey[getPQ(trep->iniq)];
			for (i = 0; i < K2; i++) {
				for (j = 0; j < K2; j++) {
					node2 = node + K2 * i + j;
					if (rep->bt_len > 0 && isBitSet(rep->bt, node2)) {
						conttmp++;
						AddItem2(
								trep,
								rep->cutBt
										+ (rank(rep->bt, node2 - 1) - cutPreRank)
												* K2K2, dp + divlevel * i,
								dq + divlevel * j);
					}
				}
			}
			RemoveItem3(trep);
		}
		queuecont = conttmp;
		checkLimites(trep);
	}
	while (trep->iniq <= trep->finq) {
		nleaf = element[getPQ(trep->iniq)] - rep->bt_len;
		dp = basex[getPQ(trep->iniq)];
		dq = basey[getPQ(trep->iniq)];
		int jj;
		for (i = 0; i < K2; i++) {
			for (j = 0; j < K2; j++) {
				leaf = nleaf + i * K2 + j;
				if (isBitSet(rep->bn, leaf)) {
					posInf = rank(rep->bn, leaf);
					realvalue = accessFT(rep->compressIL, posInf);
					for (ii = 0; ii < K2_2; ii++)
						for (jj = 0; jj < K2_2; jj++) {
							if (bitgetchar(&(trep->words[realvalue*trep->lenWords]),(jj%K2_2+(ii%K2_2)*K2_2))) {
								pares[0].x++;
								if (pares[0].x==NUM_ELEMENTOS) {fprintf(stderr,"overflow num elementos!\n");exit(1);}
								pares[pares[0].x].x = dp + i * K2_2 + ii
										+ trep->fila * trep->tamSubm;
								pares[pares[0].x].y = dq + j * K2_2 + jj
										+ trep->columna * trep->tamSubm;
							}
						}
				}
			}
		}
		RemoveItem3(trep);
	}
	return;
}

void compactAllOrdenadoCol(TREP * trep, MREP * rep) {
	int i, ii, j;
	uint K1K1 = 16, K2K2 = 4, AJUSTEK1K2 = 4;
	uint  k, divlevel;
	uint realvalue, leaf, posInf;
	trep->iniq = -1;
	trep->finq = -1;
	uint nleaf, dp, dq;
	int indexRel;
	int kActual;
	int queuecont, conttmp, node, node2;

	if (rep->numberOfEdges == 0)
		return;
	int columna;
	AddItem2(trep, 0, 0, 0);

	queuecont = 1;
	for (ii = 0; ii < trep->maxLevel1; ii++) {
		conttmp = 0;
		divlevel = trep->div_level_table1[ii];
		kActual = 0;
		indexRel = trep->iniq;

		while (kActual < queuecont) {
			columna = basey[indexRel + kActual];
			for (j = 0; j < K1; j++) {
				k = kActual;
				while (k < queuecont && columna == basey[indexRel + k]) {
					node = element[indexRel + k];
					dp = basex[indexRel + k];
					dq = basey[indexRel + k];
					for (i = 0; i < K1; i++) {
						node2 = node + K1 * i + j;
						if ((rep->bt_len > 0) && isBitSet(rep->bt, node2)) {
							conttmp++;
							if (ii != trep->maxLevel1 - 1) {
								AddItem2(trep, rank(rep->bt, node2) * K1K1,
										dp + divlevel * i, dq + divlevel * j);
							} else {
								AddItem2(
										trep,
										rep->cutBt
												+ (rank(rep->bt, node2) * K1K1
														- rep->cutBt)
														/ AJUSTEK1K2,
										dp + divlevel * i, dq + divlevel * j);
							}
						}
					}
					k++;
				}

			}
			kActual = k;
		}
		trep->iniq += queuecont;
		queuecont = conttmp;
		checkLimites(trep);
	}

	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);

	for (ii = 0; ii < trep->maxLevel2 - 1; ii++) {
		conttmp = 0;
		divlevel = trep->div_level_table2[ii];
		kActual = 0;
		indexRel = trep->iniq;

		while (kActual < queuecont) {
			columna = basey[indexRel + kActual];

			for (j = 0; j < K2; j++) {
				k = kActual;
				while (k < queuecont && columna == basey[indexRel + k]) {

					node = element[indexRel + k];
					dp = basex[indexRel + k];
					dq = basey[indexRel + k];
					for (i = 0; i < K2; i++) {
						node2 = node + K2 * i + j;
						if (rep->bt_len > 0 && isBitSet(rep->bt, node2)) {
							conttmp++;
							AddItem2(
									trep,
									rep->cutBt
											+ (rank(rep->bt, node2 - 1)
													- cutPreRank) * K2K2,
									dp + divlevel * i, dq + divlevel * j);
						}
					}
					k++;
				}
			}
			kActual = k;
		}
		trep->iniq += queuecont;
		queuecont = conttmp;
		checkLimites(trep);
	}

	while (trep->iniq <= trep->finq) {
		int jj;
		for (j = 0; j < K2; j++) {
			for (jj = 0; jj < K2_2; jj++) {
				indexRel = trep->iniq;
				int columna = basey[indexRel];

				while (indexRel <= trep->finq && basey[indexRel] == columna) {
					nleaf = element[indexRel] - rep->bt_len;
					dp = basex[indexRel];
					dq = basey[indexRel];

					for (i = 0; i < K2; i++) {
						leaf = nleaf + i * K2 + j;

						if (isBitSet(rep->bn, leaf)) {
							posInf = rank(rep->bn, leaf);
							realvalue = accessFT(rep->compressIL, posInf);
							for (ii = 0; ii < K2_2; ii++)
								if (bitgetchar(&(trep->words[realvalue*trep->lenWords]), (jj%K2_2+(ii%K2_2)*K2_2))) {
									infoTOTAL[0][0]++;
									infoTOTAL[0][infoTOTAL[0][0]] = dp
											+ i * K2_2 + ii
											+ trep->fila * trep->tamSubm;
									infoTOTAL[1][infoTOTAL[0][0]] = dq
											+ j * K2_2 + jj
											+ trep->columna * trep->tamSubm;
								}
						}
					}

					indexRel++;
				}
			}
		}
		if (trep->iniq == indexRel)
			break;
		trep->iniq = indexRel;
	}
	return;
}

int comparar(const void *p1, const void *p2) {
	SET * par1 = (SET *) p1;
	SET * par2 = (SET *) p2;
	if (par1->y < par2->y)
		return -1;
	if (par1->y > par2->y)
		return 1;
	return 0;
}

int compararFilas(const void *p1, const void *p2) {
	SET * par1 = (SET *) p1;
	SET * par2 = (SET *) p2;
	if (par1->x < par2->x)
		return -1;
	if (par1->x > par2->x)
		return 1;
	return 0;
}

int compactTreeRangeQuery(TREP * trep, int ordenar,SET * pares) {

	pares[0].x=0;

	MREP * rep;
	uint j, i;
	for (i = 0; i < trep->part; i++) {
		for (j = 0; j < trep->part; j++) {

			rep = trep->submatrices[i][j];
			trep->fila = i;
			trep->columna = j;
			if (rep != NULL) {
				compactAll(trep,rep,pares);
			}
		}
	}
	if (ordenar==POROBJETO){
		qsort(&(pares[1]), pares[0].x, sizeof(SET), comparar);
	}
	if (ordenar==PORSUJETO){
		qsort(&(pares[1]), pares[0].x, sizeof(SET), compararFilas);
	}
	return pares[0].x;
}




uint * compactInverseListMerge(TREP * trep, MREP * rep, int y,
		Result2 ** vectorResultados, int indicePredicado, int * tamanioVector,
		int predicado) {

	if (rep == NULL || rep->numberOfEdges == 0)
		return infoTOTAL[0];
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;
	uint factorAdjust = K1K1 / K2K2;
	trep->iniq = -1;
	trep->finq = -1;
	uint nleaf, posInf, realvalue, nleafrelat;
	//	uint summemoria =0;
	int i, k, j, queuecont, conttmp, node, div_level, yrelat;
	AddItem2(trep, 0, y, 0);
	queuecont = 1;
	for (i = 0; i < trep->maxLevel1 - 1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {
				yrelat = basex[trep->iniq];
				node = K1 * j + yrelat / div_level;
				node += element[trep->iniq];

				if (isBitSet(rep->bt, node)) {
					conttmp++;
					AddItem2(trep, rank(rep->bt, node) * K1K1,
							basex[trep->iniq] % div_level,
							basey[trep->iniq] + j * div_level);
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	//fprintf(stderr,"cutBt: %d, factorAdjust: %d\n",rep->cutBt, factorAdjust);
	for (i = trep->maxLevel1 - 1; i < trep->maxLevel1; i++) {
		conttmp = 0;
		//		fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table1[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K1, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {
				yrelat = basex[trep->iniq];
				node = K1 * j + yrelat / div_level;
				node += element[trep->iniq];
				//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);

				if (isBitSet(rep->bt, node)) {
					//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
					conttmp++;
					//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]+xrelat/div_level*div_level);
					//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rep->cutBt + (rank(rep->bt,node)*K1*K1-rep->cutBt)/factorAdjust);
					AddItem2(
							trep,
							rep->cutBt
									+ (rank(rep->bt, node) * K1K1 - rep->cutBt)
											/ factorAdjust,
							basex[trep->iniq] % div_level,
							basey[trep->iniq] + j * div_level);
					//						summemoria+=sizeof(struct QUEUE2);
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);
	//fprintf(stderr,"cutPreRank %d lastBt_1: %d\n",cutPreRank,rep->lastBt1_len);
	for (i = 0; i < trep->maxLevel2 - 1; i++) {
		conttmp = 0;
		//	fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table2[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K2, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				yrelat = basex[trep->iniq];
				node = K2 * j + yrelat / div_level;
				node += element[trep->iniq];
				//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);

				if (isBitSet(rep->bt, node)) {
					//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
					conttmp++;
					//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basex[trep->iniq]+xrelat/div_level*div_level);
					//fprintf(stderr,"cutBt: %d, rank: %d, cutPrerank: %d\n",rep->cutBt ,rank(rep->bt,node),cutPreRank);
					//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rep->cutBt + (rank(rep->bt,node)-cutPreRank)*K2*K2);
					AddItem2(
							trep,
							rep->cutBt
									+ (rank(rep->bt, node - 1) - cutPreRank)
											* K2K2,
							basex[trep->iniq] % div_level,
							basey[trep->iniq] + j * div_level);
					//						summemoria+=sizeof(struct QUEUE2);
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	while (trep->iniq <= trep->finq) {
		nleaf = element[trep->iniq] - rep->bt_len;
		//fprintf(stderr,"\nMirando la hoja %d con basey %d\n",nleaf,trep->basey[trep->iniq]);
		for (j = 0; j < K2; j++) {
			nleafrelat = nleaf + (basex[trep->iniq] / K2_2) + K2 * j;
			//fprintf(stderr,"Relativo %d:, isbitset: %d\n",nleafrelat,isBitSet(rep->bn,nleafrelat));
			if (isBitSet(rep->bn, nleafrelat)) {
				//fprintf(stderr,"Esta el bit a 1! posinf sera: %d\n",(rank(rep->bn,nleafrelat)-1)*K2*K2);
				//posInf = (rank(rep->bn,nleafrelat)-1)*K2K2;

				posInf = rank(rep->bn, nleafrelat);
				realvalue = accessFT(rep->compressIL, posInf);

				for (i = 0; i < K2_2; i++) {
					if (bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i*K2_2+(y%K2_2)))) {
						if (vectorResultados[indicePredicado][0].value + 1
								== tamanioVector[0]) {
							*tamanioVector = *tamanioVector * 2;
							vectorResultados[indicePredicado] =
									(Result2 *) realloc(
											vectorResultados[indicePredicado],
											(*tamanioVector) * sizeof(Result2));
						}
						vectorResultados[indicePredicado][0].value++;
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].value =
								basey[trep->iniq] + i + K2_2 * j
										+ trep->fila * trep->tamSubm;
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].sources =
								mymalloc(sizeof(int) * 2);
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].sources[0] =
								1;
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].sources[1] =
								predicado;

					}
				}
			}
		}
		RemoveItem2(trep);
	}
	return infoTOTAL[1];
}

uint compactTreeInverseListMerge(TREP * trep, int y, Result2 ** vectorResultados,
		int indicePredicado, int predicado) {
	infoTOTAL[0][0] = 0;
	MREP * rep;
	uint i;
	uint yrelatIn = y / trep->tamSubm;
	uint * listady;
	int tamanioVector = TAMINICIAL;
	y = y % trep->tamSubm;
	for (i = 0; i < trep->part; i++) {
		rep = trep->submatrices[i][yrelatIn];
		trep->fila = i;
		if (trep->numberOfEdges > 0 && rep!=NULL && rep->numberOfEdges!=0)
			listady = compactInverseListMerge(trep, rep, y, vectorResultados,
					indicePredicado, &tamanioVector, predicado);
	}
	return vectorResultados[indicePredicado][0].value;
}

uint * compactAdjacencyListMerge(TREP * trep, MREP * rep, int x,
		Result2 ** vectorResultados, int indicePredicado, int * tamanioVector,
		int predicado) {
	int K1K1 = K1 * K1;
	int K2K2 = K2 * K2;

	if (rep == NULL || rep->numberOfEdges == 0) {
		return infoTOTAL[0];
	}
	uint factorAdjust = K1K1 / K2K2;
	trep->iniq = -1;
	trep->finq = -1;
	uint nleaf, posInf, realvalue, nleafrelat;
	int i, k, j, queuecont, conttmp, node, div_level, xrelat;
	AddItem2(trep, 0, 0, x);
	queuecont = 1;

	for (i = 0; i < trep->maxLevel1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table1[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K1; j++) {
				xrelat = basey[trep->iniq];
				node = xrelat / div_level * K1 + j;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					if (i != trep->maxLevel1 - 1)
						AddItem2(trep, rank(rep->bt, node) * K1K1,
								basex[trep->iniq] + j * div_level,
								basey[trep->iniq] % div_level);
					else {
						AddItem2(
								trep,
								rep->cutBt
										+ (rank(rep->bt, node) * K1K1
												- rep->cutBt) / factorAdjust,
								basex[trep->iniq] + j * div_level,
								basey[trep->iniq] % div_level);

					}
				}
			}

			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	uint cutPreRank = rank(rep->bt, rep->lastBt1_len - 1);
	for (i = 0; i < trep->maxLevel2 - 1; i++) {
		conttmp = 0;
		div_level = trep->div_level_table2[i];
		for (k = 0; k < queuecont; k++) {
			for (j = 0; j < K2; j++) {
				xrelat = basey[trep->iniq];
				node = xrelat / div_level * K2 + j;
				node += element[trep->iniq];
				if (isBitSet(rep->bt, node)) {
					conttmp++;
					AddItem2(
							trep,
							rep->cutBt
									+ (rank(rep->bt, node - 1) - cutPreRank)
											* K2K2,
							basex[trep->iniq] + j * div_level,
							basey[trep->iniq] % div_level);
				}
			}
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	while (trep->iniq <= trep->finq) {
		nleaf = element[trep->iniq] - rep->bt_len;
		for (j = 0; j < K2; j++) {
			nleafrelat = nleaf + (basey[trep->iniq] / K2_2) * K2 + j;
			if (isBitSet(rep->bn, nleafrelat)) {
				posInf = rank(rep->bn, nleafrelat);
				realvalue = accessFT(rep->compressIL, posInf);
				for (i = 0; i < K2_2; i++) {
					if (bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i+(x%K2_2)*K2_2))) {
						if (vectorResultados[indicePredicado][0].value + 1
								== tamanioVector[0]) {
							*tamanioVector = *tamanioVector * 2;
							vectorResultados[indicePredicado] =
									(Result2 *) realloc(
											vectorResultados[indicePredicado],
											(*tamanioVector) * sizeof(Result2));
						}

						vectorResultados[indicePredicado][0].value++;
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].value =
								basex[trep->iniq] + i + K2_2 * j
										+ trep->columna * trep->tamSubm;

						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].sources =
								mymalloc(sizeof(int) * 2);
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].sources[0] =
								1;
						vectorResultados[indicePredicado][vectorResultados[indicePredicado][0].value].sources[1] =
								predicado;
					/*	infoTOTAL[0][0]++;
						infoTOTAL[0][infoTOTAL[0][0]] = basex[trep->iniq] + i
								+ K2_2 * j + trep->columna * trep->tamSubm;*/
					}
				}
			}
		}
		RemoveItem2(trep);
	}
	return infoTOTAL[0];
}

uint compactTreeAdjacencyListMerge(TREP * trep, int x,
		Result2 ** vectorResultados, int indicePredicado, int predicado) {
	infoTOTAL[0][0] = 0;
	MREP * rep;
	uint i;
	uint xrelatIn = x / trep->tamSubm; //*trep->part;
	uint * listady;
	x = x % trep->tamSubm;
	int tamanioVector = TAMINICIAL;
	for (i = 0; i < trep->part; i++) {
		rep = trep->submatrices[xrelatIn][i];
		trep->columna = i;
		listady = compactAdjacencyListMerge(trep, rep, x, vectorResultados,
				indicePredicado, &tamanioVector, predicado);
	}
	return infoTOTAL[0][0];

}

