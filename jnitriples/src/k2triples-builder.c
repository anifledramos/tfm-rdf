#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "directcodesI.h"
#include "commons.h"
#include "k2triples-builder.h"


#define MAXTRIPLEBUFFER 200
#define INDEXSUJETO 3
#define TAM 200

typedef struct subject {
	int numAristas;
	int tamanio;
	int * objetos;
} SUJETO;

int MINPREDICADO, MAXPREDICADO, MINSUJETO, MAXSUJETO, MINOBJETO, MAXOBJETO;

int identifiersStats(FILE * fdataset) {
	int minPredicado = -1;
	int maxPredicado = -1;
	int minSujeto = -1;
	int maxSujeto = -1;
	int minObjeto = -1;
	int maxObjeto = -1;
	int numAristas = 0;
	int numEnteros, predicado, objeto, sujeto;
	int buffer[MAXTRIPLEBUFFER * 3];


	// PARSEAR	

	while (0 != (numEnteros = fread(buffer, sizeof(int), MAXTRIPLEBUFFER * 3, fdataset))) {
		int index = 0;
		while (index < numEnteros) {
			// Lectura del buffer

			predicado = buffer[index++];
			sujeto = buffer[index++];
			objeto = buffer[index++];

			if (!numAristas) {
				minPredicado = predicado;
				maxPredicado = predicado;
				minObjeto = objeto;
				maxObjeto = objeto;
				minSujeto = sujeto;
				maxSujeto = sujeto;
			} else {
				if (sujeto > maxSujeto)
					maxSujeto = sujeto;
				if (sujeto < minSujeto)
					minSujeto = sujeto;
				if (objeto > maxObjeto)
					maxObjeto = objeto;
				if (objeto < minObjeto)
					minObjeto = objeto;
				if (predicado > maxPredicado)
					maxPredicado = predicado;
				if (predicado < minPredicado)
					minPredicado = predicado;
			}

			numAristas++;
		}
	}

	MINPREDICADO = minPredicado;
	MAXPREDICADO = maxPredicado;
	MINOBJETO = minObjeto;
	MAXOBJETO = maxObjeto;
	MINSUJETO = minSujeto;
	MAXSUJETO = maxSujeto;

	fprintf(stderr, "********* ESTADÍSTICAS **************\n");
	fprintf(stderr, "* TRIPLETAS: %d\n", numAristas);
	fprintf(stderr, "* SUJETOS: de %d a %d\n", minSujeto, maxSujeto);
	fprintf(stderr, "* OBJETOS: de %d a %d\n", minObjeto, maxObjeto);
	fprintf(stderr, "* PREDICADOS: de %d a %d\n", minPredicado, maxPredicado);
	fprintf(stderr,
			"*****************************************************************************************\n");

	return 0;
}

TREP * buildKTree(int nnodes, int naristas, SUJETO * grafo) {
	

   int buffer[MAXTRIPLEBUFFER*2];


	uint nodes;
	uint edges;
  	uint sujetos= MAXSUJETO;
  	uint edgesInserted=0;
	register uint i;

	nodes = nnodes;

	int _K1 = 4;
	int _K2 = 2;

	uint tamSubm = 1 << 22;
	uint part;
	part = nodes / tamSubm + 1;
	uint nodesOrig = nodes;
	nodes = tamSubm;

	int max_level1;
	int max_real_level1;
	max_real_level1 = ceil(log(nodes) / log(_K1)) - 1;
    max_level1 = 5;

	int nodes2 = 1;
	for (i = 0; i < max_real_level1 + 1; i++) {
		nodes2 *= _K1;
	}

	for (i = 0; i < max_level1; i++)
		nodes2 = ceil((double) nodes2 / _K1);


	int max_level2 = ceil(log(nodes2) / log(_K2)) - 1;

	edges = naristas;
	uint nodes_read = 0;

	uint algo;
	ulong algo2;
	int id1, id2;
	NODE * tree;
	MREP * rep;
	TREP * trep;
	int fila, columna;

	trep = createTreeRep(nodesOrig, edges, part, tamSubm, max_real_level1,
			max_level1, max_level2, _K1, _K2);
	uint edges_read = 0;
	for (fila = 0; fila < part; fila++) {
               for (columna = 0; columna < part; columna++) {
			fprintf(stderr, "(%2.2f\%)\n", (float) (fila * part + columna) * 100 / part / part);
		        uint edges_sub = 0;
	                if (fila*tamSubm> sujetos ) {
			  tree = createKTree(_K1, _K2, max_real_level1, max_level1,max_level2);
                          MREP * rep;	
			  rep = createRepresentation(tree, nodes,edges_sub);
			  insertIntoTreeRep(trep, rep, fila, columna);
			  continue;
			}
			

         	numberNodes = 0;
			numberLeaves = 0;
			numberTotalLeaves = 0;

			tree = createKTree(_K1, _K2, max_real_level1, max_level1,
					max_level2);
			nodes_read = 0;
			edges_read = 0;
	
			int it1;
			uint id1, id2;
			int k;
			for (it1 = 1; it1 <= MAXSUJETO; it1++){
				for (k = 0; k < grafo[it1].numAristas; k++) {
					id1 = it1 - 1;
					id2 = grafo[it1].objetos[k]-1;
					if ((id1 >= fila * tamSubm) && (id1 < (fila + 1) * tamSubm)
							&& (id2 >= columna * tamSubm) && (id2 < (columna
							+ 1) * tamSubm)) {
						insertNode(tree, id1 - fila * tamSubm, id2 - columna
								* tamSubm);
						edges_sub++;
						edgesInserted++;
					}
					edges_read++;
				}
			  }
                        

			MREP * rep;
			rep = createRepresentation(tree, nodes, edges_sub);
			//fprintf(stderr,"edges in this submatrix: %d\n",edges_sub);

			insertIntoTreeRep(trep, rep, fila, columna);
		}
	}
        // fprintf(stderr,"edgesInserted:%d\n",edgesInserted);
	compressInformationLeaves(trep);

	return trep;
}




K2TRIPLES * buildTrees(FILE *fdataset) {


	K2TRIPLES * k2triples = (K2TRIPLES *) malloc(sizeof(K2TRIPLES));

	identifiersStats(fdataset);
	

	SUJETO * grafo = (SUJETO *) malloc(sizeof(SUJETO) * (MAXSUJETO + 1));

	int buffer[MAXTRIPLEBUFFER * 3];

	if(	grafo==NULL) {fprintf(stdout,"error al inicializar\n");return NULL;}

	int * sujetosxobjeto = (int *) malloc(sizeof(int) * (MAXOBJETO + 1));
	int numTripletas = 0;
	int numEnteros, i = 0;

	//Tripleta actual
	int sujeto, objeto, predicado;


	for (i = 1; i <= MAXSUJETO; i++) {
		grafo[i].numAristas = 0;
		grafo[i].tamanio = 0;
	}
	for (i = 1; i <= MAXOBJETO; i++) {
		sujetosxobjeto[i] = 0;
	}

	int predicadoActual;

	k2triples->trees = (TREP **) malloc(MAXPREDICADO*sizeof(TREP*));

	for (predicadoActual = 1; predicadoActual <= MAXPREDICADO; predicadoActual++) {

		numTripletas = 0;
		for (i = 1; i <= MAXSUJETO; i++) {
			grafo[i].numAristas = 0;
		}
		for (i = 1; i <= MAXOBJETO; i++) {
			sujetosxobjeto[i] = 0;
		}
		
		fseek(fdataset, 0L, SEEK_SET);

		// Parser
		while (0 != (numEnteros = fread(buffer, sizeof(int), MAXTRIPLEBUFFER * 3, fdataset))) {
			int index = 0;
			while (index < numEnteros) {
				// Lectura del buffer
				predicado = buffer[index++];
				sujeto = buffer[index++];
				objeto = buffer[index++];
				if (predicado != predicadoActual)
					continue;

				sujetosxobjeto[objeto]++;
				int numAristas = grafo[sujeto].numAristas;

				//Reserva memoria si hiciera falta
				if (!(grafo[sujeto].tamanio)) {
					grafo[sujeto].objetos = (int *) malloc(TAM * sizeof(int));
					grafo[sujeto].tamanio = TAM;
				}
				if (numAristas == grafo[sujeto].tamanio) {
					grafo[sujeto].objetos = (int *) realloc(grafo[sujeto].objetos,
							(grafo[sujeto].tamanio) * 2 * sizeof(int));
					grafo[sujeto].tamanio *= 2;
				}

				//Add arista
				grafo[sujeto].objetos[grafo[sujeto].numAristas] = objeto;
				grafo[sujeto].numAristas++;

				numTripletas++;
			}
		}


		int numColumnas = MAXOBJETO;
		if (numColumnas < MAXSUJETO) {
			numColumnas = MAXSUJETO;
		}

		TREP * trep = buildKTree(numColumnas, numTripletas, grafo);
		printf("%d %d %d\n", trep->part, trep->numberOfNodes, trep->numberOfEdges);
		k2triples->trees[predicadoActual-1] = trep;
	}
	k2triples->npreds = MAXPREDICADO;
	k2triples->nso = 0;
	return k2triples;

}




K2TRIPLES * createK2TRIPLES(FILE * inputFile){
	K2TRIPLES * triples = buildTrees(inputFile);
	triples->indiceSP = crearINDICEDAC(inputFile, PORSUJETO, triples->npreds);
	triples->indiceOP = crearINDICEDAC(inputFile, POROBJETO, triples->npreds);
	return triples;
}



