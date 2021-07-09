#include "simplePatterns.h"
#include "commons.h"

int MAXRES = 50000000;
int **bufferResultados = NULL;

void initBuffer() {
	if (bufferResultados == NULL) {
		bufferResultados = (int **) malloc(2 * sizeof(int *));
		bufferResultados[0] = (int *) malloc(MAXRES * sizeof(int));
		bufferResultados[1] = (int *) malloc(MAXRES * sizeof(int));
	}
}

int spo(K2TRIPLES * k2triples, int s, int p, int o) {

	initBuffer();
	if (DEBUG) {
		fprintf(stdout, "<%d,%d,%d>\n", s + 1, p, o + 1);
	}
	if (compactTreeCheckLinkQuery(k2triples->trees[p], s, o))

	{
		if (DEBUG)
			fprintf(stdout, "1\n");

		return 1;
	} else {
		if (DEBUG)
			fprintf(stdout, "0\n");
		return 0;
	}

}

int *so(K2TRIPLES * k2triples, int s, int o) {

	initBuffer();
	int i, resultados = 0;

	int * p = bufferResultados[0];

	if (DEBUG)
		fprintf(stdout, "<%d,?,%d>\n", s + 1, o + 1);
	for (i = 0; i < k2triples->npreds; i++) {
		if (compactTreeCheckLinkQuery(k2triples->trees[i], s, o)) {
			if (DEBUG)
				fprintf(stdout, "<%d,%d,%d>\n", s + 1, i+1, o + 1);
			resultados++;
			if (resultados < MAXRES) p[resultados] = i;
		}
	}

	p[0] = resultados;

	return p;
}

int * soIndex(K2TRIPLES * k2triples,  int s, int o) {
	initBuffer();
	int i, resultados = 0;
	if (DEBUG)
		fprintf(stdout, "<%d,?,%d>\n", s + 1, o + 1);

	int * p = bufferResultados[0];


	obtenerPredicados(*k2triples->indiceSP, s);

	for (i = 0; i < resDAC.numeroPredicados; i++) {
		if (compactTreeCheckLinkQuery(k2triples->trees[resDAC.predicados[i]], s, o)) {
			resultados++;
			if (resultados < MAXRES) p[resultados] = resDAC.predicados[i];
			if (DEBUG)
				fprintf(stdout, "<%d,%d,%d>\n", s + 1, resDAC.predicados[i],
						o + 1);

		}
	}

	p[0] = resultados;
	return p;
}

//FIXME
int *soDobleIndex(K2TRIPLES * k2triples, int s, int o) {
	initBuffer();
	int  resultados = 0;

	int * p = bufferResultados[0];

	if (DEBUG)
		fprintf(stdout, "<%d,?,%d>\n", s + 1, o + 1);
	obtenerPredicados(*k2triples->indiceSP, s);
	obtenerPredicados2(*k2triples->indiceOP,o);
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
			if (compactTreeCheckLinkQuery(k2triples->trees[resDAC.predicados[i]],s,o)){
				fprintf(stdout, "<%d,%d,%d>\n", s + 1, resDAC.predicados[i],o + 1);
				resultados++;
				if (resultados < MAXRES) p[resultados] = resDAC.predicados[i];
				//i++;
				//j++;
			}
			i++;
			j++;
		}
	}
	p[0] = resultados;
	return p;
}


int *sp(K2TRIPLES * k2triples, int s, int p) {
	initBuffer();
	int i;
	uint * x = infoTOTAL[0];
	int resultados;

	if (DEBUG)
		fprintf(stdout, "<%d,%d,?>\n", s + 1, p);

	resultados = compactTreeAdjacencyList(k2triples->trees[p], s, x);

	if (DEBUG)
		for (i = 0; i < x[0]; i++) {
			fprintf(stdout, "<%d,%d,%d>\n", s + 1, p, x[i + 1] + 1);
		}

	return x;
}


int* po(K2TRIPLES * k2triples, int p, int o) {
	initBuffer();
	int i;
	int resultados;
	uint * x = infoTOTAL[0];

	if (DEBUG)
		fprintf(stdout, "<?,%d,%d>\n", p, o + 1);

	resultados = compactTreeInverseList(k2triples->trees[p], o, x);

	if (DEBUG)
		for (i = 0; i < x[0]; i++) {
			fprintf(stdout, "<%d,%d,%d>", x[i + 1] + 1, p, o + 1);
		}
	return x;
}

int **s(K2TRIPLES * k2triples, int s) {
	initBuffer();
	int i, j;
	uint * x = infoTOTAL[0];
	int resultados = 0;

	if (DEBUG)
		fprintf(stdout, "<%d,?,?>\n", s + 1);


	for (i = 0; i < k2triples->npreds; i++) {

		int thisres = compactTreeAdjacencyList(k2triples->trees[i], s, x);


		for (j = 0; j < x[0]; j++) {
			bufferResultados[0][1+resultados + j] = i;
			bufferResultados[1][1+resultados + j] = x[j + 1];
		}
		resultados += thisres;

		if (DEBUG) {
			for (j = 0; j < x[0]; j++)
				fprintf(stdout, "<%d,%d,%d>\n", s + 1, i, x[j + 1] + 1);
		}
	}
	bufferResultados[0][0] = resultados;
	return bufferResultados;
}

//FIXME
int **sIndex(K2TRIPLES * k2triples,  int s) {
	initBuffer();
	uint * x = infoTOTAL[0];
	if (DEBUG)
		fprintf(stdout, "<%d,?,?>\n", s + 1);

	int cont = 0, i, j;
	obtenerPredicados(*k2triples->indiceSP, s);
	for (i = 0; i < resDAC.numeroPredicados; i++) {
		compactTreeAdjacencyList(k2triples->trees[resDAC.predicados[i]-1], s, x);
		for (j = 1; j <= x[0]; j++) {
			if (DEBUG) {
				fprintf(stdout, "<%d,%d,%d>\n", s+1, i, x[j]+1);
			}
			cont++;
		}
	}


	return NULL;
}


int **p(K2TRIPLES * k2triples, int p) {
	initBuffer();
	int i;
	SET * pares=paresBuff1;
	if (DEBUG)
		fprintf(stdout, "<?,%d,?>\n", p);

	compactTreeRangeQuery(k2triples->trees[p], 0,pares);

	if (DEBUG) {
		for (i = 1; i <= pares[0].x; i++) {
			fprintf(stdout, "<%d,%d,%d>\n", pares[i].x + 1, p, pares[i].y + 1);
		}
	}

	for (i = 1; i <= pares[0].x; i++) {
		bufferResultados[0][i] = pares[i].x;
		bufferResultados[1][i] = pares[i].y;
	}
	bufferResultados[0][0] = pares[0].x;

	return bufferResultados;
}

//FIXME
int **pOrdenado(K2TRIPLES * k2triples, int p) {
	initBuffer();
	int i;

	if (DEBUG)
		fprintf(stdout, "<?,%d,?>\n", p);

	SET * pares=paresBuff1;
	compactTreeRangeQuery(k2triples->trees[p], 1,pares);

	if (DEBUG) {
		for (i = 1; i <= pares[0].x; i++) {
			fprintf(stdout, "<%d,%d,%d>\n", pares[i].x + 1, p, pares[i].y + 1);
		}
	}

	return NULL;

}

int **o(K2TRIPLES * k2triples, int o) {
	initBuffer();
	int i, j;
	uint * x = infoTOTAL[0];
	int cont = 0;

	if (DEBUG)
		fprintf(stdout, "<?,?,%d>\n", o + 1);

	for (i = 0; i < k2triples->npreds; i++) {
		int thisres = compactTreeInverseList(k2triples->trees[i], o, x);

		for (j = 0; j < x[0]; j++) {
			bufferResultados[0][1+cont + j] = x[j + 1];
			bufferResultados[1][1+cont + j] = i;
		}

		cont += thisres;
		if (DEBUG)
			for (j = 0; j < x[0]; j++)
				fprintf(stdout, "<%d,%d,%d>", x[j + 1] + 1, i, o + 1);
	}

	bufferResultados[0][0] = cont;
	return bufferResultados;
}

//FIXME
int **oIndex(K2TRIPLES * k2triples,  int o) {
	initBuffer();
	int i, j;
	uint * x = infoTOTAL[0];
	if (DEBUG)
		fprintf(stdout, "<?,?,%d>\n", o + 1);

	int cont = 0;

	obtenerPredicados(*k2triples->indiceOP, o);

	for (i = 0; i < resDAC.numeroPredicados; i++) {
		compactTreeInverseList(k2triples->trees[resDAC.predicados[i]-1], o, x);
		for (j = 0; j < x[0]; j++) {
			cont++;
			fprintf(stdout, "<%d,%d,%d>", x[j + 1] + 1, resDAC.predicados[i], o + 1);
		}

	}

	return NULL;

}
