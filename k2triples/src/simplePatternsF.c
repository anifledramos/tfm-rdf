#include "simplePatternsF.h"
#include "commons.h"

int spoF(FILE * f,TREP ** treps, int nPreds, int s, int p, int o) {

	if (compactTreeCheckLinkQuery(treps[p], s, o))

	{
	  fprintf(f,"%d %d %d\n",s+1,p,o+1);
	} else {

	}

}

int soF(FILE * f,TREP ** treps, int nPreds, int s, int o) {
	int i, resultados = 0;

	for (i = 1; i <= nPreds; i++) {
		if (compactTreeCheckLinkQuery(treps[i], s, o)) {
			
			fprintf(f, "%d %d %d\n", s + 1, i, o + 1);
			resultados++;
		}
	}

	return resultados;
}

int soIndexF(FILE * f,TREP ** treps, INDICEDAC dac, int s, int o) {
	int i, resultados = 0;

	obtenerPredicados(dac, s);

	for (i = 0; i < resDAC.numeroPredicados; i++) {
		if (compactTreeCheckLinkQuery(treps[resDAC.predicados[i]], s, o)) {
			resultados++;
			if (DEBUG)
				fprintf(f, "%d %d %d\n", s + 1, resDAC.predicados[i],
						o + 1);
		}
	}
	return resultados;
}

int soDobleIndexF(FILE * f,TREP ** treps, INDICEDAC dac,INDICEDAC dac2, int s, int o) {
	int  resultados = 0;

	obtenerPredicados(dac, s);
	obtenerPredicados2(dac2,o);
	int i=0,j=0;

	while (i<resDAC.numeroPredicados && j<resDAC2.numeroPredicados){
		//Dado que las ristras de predicados de SP y OP están ordenadas por predicado
		if (resDAC.predicados[i]>resDAC2.predicados[j]){
			j++;
		}
		else if (resDAC.predicados[i]<resDAC2.predicados[j]){
			i++;
		}
		else {
			if (compactTreeCheckLinkQuery(treps[resDAC.predicados[i]],s,o)){
				fprintf(f, "%d %d %d\n", s + 1, resDAC.predicados[i],o + 1);
				resultados++;
			}
			i++;
			j++;
		}
	}
	
	return resultados;
}


int spF(FILE * f,TREP ** treps, int nPreds, int s, int p) {
	int i;
	uint * x = infoTOTAL[0];
	int resultados;


	resultados = compactTreeAdjacencyList(treps[p], s, x);

	for (i = 0; i < x[0]; i++) {
	   fprintf(f, "%d %d %d\n", s + 1, p, x[i + 1] + 1);
	}

	return x[0];
}

int poF(FILE * f,TREP ** treps, int nPreds, int p, int o) {
	int i;
	int resultados;
	uint * x = infoTOTAL[0];


	resultados = compactTreeInverseList(treps[p], o, x);
	
	for (i = 0; i < x[0]; i++) {
		fprintf(f, "%d %d %d\n", x[i + 1] + 1, p, o + 1);
	}
	return x[0];
}

int sF(FILE * f,TREP ** treps, int nPreds, int s) {
	int i, j;
	uint * x = infoTOTAL[0];
	int resultados = 0;



	for (i = 1; i <= nPreds; i++) {
		resultados += compactTreeAdjacencyList(treps[i], s, x);
		for (j = 0; j < x[0]; j++)
		  fprintf(f, "%d %d %d\n", s + 1, i, x[j + 1] + 1);
	}
	return resultados;
}

int sIndexF(FILE * f,TREP ** treps, INDICEDAC dac, int s) {
	uint * x = infoTOTAL[0];

	int cont = 0, i, j;
	obtenerPredicados(dac, s);
	for (i = 0; i < resDAC.numeroPredicados; i++) {
		compactTreeAdjacencyList(treps[resDAC.predicados[i]], s, x);
		for (j = 1; j <= x[0]; j++) {
			fprintf(f, "%d %d %d\n", s+1, resDAC.predicados[i], x[j]+1);
			cont++;
		}
	}
	return cont;
}

int pF(FILE * f,TREP ** treps, int nPreds, int p) {
	int i;
	SET * pares=paresBuff1;

	compactTreeRangeQuery(treps[p], 0,pares);

	for (i = 1; i <= pares[0].x; i++) {
	  fprintf(f, "%d %d %d\n", pares[i].x + 1, p, pares[i].y + 1);
	}
	return pares[0].x;

}

int pOrdenadoF(FILE * f,TREP ** treps, int nPreds, int p) {
	int i;


	SET * pares=paresBuff1;
	compactTreeRangeQuery(treps[p], 1,pares);

	for (i = 1; i <= pares[0].x; i++) {
	  fprintf(f, "%d %d %d\n", pares[i].x + 1, p, pares[i].y + 1);
	}

	return pares[0].x;

}

int oF(FILE * f,TREP ** treps, int nPreds, int o) {
	int i, j;
	uint * x = infoTOTAL[0];
	int cont = 0;


	for (i = 1; i <= nPreds; i++) {
		cont += compactTreeInverseList(treps[i], o, x);

			for (j = 0; j < x[0]; j++)
				fprintf(f, "%d %d %d", x[j + 1] + 1, i, o + 1);
	}
	return cont;
}

int oIndexF(FILE * f,TREP ** treps, INDICEDAC dac, int o) {
	int i, j;
	uint * x = infoTOTAL[0];

	int cont = 0;

	obtenerPredicados(dac, o);

	for (i = 0; i < resDAC.numeroPredicados; i++) {
		compactTreeInverseList(treps[resDAC.predicados[i]], o, x);
		for (j = 0; j < x[0]; j++) {
			cont++;
			fprintf(f, "%d %d %d", x[j + 1] + 1, resDAC.predicados[i], o + 1);
		}

	}

	return cont;

}

