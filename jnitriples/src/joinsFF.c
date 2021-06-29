#include "joinsFF.h"

//JOIN 1 - IMPLEMENTACION BASICA
int sppo(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2, int parte1,
		int parte2) {
	int rango = 0;
	uint * x = infoTOTAL[1];
	uint * y = infoTOTAL[0];
	int i = 1, j = 1;
	int resultado = 0;

	if (DEBUG) {
		printTripleta(s1 + 1, p1, -1, parte1);
		printTripleta(o2 + 1, p2, -1, parte2);
		fprintf(stdout, "\n");
	}

	if (parte1 != parte2)
		rango = 1;
	compactNeighbour(treps[p1], s1, x, parte1);
	compactNeighbour(treps[p2], o2, y, parte2);
//	fprintf(stderr,"%d\t%d\t%d\t%d\n",x[0],y[0],treps[p1]->numberOfEdges,treps[p2]->numberOfEdges);
//	return 1;
	while (i <= x[0] && j <= y[0]) {
		if (rango && (x[i] > numeroSO - 1 || y[j] > numeroSO - 1)) {
			//fprintf(stderr,"filtrados %d/%d y %d/%d\n",x[0]+1-i,x[0],y[0]+1-j,y[0]);
			break;
		}
		if (x[i] == y[j]) {
			resultado++;

			if (DEBUG) {
				printTripleta(s1 + 1, p1, x[i] + 1, parte1);
				printTripleta(o2 + 1, p2, y[j] + 1, parte2);
				fprintf(stdout, "\n");
			}

			i++;
			j++;
		} else if (x[i] > y[j]) {
			j++;
		} else
			i++;
	}
	return resultado;
}

//JOIN 1 - EN PARALELO
int sppoParalelo(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2,
		int parte1, int parte2) {
	int i;
	SET * pares = paresBuff1;
	int resultados = 0;
	int rango = 0;
	if (DEBUG) {
		printTripleta(s1 + 1, p1, -1, parte1);
		printTripleta(o2 + 1, p2, -1, parte2);
		fprintf(stdout, "\n");
	}
	if (parte1 != parte2)
		rango = 1;
	joinAsim1(treps, o2, s1, p2, p1, nPreds, pares, parte2, parte1);

	for (i = 1; i <= infoTOTAL[0][0]; i++) {
		if (rango && infoTOTAL[0][i] > numeroSO - 1) {
			break;
		}
		if (DEBUG) {
			printTripleta(s1 + 1, p1, infoTOTAL[0][i] + 1, parte1);
			printTripleta(o2 + 1, p2, infoTOTAL[0][i] + 1, parte2);
			fprintf(stdout, "\n");
		}
		resultados++;
	}
	return resultados;
}

//JOIN 1 - PRIMERO IZQUIERDA
int sppoIzquierda(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2,
		int parte1, int parte2) {
	uint * x = infoTOTAL[0];
	int i = 1;
	int resultado = 0;
	int rango = 0;
	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(s1 + 1, p1, -1, parte1);
			printTripleta(o2 + 1, p2, -1, parte2);
			fprintf(stdout, "\n");
		}
	}
	if (parte1 != parte2)
		rango = 1;
	compactNeighbour(treps[p1], s1, x, parte1);

	for (i = 1; i <= x[0]; i++) {

		if (rango && x[i] > numeroSO - 1) {
			break;
		}

		if (compactLink(treps[p2], x[i], o2, parte2)) {

			if (DEBUG) {
				printCompleto(s1 + 1, p1, x[i] + 1, parte1,o2 + 1, p2, x[i] + 1, parte2);
			}

			resultado++;
		}

	}

	return resultado;
}

//JOIN 1 - PRIMERO DERECHA
int sppoDerecha(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2,
		int parte1, int parte2) {
	uint * x = infoTOTAL[0];
	int i = 1;
	int resultado = 0;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		printTripleta(s1 + 1, p1, -1, parte1);
		printTripleta(o2 + 1, p2, -1, parte2);
		fprintf(stdout, "\n");
	}

	compactNeighbour(treps[p2], o2, x, parte2);

	for (i = 1; i <= x[0]; i++) {
		if (rango && x[i] > numeroSO - 1) {
			//fprintf(stderr, "filtrados a partir de %d\n", x[i]);
			break;
		}

		if (compactLink(treps[p1], x[i], s1, parte1)) {
			if (DEBUG) {
				printTripleta(s1 + 1, p1, x[i] + 1, parte1);
				printTripleta(o2 + 1, p2, x[i] + 1, parte2);
				fprintf(stdout, "\n");
			}
			resultado++;
		}
	}
	return resultado;
}

//JOIN 2 
int sp_o(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int s1, int p1,
		int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	uint * x = infoTOTAL[0];
	int i, j;
	int resultados = 0;

	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(s1 + 1, p1, -1, parte1);
			printTripleta(o2 + 1, -1, -1, parte2);
			fprintf(stdout, "\n");
		}
	}
	if (condac) {
		commonObtenerPredicados(sp, op, o2, parte2);
	}

	compactNeighbour(treps[p1], s1, x, parte1);

	for (i = 1; i <= x[0]; i++) {
		
		if (rango && x[i] > numeroSO - 1) {
			break;
		}
		if (condac) {
			for (j = 0; j < resDAC.numeroPredicados; j++) {

				if (compactLink(treps[resDAC.predicados[j]], x[i], o2, parte2)) {
					if (DEBUG) {
						printTripleta(s1 + 1, p1, x[i] + 1, parte1);
						printTripleta(o2 + 1, resDAC.predicados[j], x[i] + 1,
								parte2);
						fprintf(stdout, "\n");
					}
					resultados++;
				}
			}
		} else {
			for (j = 1; j <= nPreds; j++) {
					
				if (compactLink(treps[j], x[i], o2, parte2)) {
					if (DEBUG) {
						printTripleta(s1 + 1, p1, x[i] + 1, parte1);
						printTripleta(o2 + 1, j, x[i] + 1, parte2);
						fprintf(stdout, "\n");
					}
					resultados++;
				}
			}
		}
	}
	return resultados;
}

int sp_oInterseccion(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op,
		int s1, int p1, int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	uint * x = infoTOTAL[0];
	uint * y = infoTOTAL[1];
	int k;
	int resultados = 0;

	if (DEBUG) {
		printTripleta(s1 + 1, p1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	if (condac) {
		commonObtenerPredicados(sp, op, o2, parte2);
	}

	compactNeighbour(treps[p1], s1, x, parte1);

	if (condac) {
		for (k = 0; k < resDAC.numeroPredicados; k++) {
			compactNeighbour(treps[resDAC.predicados[k]], o2, y, parte2);
			int i = 1, j = 1;
			while (i <= x[0] && j <= y[0]) {
				if (rango && ((x[i] > numeroSO - 1) || (y[j] > numeroSO - 1))) {
					//fprintf(stderr, "filtrados a partir de %d\n", x[i]);
					break;
				}
				if (x[i] == y[j]) {
					if (DEBUG) {
						printTripleta(s1 + 1, p1, x[i] + 1, parte1);
						printTripleta(o2 + 1, resDAC.predicados[k], x[i] + 1,
								parte2);
						fprintf(stdout, "\n");
					}
					i++;
					j++;
					resultados++;
				} else if (x[i] > y[j]) {
					j++;
				} else
					i++;
			}
		}
	} else {
		for (k = 1; k <= nPreds; k++) {
			compactNeighbour(treps[k], o2, y, parte2);
			int i = 1, j = 1;
			while (i <= x[0] && j <= y[0]) {
				if (rango && ((x[i] > numeroSO - 1) || (y[j] > numeroSO - 1))) {
					//fprintf(stderr, "filtrados a partir de %d\n", x[i]);
					break;
				}
				if (x[i] == y[j]) {

					if (DEBUG) {
						printTripleta(s1 + 1, p1, x[i] + 1, parte1);
						printTripleta(o2 + 1, k, x[i] + 1, parte2);
						fprintf(stdout, "\n");
					}
					i++;
					j++;
					resultados++;
				} else if (x[i] > y[j]) {
					j++;
				} else
					i++;
			}
		}
	}
	return resultados;
}


int sp_oParalelo(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int s1,
		int p1, int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int i;
	int resultados = 0;

	if (DEBUG) {
		printTripleta(s1 + 1, p1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	SET * pares = paresBuff1;

	joinAsim2(treps, sp, op, o2, s1, p1, nPreds, condac, pares, parte2, parte1);
	for (i = 1; i <= infoTOTAL[0][0]; i++) {
		if (rango && (infoTOTAL[0][i] > numeroSO - 1 || infoTOTAL[1][i]
				> numeroSO - 1)) {

		} else {
			if (DEBUG) {
				printCompleto(s1 + 1, p1, infoTOTAL[1][i] + 1, parte1, o2 + 1, infoTOTAL[0][i], infoTOTAL[1][i] + 1,parte2);
			}
			resultados++;
		}
	}
	return resultados;
}

//JOIN 3

int s__o(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int s1, int o2,
		int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	uint * x = infoTOTAL[0];
	uint * y = infoTOTAL[1];

	int i = 1, j = 1, predicado;
	int resultados = 0;

	if (DEBUG) {
		printTripleta(s1 + 1, -1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	if (!condac) {
		for (predicado = 1; predicado <= nPreds; predicado++) {
			compactNeighbour(treps[predicado], s1, x, parte1);
			if (x[0] == 0)
				continue;
			int predicado2;
			for (predicado2 = 1; predicado2 <= nPreds; predicado2++) {
				compactNeighbour(treps[predicado2], o2, y, parte2);
				i = 1;
				j = 1;
				while (i <= x[0] && j <= y[0]) {

					if (rango && (x[i] > numeroSO - 1 || y[j] > numeroSO - 1)) {
						break;
					}

					if (x[i] == y[j]) {
						if (DEBUG) {
							printTripleta(s1 + 1, predicado, x[i] + 1, parte1);
							printTripleta(o2 + 1, predicado2, y[j] + 1, parte2);
							fprintf(stdout, "\n");
						}
						i++;
						j++;
						resultados++;
					} else if (x[i] > y[j]) {
						j++;
					} else
						i++;
				}
			}

		}
	} else {
		commonObtenerPredicados(sp, op, s1, parte1);
		commonObtenerPredicados2(sp, op, o2, parte2);
		for (predicado = 0; predicado < resDAC.numeroPredicados; predicado++) {
			compactNeighbour(treps[resDAC.predicados[predicado]], s1, x, parte1);
			if (x[0] == 0)
				continue;
			int predicado2;
			for (predicado2 = 0; predicado2 < resDAC2.numeroPredicados; predicado2++) {
				compactNeighbour(treps[resDAC2.predicados[predicado2]], o2, y,
						parte2);
				i = 1;
				j = 1;
				while (i <= x[0] && j <= y[0]) {
	
					if (rango && (x[i] > numeroSO - 1 || y[j] > numeroSO - 1)) {
						break;
					}

					if (x[i] == y[j]) {
						if (DEBUG) {
							printTripleta(s1 + 1, resDAC.predicados[predicado],
									x[i] + 1, parte1);
							printTripleta(o2 + 1,
									resDAC2.predicados[predicado2], y[j] + 1,
									parte2);
							fprintf(stdout, "\n");
						}

						resultados++;
						
						i++;
						j++;
					} else if (x[i] > y[j]) {
						j++;
					} else
						i++;
				}
			}
		}
	}

	return resultados;
}

int s__oParalelo(TREP ** treps, INDICEDAC sp, INDICEDAC op, int nPreds, int s1,
		int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int resultados = 0;
	int i;
	SET * pares = paresBuff1;

	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(s1 + 1, -1, -1, parte1);
			printTripleta(o2 + 1, -1, -1, parte2);
			fprintf(stdout, "\n");
		}	
	}
	joinAsim3(treps, sp, op, o2, s1, nPreds, condac, pares, parte2, parte1);

	for (i = 1; i <= pares[0].x; i++) {
		if (rango && (pares[i].y > numeroSO - 1)) {

		} else {
			resultados++;
			if (DEBUG) {
				printCompleto(s1 + 1, pares[i].x, pares[i].y + 1, parte1,o2 + 1, pares[i].z, pares[i].y + 1, parte2);
			}
		}
	}
	return resultados;
}

//En cadena
int s__oMerge(TREP ** treps, int numPreds, INDICEDAC sp, INDICEDAC op,
		int sujeto, int objeto2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int resultados = 0;
	int i = 1, predicado;

	if (DEBUG) {
		printTripleta(sujeto + 1, -1, -1, parte1);
		printTripleta(objeto2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	int indiceVector = 0;
	vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
	vectores[indiceVector][0].value = 0;

	if (!condac) {
		for (predicado = 1; predicado <= numPreds; predicado++) {
			compactNeighbourMerge(treps[predicado], objeto2, vectores,
					indiceVector, predicado, parte2);
			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}
		}
	} else {
		commonObtenerPredicados(sp, op, objeto2, parte2);
		for (predicado = 0; predicado < resDAC.numeroPredicados; predicado++) {
			compactNeighbourMerge(treps[resDAC.predicados[predicado]], objeto2,
					vectores, indiceVector, resDAC.predicados[predicado],
					parte2);
			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}
		}
	}

	if (indiceVector > 1)
		mergeMiguel(vectores, indiceVector);

	int predicado2;
	int k;

	if (!condac) {
		for (predicado2 = 1; predicado2 <= numPreds; predicado2++) {
			for (i = 1; i <= vectores[0][0].value; i++) {
				if (compactLink(treps[predicado2], vectores[0][i].value,
						sujeto, parte1)) {
					for (k = 1; k <= vectores[0][i].sources[0]; k++) {

						if (rango && vectores[0][i].value > numeroSO - 1) {

						} else {
							if (DEBUG) {
								printTripleta(sujeto + 1, predicado2,
										vectores[0][i].value + 1, parte1);
								printTripleta(objeto2 + 1,
										vectores[0][i].sources[k],
										vectores[0][i].value + 1, parte2);
								fprintf(stdout, "\n");
							}
							resultados++;
						}
					}
				}
			}
		}
	} else {
		commonObtenerPredicados(sp, op, sujeto, parte1);
		for (predicado2 = 0; predicado2 < resDAC.numeroPredicados; predicado2++) {
			for (i = 1; i <= vectores[0][0].value; i++) {
				if (compactLink(treps[resDAC.predicados[predicado2]],
						vectores[0][i].value, sujeto, parte1))
					for (k = 1; k <= vectores[0][i].sources[0]; k++) {
						if (rango && vectores[0][i].value > numeroSO - 1) {

						} else {
							if (DEBUG) {
								printTripleta(sujeto + 1,
										resDAC.predicados[predicado2],
										vectores[0][i].value + 1, parte1);
								printTripleta(objeto2 + 1,
										vectores[0][i].sources[k],
										vectores[0][i].value + 1, parte2);
								fprintf(stdout, "\n");
							}
							resultados++;
						}
					}
			}
		}
	}
	liberarMerge();
	return resultados;
}

int s__oMergeParalelo(TREP ** treps, int numPreds, INDICEDAC sp, INDICEDAC op,
		int sujeto, int objeto2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int i = 1, j = 1, predicado, predicado2, k, l;
	int resultados = 0;

	if (DEBUG) {
		printTripleta(sujeto + 1, -1, -1, parte1);
		printTripleta(objeto2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	int indiceVector = 0;
	vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
	vectores[indiceVector][0].value = 0;

	if (!condac) {
		for (predicado = 1; predicado <= numPreds; predicado++) {

			compactNeighbourMerge(treps[predicado], objeto2, vectores,
					indiceVector, predicado, parte2);

			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}

		}
	} else {
		commonObtenerPredicados(sp, op, objeto2, parte2);

		for (predicado = 0; predicado < resDAC.numeroPredicados; predicado++) {

			compactNeighbourMerge(treps[resDAC.predicados[predicado]], objeto2,
					vectores, indiceVector, predicado, parte2);

			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}

		}
	}

	if (indiceVector > 1)
		mergeMiguel(vectores, indiceVector);

	vectores2 = (Result2 **) malloc(1000000 * sizeof(Result2 *));
	int indiceVector2 = 0;
	vectores2[indiceVector2] = malloc(sizeof(Result2) * TAMINICIAL);
	vectores2[indiceVector2][0].value = 0;

	if (!condac) {
		for (predicado2 = 1; predicado2 <= numPreds; predicado2++) {
			compactNeighbourMerge(treps[predicado2], sujeto, vectores2,
					indiceVector2, predicado2, parte1);

			if (vectores2[indiceVector2][0].value) {
				indiceVector2++;
				vectores2[indiceVector2] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores2[indiceVector2][0].value = 0;
			}
		}
	} else {
		commonObtenerPredicados(sp, op, sujeto, parte1);
		for (predicado2 = 0; predicado2 < resDAC.numeroPredicados; predicado2++) {
			compactNeighbourMerge(treps[resDAC.predicados[predicado2]], sujeto,
					vectores2, indiceVector2, resDAC.predicados[predicado2],
					parte1);

			if (vectores2[indiceVector2][0].value) {
				indiceVector2++;
				vectores2[indiceVector2] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores2[indiceVector2][0].value = 0;
			}
		}
	}

	if (indiceVector2 > 1)
		mergeMiguel(vectores2, indiceVector2);

	//Hacemos "merge en paralelo"

	i = 1;
	j = 1;
	while (i <= vectores[0][0].value && j <= vectores2[0][0].value) {

		if (vectores[0][i].value == vectores2[0][j].value) {
			for (k = 1; k <= vectores[0][i].sources[0]; k++) {
				for (l = 1; l <= vectores2[0][j].sources[0]; l++) {
					if (rango && vectores[0][i].value > numeroSO - 1) {

					} else {
						if (DEBUG) {
							printTripleta(sujeto + 1,
									vectores2[0][j].sources[l],
									vectores[0][i].value + 1, parte1);
							printTripleta(objeto2 + 1,
									vectores[0][i].sources[k],
									vectores[0][i].value + 1, parte2);
							fprintf(stdout, "\n");
						}

						resultados++;
					}
				}
			}
			i++;
			j++;
		} else if (vectores[0][i].value > vectores2[0][j].value) {
			j++;
		} else
			i++;
	}
	liberarMerge();
	liberarMerge2();
	return resultados;
}

