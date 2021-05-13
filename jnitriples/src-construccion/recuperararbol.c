#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"



int main(int argc, char* argv[]){

	if(argc<2){
		fprintf(stderr,"USAGE: %s <GRAPH>\n",argv[0]);
		return(-1);
	}

	char *filename = (char *)malloc(sizeof(char)*(strlen(argv[1])+4));
	TREP * rep = loadTreeRepresentation(argv[1]);
	
  strcpy(filename,argv[1]);
  strcat(filename,".rb");
	FILE *fr = fopen(filename,"w");
	
	fwrite(&(rep->numberOfNodes),sizeof(uint),1,fr);
  	
	fwrite(&(rep->numberOfEdges),sizeof(uint),1,fr);

	uint * listady, node, i, j, edge;
//	for(i=0;i<10000;i++){
	int contadorAristas=0;
	for(i=0;i<rep->numberOfNodes;i++){
  		listady = compactTreeAdjacencyList(rep, i);
  		node = -(i+1);
		contadorAristas+=listady[0];
 		for(j=0;j<listady[0];j++){
  			edge = listady[j+1] + 1;
 	  		fwrite(&node,sizeof(uint),1,fr);
  			fwrite(&edge,sizeof(uint),1,fr);
  		}
  	}
	fprintf(stderr,"Aristas\t%d\t%d\t%d\n",contadorAristas,rep->numberOfEdges,rep->numberOfEdges-contadorAristas);
  fclose(fr);
  
 // destroyTreeRepresentation(rep);
  free(filename);
  
  return 0;
}


