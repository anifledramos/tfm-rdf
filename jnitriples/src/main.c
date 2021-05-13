#include  	   <stdio.h>
#include  	   <stdlib.h>
#define NUMSUJETOS 18425128
#include           "directcodes.h"
#define NUMRISTRAS 787033

int main(int argc, char ** argv){

	FILE * f,*f2,*f3,*f4;

	uint i,j;

	if (argc < 5) {
		fprintf(stderr, "%s <inputFile> <outFile> <indexFile> <diccFile>\n", argv[0]);
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
  	
	
	uint * sujetos= (uint *) malloc(sizeof(uint)*(NUMSUJETOS+1));

	uint * index=(uint *) malloc(sizeof(int)*(NUMRISTRAS+1));

	fread(&(sujetos[1]),NUMSUJETOS,sizeof(uint),f);
	
	fread(&(index[1]),NUMRISTRAS,sizeof(uint),f3);

	int count=0;
	index[0]=0;
	fprintf(stderr,"INDEX[1]:%d\n",index[1]);
	for (i=1;i<=NUMRISTRAS;i++){
		count+=index[i];
		index[i]=count;
	
	}


	uint * predicados=(uint *) malloc(sizeof(uint)*(count));
	index[NUMRISTRAS+1]=count;
	fread(predicados,count,sizeof(uint),f4);
		
	FTRep * sujetoDAC=createFT(&(sujetos[1]),NUMSUJETOS); 
	
	
	for (i=1;i<10;i++){
		uint code=accessFT(sujetoDAC,i);	
		fprintf(stderr,"%d-->%d(",i,code);
		int primero=index[code];
		int ultimo=index[code+1];

		for (j=primero;j<ultimo;j++){

			fprintf(stderr,"%d-",predicados[j]);

		}

		fprintf(stderr,")\n");
	}	

	saveFT(sujetoDAC,f2);

	return 0;

}















