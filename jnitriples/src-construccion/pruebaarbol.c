#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#define MAXTRIPLEBUFFER 200




int main(int argc, char* argv[]) {

  int buffer[MAXTRIPLEBUFFER*2];


	FILE *f;
	uint nodes;
	uint edges;
  	uint sujetos= 38065699;
  	uint edgesInserted=0;
	register uint i;

	if (argc < 6) {
		fprintf(stderr,
				"USAGE: %s <GRAPH> <node> <name> <K1> <K2> [<max level K1>]\n",
				argv[0]);
		return (-1);
	}
	
  	f = fopen(argv[1], "r");
	fread(&nodes, sizeof(uint), 1, f);

	int _K1 = atoi(argv[4]);
	int _K2 = atoi(argv[5]);

	uint tamSubm = 1 << 22;
	uint part;
	part = nodes / tamSubm + 1;
	uint nodesOrig = nodes;
	nodes = tamSubm;

	int max_level1;
	int max_real_level1;
	max_real_level1 = ceil(log(nodes) / log(_K1)) - 1;
	if (argc < 7)
		max_level1 = ceil(log(nodes) / log(_K1)) - 1;
	else
		max_level1 = atoi(argv[6]);

	int nodes2 = 1;
	for (i = 0; i < max_real_level1 + 1; i++) {
		nodes2 *= _K1;
	}

	for (i = 0; i < max_level1; i++)
		nodes2 = ceil((double) nodes2 / _K1);


	int max_level2 = ceil(log(nodes2) / log(_K2)) - 1;

////        fread(&sujetos,sizeof(uint),1,f);
	fread(&edges, sizeof(uint), 1, f);
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
	fclose(f);
	uint edges_read = 0;
	for (fila = 0; fila < part; fila++) {
               for (columna = 0; columna < part; columna++) {
			// fprintf(stderr, "(%2.2f\%)\n", (float) (fila * part + columna) * 100 / part / part);
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

			f = fopen(argv[1], "r");

			fread(&algo, sizeof(uint), 1, f);
			//fread(&algo, sizeof(uint), 1, f);
			fread(&algo, sizeof(uint), 1, f);

			tree = createKTree(_K1, _K2, max_real_level1, max_level1,
					max_level2);
			nodes_read = 0;
			edges_read = 0;
			int numEnteros,k;
	
	                while (0!=(numEnteros=fread(buffer,sizeof(int),MAXTRIPLEBUFFER*2,f))){
                          int index=0;
	                  //if (!(edgesInserted%100000)) fprintf(stderr,"%dedges\n",edgesInserted);
        		  
	                  while (index<numEnteros){
				
				k=buffer[index++];

				if (k < 0) {
					nodes_read = (-1)*k;
				} else {
					k--;
					id1 = nodes_read - 1;
					id2 = k;
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
                        }
			fclose(f);

			MREP * rep;
			rep = createRepresentation(tree, nodes, edges_sub);
			//fprintf(stderr,"edges in this submatrix: %d\n",edges_sub);

			insertIntoTreeRep(trep, rep, fila, columna);
		}
	}
        fprintf(stderr,"(%s)edgesInserted:%d\n",argv[1],edgesInserted);
	compressInformationLeaves(trep);

	saveTreeRep(trep, argv[3]);

//	destroyTreeRepresentation(trep);
	return 0;
}

