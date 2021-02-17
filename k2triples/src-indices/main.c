#include  	   <stdio.h>
#include  	   <stdlib.h>
#include           "directcodes.h"

int NUMRISTRAS;
int NUMSUJETOS;


int main(int argc, char ** argv){


	FILE * f,*f2,*f3,*f4,*f5, *f6;

	uint i,j;

	if (argc < 7) {
		fprintf(stderr, "%s <inputFile> <outFile> <indexFile> <diccFile> <log> <infoFile> \n", argv[0]);
		return 1;
	}

	if ((f = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	} 
  	
	if ((f2 = fopen(argv[2], "w+")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	}
	if ((f3 = fopen(argv[3], "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	} 
  	
	if ((f4 = fopen(argv[4], "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	} 

	if ((f5 = fopen(argv[5], "w+")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	} 

	if ((f6 = fopen(argv[6], "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	} 

	fread(&NUMSUJETOS,1,sizeof(int),f6);
	fread(&NUMRISTRAS,1,sizeof(int),f6);

	uint * sujetos= (uint *) malloc(sizeof(uint)*(NUMSUJETOS+1));

	uint * index=(uint *) malloc(sizeof(int)*(NUMRISTRAS+1));

	fread(&(sujetos[1]),NUMSUJETOS,sizeof(uint),f);
	
	fread(&(index[1]),NUMRISTRAS,sizeof(uint),f3);

	int count=0;
	index[0]=0;
	
	for (i=1;i<=NUMRISTRAS;i++){
		count+=index[i];
		index[i]=count;
	}


	uint * predicados=(uint *) malloc(sizeof(uint)*(count));
	index[NUMRISTRAS+1]=count;
	fread(predicados,count,sizeof(uint),f4);
		
	FTRep * sujetoDAC=createFT(&(sujetos[1]),NUMSUJETOS); 
	
	for (i=1;i<=NUMSUJETOS;i++){
		
		uint code=accessFT(sujetoDAC,i);	
		int primero=index[code];
	
		int ultimo=index[code+1];
		for (j=primero;j<ultimo;j++){
			fprintf(f5,"%d\t%d\n",i,predicados[j]);
		}
	}	

	saveFT(sujetoDAC,f2);

	return 0;

}















