#include "joinsFV.h"

//JOIN 4 - EVALUACION EN CADENA
int _ppoDerecha(TREP ** treps, int p1, int p2, int o2, int parte1, int parte2) {

	int i = 1, j = 1;
	int resultado = 0;

	int rango = 0;

	if (parte1 != parte2)
		rango = 1;

	uint * x = infoTOTAL[0];
	uint * y = infoTOTAL[1];

	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(-1, p1, -1, parte1);
			printTripleta(o2 + 1, p2, -1, parte2);
			fprintf(stdout, "\n");
		}	
	}

	compactNeighbour(treps[p2], o2, x, parte2);

	for (i = 1; i <= x[0]; i++) {

		if (rango && x[i] > numeroSO - 1) {
			break;
		}
		compactNeighbourOpposite(treps[p1], x[i], y, parte1);

		for (j = 1; j <= y[0]; j++) {
			if (DEBUG) {
				printCompleto(y[j] + 1, p1, x[i] + 1, parte1,o2 + 1, p2, x[i] + 1, parte2);
			}
			resultado++;
		}
	}

	return resultado;
}

//JOIN 4 - EVALUACION INTERACTIVA
int _ppoInteractiva(TREP ** treps, int p1, int p2, int o2, int parte1,
		int parte2) {
	SET * pares = paresBuff1;
	int i;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		printTripleta(-1, p1, -1, parte1);
		printTripleta(o2 + 1, p2, -1, parte2);
		fprintf(stdout, "\n");
	}

	int resultados = 0;

	joinAsim4(treps[p2], o2, treps[p1], pares, parte1, parte2);

	for (i = 1; i <= infoTOTAL[0][0]; i++) {
		if (rango && (infoTOTAL[0][i] > numeroSO - 1)) {

		} else {
			if (DEBUG) {
				printTripleta(infoTOTAL[1][i] + 1, p1, infoTOTAL[0][i] + 1,
						parte1);
				printTripleta(o2 + 1, p2, infoTOTAL[0][i] + 1, parte2);
				fprintf(stdout, "\n");
			}
			resultados++;
		}
	}

	return resultados;
}

//JOIN 4 - EVALUACION INDEPENDIENTE
int _ppoIndependiente(TREP ** treps, int p1, int p2, int o2, int parte1,
		int parte2) {
	int i, j;
	SET * pares = paresBuff1;
	uint * x = infoTOTAL[0];
	int resultado = 0;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		printTripleta(-1, p1, -1, parte1);
		printTripleta(o2 + 1, p2, -1, parte2);
		fprintf(stdout, "\n");
	}
	//Evaluación de parte izquierda: ? p1 ? --> devuelve el resultado ordenado por columnas
	// resultado en pares
	compactTreeRangeQuery(treps[p1], parte1, pares);
	//Evaluación de parte derecha: ? p2 o2
	compactNeighbour(treps[p2], o2, x, parte2);
	i = 1;
	j = 1;
	while (i <= pares[0].x && j <= x[0]) {

		if (rango && x[j] > numeroSO - 1)
			break;

		if (obtenerPar(pares[i], parte1) == x[j]) {
			resultado++;
			if (DEBUG) {
				printTripleta(obtenerOppositePar(pares[i], parte1) + 1, p1,
						x[j] + 1, parte1);
				printTripleta(o2 + 1, p2, x[j] + 1, parte2);
				fprintf(stdout, "\n");
			}
			i++;
		} else if (obtenerPar(pares[i], parte1) > x[j]) {
			j++;
		} else
			i++;
	}
	return resultado;
}

//JOIN 5 - EVALUACION EN CADENA
int _p_oDerecha(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1,
		int o2, int condac, int parte1, int parte2) {
	int i, j, l;
	uint * x = infoTOTAL[0];
	int resultados = 0;
	int k;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(-1, p1, -1, parte1);
			printTripleta(o2 + 1, -1, -1, parte2);
			fprintf(stdout, "\n");
		}
	}
	if (condac) {
		commonObtenerPredicados(sp, op, o2, parte2);
	}

	int indiceVector = 0;
	vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
	vectores[indiceVector][0].value = 0;
	int total = 0;
	if (condac) {
		for (j = 0; j < resDAC.numeroPredicados; j++) {
			compactNeighbourMerge(treps[resDAC.predicados[j]], o2, vectores,
					indiceVector, resDAC.predicados[j], parte2);
			total += vectores[indiceVector][0].value;
			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}
		}
	} else {
		for (j = 1; j <= nPreds; j++) {
			compactNeighbourMerge(treps[j], o2, vectores, indiceVector, j,
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

	for (i = 1; i <= vectores[0][0].value; i++) {
		//TODO: SI NUMEROso=0 NO FILTRA
		if (rango && vectores[0][i].value > numeroSO - 1) {
			break;
		}
		compactNeighbourOpposite(treps[p1], vectores[0][i].value, x, parte1);

		for (k = 1; k <= vectores[0][i].sources[0]; k++) {
			for (l = 1; l <= x[0]; l++) {

					resultados++;

				if (DEBUG) {
					printCompleto(x[l] + 1, p1, vectores[0][i].value + 1,
							parte1,o2 + 1, vectores[0][i].sources[k],
							vectores[0][i].value + 1, parte2);
				}
			}
		}
	}
	liberarMerge();
	return resultados;
}

//JOIN 5 - EVALUACION EN CADENA
int _p_oIzquierda(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1,
		int o2, int condac, int parte1, int parte2) {
	int i, j, k;
	SET * pares = paresBuff1;
	int resultados = 0;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		printTripleta(-1, p1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}
	compactTreeRangeQuery(treps[p1], parte1, pares);
	i = 1;

	if (!condac) {
		while (i <= pares[0].x) {

			if (rango && obtenerPar(pares[i],parte1) > numeroSO - 1)
				break;

			j = i;
			for (k = 1; k <= nPreds; k++) {
				if (compactLink(treps[k], obtenerPar(pares[i], parte1), o2,
						parte2)) {
					j = i;
					while (j <= pares[0].x
							&& obtenerPar(pares[i], parte1)
									== obtenerPar(pares[j], parte1)) {
						if (DEBUG) {
							printTripleta(
									obtenerOppositePar(pares[j], parte1) + 1,
									p1, obtenerPar(pares[j], parte1) + 1,
									parte1);
							printTripleta(o2 + 1, k,
									obtenerPar(pares[j], parte1) + 1, parte2);
							fprintf(stdout, "\n");
						}
						resultados++;
						j++;
					}
				}
			}
			if (i < j)
				i = j;
			else {
				i++;
			}
		}
	} else {
		commonObtenerPredicados(sp, op, o2, parte2);
		while (i <= pares[0].x) {
			
			if (rango && obtenerPar(pares[i],parte1) > numeroSO - 1)
				break;

			j = i;
			for (k = 0; k < resDAC.numeroPredicados; k++) {
				
				if (compactLink(treps[resDAC.predicados[k]],
						obtenerPar(pares[i], parte1), o2, parte2)) {
					j = i;
					while (j <= pares[0].x
							&& obtenerPar(pares[i], parte1)
									== obtenerPar(pares[j], parte1)) {
						if (DEBUG) {
							printTripleta(
									obtenerOppositePar(pares[j], parte1) + 1,
									p1, obtenerPar(pares[j], parte1) + 1,
									parte1);
							printTripleta(o2 + 1, resDAC.predicados[k],
									obtenerPar(pares[j], parte1) + 1, parte2);
							fprintf(stdout, "\n");
						}
						resultados++;
						j++;
					}
				}

			}
			if (i < j)
				i = j;
			else {
				i++;
			}
		}
	}
	return resultados;
}

int _p_oInteractiva(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op,
		int p1, int o2, int condac, int parte1, int parte2) {
	SET * pares = paresBuff1;
	int i;
	int resultados = 0;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;

	if (DEBUG) {
		printTripleta(-1, p1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	joinAsim5(treps, o2, treps[p1], nPreds, p1, sp, op, condac, pares, parte1,
			parte2);

	for (i = 1; i <= pares[0].x; i++) {
		if (rango && pares[i].y > numeroSO - 1)
			break;
		resultados++;
		if (DEBUG) {
			printTripleta(pares[i].x + 1, p1, pares[i].y + 1, parte1);
			printTripleta(o2 + 1, pares[i].z, pares[i].y + 1, parte2);
			fprintf(stdout, "\n");
		}
	}
	return resultados;

}

int _p_oIndependiente(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op,
		int p1, int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int i, j, k;
	SET * pares = paresBuff1;
	uint * x = infoTOTAL[0];
	int resultado = 0;

	if (DEBUG) {
		printTripleta(-1, p1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	//Evaluación de parte izquierda: ? p1 ? --> devuelve el resultado ordenado por columnas
	// resultado en pares
	compactTreeRangeQuery(treps[p1], parte1, pares);

	if (condac) {
		commonObtenerPredicados(sp, op, o2, parte2);
		for (k = 0; k < resDAC.numeroPredicados; k++) {
			compactNeighbour(treps[resDAC.predicados[k]], o2, x, parte2);
			//Para cada lista inversa, se realiza la intersección con el rango del primer patrón
			i = 1;
			j = 1;
			while (i <= pares[0].x && j <= x[0]) {

				if (rango && x[j] > numeroSO - 1)
					break;

				if (obtenerPar(pares[i], parte1) == x[j]) {
					resultado++;
					if (DEBUG) {
						printTripleta(obtenerOppositePar(pares[i], parte1) + 1,
								p1, obtenerPar(pares[i], parte1) + 1, parte1);
						printTripleta(o2 + 1, resDAC.predicados[k], x[j] + 1,
								parte2);
						fprintf(stdout, "\n");
					}
					i++;
				} else if (obtenerPar(pares[i], parte1) > x[j]) {
					j++;
				} else
					i++;
			}
		}
	} else {

		for (k = 1; k <= nPreds; k++) {
			compactNeighbour(treps[k], o2, x, parte2);
			//Para cada lista inversa, se realiza la intersección con el rango del primer patrón
			i = 1;
			j = 1;

			while (i <= pares[0].x && j <= x[0]) {
				if (rango && x[j] > numeroSO - 1)
					break;

				if (obtenerPar(pares[i], parte1) == x[j]) {
					resultado++;
					if (DEBUG) {
						printTripleta(obtenerOppositePar(pares[i], parte1) + 1,
								p1, obtenerPar(pares[i], parte1) + 1, parte1);
						printTripleta(o2 + 1, k, x[j] + 1, parte2);
						fprintf(stdout, "\n");
					}
					i++;
				} else if (obtenerPar(pares[i], parte1) > x[j]) {
					j++;
				} else
					i++;
			}

		}
	}
	return resultado;
}

int _p_oMergeIndependiente(TREP ** treps, int nPreds, INDICEDAC sp,
		INDICEDAC op, int p1, int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int i, j, k;
	SET * pares = paresBuff1;
	int resultado = 0;
	int indiceVector = 0;
	vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
	vectores[indiceVector][0].value = 0;
	int total = 0;

	if (DEBUG) {
		printTripleta(-1, p1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}
	//Evaluación de parte izquierda: ? p1 ? --> devuelve el resultado ordenado por columnas
	// resultado en pares
	compactTreeRangeQuery(treps[p1], parte1, pares);

	//Evaluacion parte derecha: merge de n columnas

	if (condac) {
		commonObtenerPredicados(sp, op, o2, parte2);
		for (j = 0; j < resDAC.numeroPredicados; j++) {
			compactNeighbourMerge(treps[resDAC.predicados[j]], o2, vectores,
					indiceVector, resDAC.predicados[j], parte2);
			total += vectores[indiceVector][0].value;
			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}
		}
	} else {
		for (j = 1; j <= nPreds; j++) {
			compactNeighbourMerge(treps[j], o2, vectores, indiceVector, j,
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

	i = 1;
	j = 1;
	while (i <= pares[0].x && j <= vectores[0][0].value) {
		if (rango && obtenerPar(pares[i], parte1) > numeroSO - 1)
			break;
		if (obtenerPar(pares[i], parte1) == vectores[0][j].value) {
			for (k = 1; k <= vectores[0][j].sources[0]; k++) {
				resultado++;
				if (DEBUG) {

					printTripleta(obtenerOppositePar(pares[i], parte1) + 1, p1,
							obtenerPar(pares[i], parte1) + 1, parte1);
					printTripleta(o2 + 1, vectores[0][j].sources[k],
							vectores[0][j].value + 1, parte2);
					fprintf(stdout, "\n");
				}
			}
			i++;
		} else if (obtenerPar(pares[i], parte1) > vectores[0][j].value) {
			j++;
		} else
			i++;
	}

	liberarMerge();

	return resultado;
}
/* JOIN 52 */
/* Evaluación en cadena: por la derecha */
int __poDerecha(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int s1,
		int p1, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int i, j, l, m;

	uint * x = infoTOTAL[0];
	uint * y = infoTOTAL[1];
	int resultados = 0;
	int k;
	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(s1+1, p1, -1, parte1);
			printTripleta(-1, -1, -1, parte2);
			fprintf(stdout, "\n");
		}	
	}

	int total = 0;

	//Patrón derecha
	compactNeighbour(treps[p1], s1, x, parte1);

	for (i = 1; i <= x[0]; i++) {

		if (rango && x[i] > numeroSO - 1) {
			break;
		}

		commonObtenerPredicados(op, sp, x[i], parte2);

		for (j = 0; j < resDAC.numeroPredicados; j++) {

			compactNeighbourOpposite(treps[resDAC.predicados[j]], x[i], y,
					parte2);

			for (l = 1; l <= y[0]; l++) {
				if (DEBUG) {
				
					printCompleto(s1 + 1, p1, x[i] + 1, parte1,y[l] + 1, resDAC.predicados[j], x[i] + 1,
							parte2);
				}
				resultados++;
			}
		}
	}

	return resultados;
}

int __poInteractiva(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op,
		int s1,int p1, int parte1, int parte2) {

	SET * pares = paresBuff1;
	int i;
	int resultados = 0;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		printTripleta(s1+1, p1, -1, parte1);
		printTripleta(-1,-1, -1, parte2);
		fprintf(stdout, "\n");
	}

	joinAsim52(treps, s1,p1, nPreds, sp, op, 1, pares, parte1,
			parte2);

	for (i = 1; i <= pares[0].x; i++) {
		if (rango && pares[i].z > numeroSO - 1)
			break;
		resultados++;
		if (DEBUG) {
			printTripleta(s1 + 1, p1, pares[i].z + 1, parte1);
			printTripleta(pares[i].x + 1, pares[i].y, pares[i].z + 1, parte2);
			fprintf(stdout, "\n");
		}
	}
	return resultados;
}

/* JOIN 6 */

/* Evaluación en cadena: por la derecha */
int ___oDerecha(TREP ** treps, int nPreds, INDICEDAC indiceSP,
		INDICEDAC indiceOP, int o2, int condac, int parte1, int parte2) {
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	int i, j, l, m;
	uint * x = infoTOTAL[0];
	int resultados = 0;
	int k;
	if (DEBUG) {
		if (foutput==NULL){
			printTripleta(-1, -1, -1, parte1);
			printTripleta(o2 + 1, -1, -1, parte2);
			fprintf(stdout, "\n");
		}	
	}

	int indiceVector = 0;
	vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
	vectores[indiceVector][0].value = 0;
	int total = 0;

	if (condac) {
		//obtenerPredicados(indiceOP, o2);
		commonObtenerPredicados(indiceSP, indiceOP, o2, parte2);

		for (j = 0; j < resDAC.numeroPredicados; j++) {

			compactNeighbourMerge(treps[resDAC.predicados[j]], o2, vectores,
					indiceVector, resDAC.predicados[j], parte2);

			total += vectores[indiceVector][0].value;

			if (vectores[indiceVector][0].value) {
				indiceVector++;
				vectores[indiceVector] = malloc(sizeof(Result2) * TAMINICIAL);
				vectores[indiceVector][0].value = 0;
			}
		}
	} else {
		for (j = 1; j <= nPreds; j++) {
			compactNeighbourMerge(treps[j], o2, vectores, indiceVector, j,
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

	if (!condac) {
		for (i = 1; i <= vectores[0][0].value; i++) {
			if (rango && vectores[0][i].value > numeroSO - 1)
				break;
			for (m = 1; m <= nPreds; m++) {

				compactNeighbourOpposite(treps[m], vectores[0][i].value, x,
						parte1);
				for (k = 1; k <= vectores[0][i].sources[0]; k++) {
					for (l = 1; l <= x[0]; l++) {
						resultados++;
						if (DEBUG) {
							printCompleto(x[l] + 1, m, vectores[0][i].value + 1,
									parte1,o2 + 1, vectores[0][i].sources[k],
									vectores[0][i].value + 1, parte2);
						}
					}
				}
			}
		}
	} else {
		for (i = 1; i <= vectores[0][0].value; i++) {
			if (rango && vectores[0][i].value > numeroSO - 1)
				break;
			commonObtenerPredicados(indiceOP, indiceSP, vectores[0][i].value,
					parte1);
			for (m = 0; m < resDAC.numeroPredicados; m++) {
				compactNeighbourOpposite(treps[resDAC.predicados[m]],
						vectores[0][i].value, x, parte1);
				for (k = 1; k <= vectores[0][i].sources[0]; k++) {
					for (l = 1; l <= x[0]; l++) {
						resultados++;
						if (DEBUG) {
							printCompleto(x[l] + 1, resDAC.predicados[m],
									vectores[0][i].value + 1, parte1,o2 + 1, vectores[0][i].sources[k],
									vectores[0][i].value + 1, parte2);
						}
					}
				}
			}
		}
	}
	liberarMerge();
	return resultados;
}

int ___oInteractiva(TREP ** treps, int nPreds, INDICEDAC indiceSP,
		INDICEDAC indiceOP, int o2, int condac, int parte1, int parte2) {
	SET * pares = paresBuff1;
	int i;
	int resultados = 0;
	int rango = 0;

	if (parte1 != parte2)
		rango = 1;
	if (DEBUG) {
		printTripleta(-1, -1, -1, parte1);
		printTripleta(o2 + 1, -1, -1, parte2);
		fprintf(stdout, "\n");
	}

	joinAsim6(treps, o2, nPreds, indiceSP, indiceOP, condac, pares, parte1,
			parte2);

	for (i = 1; i <= pares[0].x; i++) {
		if (rango && pares[i].z > numeroSO - 1)
			break;
		resultados++;
		if (DEBUG) {
			printTripleta(pares[i].x + 1, pares[i].y, pares[i].z + 1, parte1);
			printTripleta(o2 + 1, pares[i].w, pares[i].z + 1, parte2);
			fprintf(stdout, "\n");
		}
	}
	return resultados;
}
