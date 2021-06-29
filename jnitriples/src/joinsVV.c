#include "joinsVV.h"

/* JOIN 7 */
int _pp_Izquierda(TREP ** treps, int p1, int p2, int parte1, int parte2) {
	int rango=0;
	int i, j=-1, k;
	uint * x = infoTOTAL[0];
	SET * pares = paresBuff1;
	int resultados = 0;

	// if (DEBUG) {
	// 	printTripleta(-1, p1, -1, parte1);
	// 	printTripleta(-1, p2, -1, parte2);
	// 	fprintf(stdout, "\n");
	// }

	if (parte1!=parte2) rango=1;
	compactTreeRangeQuery(treps[p1], parte1, pares);


	i = 1;

	while (i <= pares[0].x) {
		if (rango && pares[j].y >numeroSO - 1) {i++;continue;}
		compactNeighbourOpposite(treps[p2], obtenerPar(pares[i], parte1), x, parte2);
		j = i;
		while (obtenerPar(pares[i], parte1) == obtenerPar(pares[j], parte1)) {
		for (k = 1; k <= x[0]; k++) {
			// if (DEBUG) {
			// 		printTripleta(obtenerOppositePar(pares[j], parte1) + 1, p1,
			// 				obtenerPar(pares[j], parte1) + 1, parte1);
			// 		printTripleta(x[k] + 1, p2, obtenerPar(pares[j], parte1) + 1, parte2);
			// 		fprintf(stdout, "\n");
			// 	}

				resultados++;
			}
			j++;
		}

		i = j;
	}
//	fprintf(stderr,"asdasdasd\n");
	return resultados;
}

int _pp_Derecha(TREP ** treps, int p1, int p2, int parte1, int parte2) {

	int i, j, k;
	uint * x = infoTOTAL[0];
	SET * pares = paresBuff1;
	int resultados = 0;
	int rango=0;
	if (parte1!=parte2) rango=1;
	// if (DEBUG) {
	// 	printTripleta(-1, p1, -1, parte1);
	// 	printTripleta(-1, p2, -1, parte2);
	// 	fprintf(stdout, "\n");
	// }

	compactTreeRangeQuery(treps[p2], parte2, pares);


	i = 1;

	while (i <= pares[0].x) {
		int par=obtenerPar(pares[i],parte2);
		if (rango && par > numeroSO - 1) {i++;continue;}
		compactNeighbourOpposite(treps[p1], par, x,parte1);
		j = i;
		while (obtenerPar(pares[i],parte2) == obtenerPar(pares[j],parte2)) {

			for (k = 1; k <= x[0]; k++) {

				// if (DEBUG) {
				// 	printTripleta(x[k] + 1, p1, obtenerPar(pares[j],parte2) + 1, parte1);
				// 	printTripleta(obtenerOppositePar(pares[j],parte2)+1, p2,  obtenerPar(pares[j],parte2)+1, parte2);
				// 	fprintf(stdout, "\n");
				// }
				resultados++;
			}
			j++;
		}

		i = j;
	}
	return resultados;
}

int _pp_Interactiva(TREP ** treps, int p1, int p2, int parte1, int parte2) {
	int i, resultados = 0;
	SET * pares = paresBuff1;
	pares[0].x = 1;
	int rango=0;

	if (parte1!=parte2) rango=1;
	// if (DEBUG) {
	// 	if (foutput==NULL){
	// 		printTripleta(-1, p1, -1, parte1);
	// 		printTripleta(-1, p2, -1, parte2);
	// 		fprintf(stdout,"\n");
	// 	}
	// }

	joinAsim7(treps[p1], treps[p2], pares, parte1, parte2);

	for (i = 1; i <= pares[0].x; i++) {
			if (rango && pares[i].y > numeroSO -1) {i++;continue;}
		// if (DEBUG) {
		// 	printCompleto(pares[i].x + 1, p1, pares[i].y + 1, parte1,pares[i].z + 1, p2, pares[i].y + 1, parte2);
		// }
		resultados++;
	}
	return resultados;

}


int _pp_Independiente(TREP ** treps, int p1, int p2, int parte1, int parte2) {

	int i, j,  m, l;
	int resultados = 0;

	SET * pares = paresBuff1;
	SET * pares2 = paresBuff2;
	int rango=0;
	if (parte1!=parte2) rango=1;
	// if (DEBUG) {
	// 	printTripleta(-1, p1, -1, parte1);
	// 	printTripleta(-1, p2, -1, parte2);
	// 	fprintf(stdout, "\n");
	// }

	compactTreeRangeQuery(treps[p1], parte1, pares);

	compactTreeRangeQuery(treps[p2], parte2, pares2);


	i = 1;
	j = 1;
	while (i <= pares[0].x && j <= pares2[0].x) {
		int par1=obtenerPar(pares[i],parte1);
		int par2=obtenerPar(pares2[j],parte2);
		if (rango && par1 > numeroSO -1) break;

		if (par1 > par2) {
			j++;
		}

		if (par1 < par2) {
			i++;
		}

		if (par1 == par2) {
			l = i;
			m=j;
			while (l <= pares[0].x && obtenerPar(pares[l],parte1) == obtenerPar(pares[i],parte1)) {
				m = j;
				while (m <= pares2[0].x && obtenerPar(pares2[m],parte2) == obtenerPar(pares2[j],parte2)) {
					// if (DEBUG){
					// 	printTripleta(obtenerOppositePar(pares[l],parte1) + 1, p1, obtenerPar(pares[l],parte1) + 1, parte1);
					// 	printTripleta(obtenerOppositePar(pares2[m],parte2) + 1, p2, obtenerPar(pares[l],parte1) + 1, parte2);
					// 	fprintf(stdout, "\n");

					// }
						resultados++;
					m++;
				}
				l++;
			}
			j = m;
			i = l;
		}
	}
	return resultados;
}


/* JOIN 8 */
int _p__Izquierda(TREP ** treps, int p1, int nPreds, INDICEDAC sp,INDICEDAC op, int condac, int parte1, int parte2) {
	int i, j=0, k, m;
	uint * x = infoTOTAL[0];
	SET * pares = paresBuff1;
	int resultados = 0;

	// if (DEBUG){
	// 	if (foutput==NULL){
	// 		printTripleta(-1, p1, -1, parte1);
	// 		printTripleta(-1, -1, -1, parte2);
	// 		fprintf(stdout, "\n");
	// 	}	
	// }
	int rango=0;
	if (parte1!=parte2) rango=1;
	compactTreeRangeQuery(treps[p1], parte1, pares);


	i = 1;

	if (condac) {
		while (i <= pares[0].x) {

			int par=obtenerPar(pares[i],parte1);

			if (rango && par> numeroSO -1) {i++;continue;}

			commonObtenerPredicados(op,sp,par,parte2);

			for (m = 0; m < resDAC.numeroPredicados; m++) {

				compactNeighbourOpposite(treps[resDAC.predicados[m]],par,x,parte2);

				//compactTreeAdjacencyList(treps[resDAC.predicados[m]], pares[i].y, x);

				j = i;
				while (j<=pares[0].x && par == obtenerPar(pares[j],parte1)) {

					for (k = 1; k <= x[0]; k++) {
						// if (DEBUG) {
						// 	printCompleto(obtenerOppositePar(pares[j],parte1) + 1, p1,
						// 			obtenerPar(pares[j],parte1) + 1, parte1,
						// 			x[k] + 1, 
						// 			resDAC.predicados[m], 
						// 			obtenerPar(pares[j],parte1) + 1, parte2);
						// }
						resultados++;
					}
					j++;
				}
			}
			i = j;
		}
	} else {
		while (i <= pares[0].x) {
			int par=obtenerPar(pares[i],parte1);

			if (rango && par> numeroSO -1) {i++;continue;}

			for (m = 1; m <= nPreds; m++) {

				compactNeighbourOpposite(treps[m],par,x,parte2);

				j = i;
				while (par == obtenerPar(pares[j],parte1)) {
					for (k = 1; k <= x[0]; k++) {


						// if (DEBUG) {
						// 	printTripleta	(obtenerOppositePar(pares[j],parte1) + 1, p1,
						// 							obtenerPar(pares[j],parte1) + 1, parte1);
						// 	printTripleta	(x[k] + 1, m, obtenerPar(pares[j],parte1) + 1, parte2);
						// 	fprintf(stdout, "\n");
						// }
						resultados++;
					}
					j++;
				}
			}
			i = j;
		}
	}
	return resultados;
}

int _p__Interactiva(TREP ** treps, int p1, int nPreds, int parte1, int parte2) {
	int i, resultados = 0;
	SET * pares = paresBuff1;
	pares[0].x = 1;
	// if (DEBUG){
	// 	printTripleta(-1, p1, -1, parte1);
	// 	printTripleta(-1, -1, -1, parte2);
	// 	fprintf(stdout, "\n");
	// }

	int rango=0;
	if (parte1!=parte2) rango=1;
	joinAsim8(treps[p1], p1, treps, nPreds, pares, parte1, parte2);

	for (i = 1; i <= pares[0].x; i++) {
		if (rango && pares[i].y > numeroSO -1) continue;
		// if (DEBUG) {
		// 	printTripleta(pares[i].x + 1, p1, pares[i].y + 1, parte1);
		// 	printTripleta(pares[i].w + 1, pares[i].z, pares[i].y + 1, parte2);
		// 	fprintf(stdout, "\n");
		// }
		resultados++;
	}

	return resultados;

}
