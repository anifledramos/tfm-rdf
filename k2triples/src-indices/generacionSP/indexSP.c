#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define TAMBUFFER 1024
#define TAM_INICIAL 100

typedef struct sujeto {
	int * predicados;
	int numPredicados;
	int slots;
	int sujeto;
	int valorSujeto;
	int identificadorDAC;
} SSUJETO;



typedef struct bloque{
	int * predicados;
	int frecuencia;
	int numPredicados;
}BLOQUEP;



int compare(const void * A,const void * B){

	return compararSujetos ((SSUJETO *) A, (SSUJETO *) B);
}


int cmpInt(const void * A,const void * B){

	BLOQUEP * bloqueA=(BLOQUEP *) A;
	BLOQUEP * bloqueB=(BLOQUEP *) B;
	if (bloqueA->frecuencia<bloqueB->frecuencia)
	return -1;
	if (bloqueA->frecuencia>bloqueB->frecuencia) 
	return 1;
	return 0;
}

int cmpSujeto(const void * A,const void * B){

	SSUJETO * bloqueA=(SSUJETO *) A;
	SSUJETO * bloqueB=(SSUJETO *) B;
	if (bloqueA->valorSujeto<bloqueB->valorSujeto)
	return -1;
	if (bloqueA->valorSujeto>bloqueB->valorSujeto) 
	return 1;
	return 0;
}


int compararSujetos(SSUJETO * A,SSUJETO * B){
  int cont=0;

  while(1){
    if (A->numPredicados==cont)
	if (B->numPredicados==cont)
 	  return 0;
	else return -1;
    if (B->numPredicados==cont){
	return 1;
    }
    if (A->predicados[cont]>B->predicados[cont])
	return 1;
    if (A->predicados[cont]<B->predicados[cont])
	return -1;
    cont++;
  }
}


int search(SSUJETO * sinfos, int sujeto, int predicado) {
	int i;
	for (i = 0; i < sinfos[sujeto].numPredicados; i++) {
		if (sinfos[sujeto].predicados[i] == predicado)
			return 1;
	}
	return 0;
}

int printInfo(SSUJETO ssujeto){
	int i;
	fprintf(stderr,"[%d]:",ssujeto.sujeto);
	for (i=0;i<ssujeto.numPredicados;i++){
		fprintf(stderr,"%d-",ssujeto.predicados[i]);
	}
	fprintf(stderr,"\n");
	return 0;

}

int insert (SSUJETO * sinfos, int sujeto, int predicado) {	
	if (sinfos[sujeto].slots == sinfos[sujeto].numPredicados) {
		sinfos[sujeto].slots *= 2;
		sinfos[sujeto].predicados = realloc(sinfos[sujeto].predicados,
				sinfos[sujeto].slots*sizeof(int));
	}
	sinfos[sujeto].predicados[sinfos[sujeto].numPredicados] = predicado;
	sinfos[sujeto].numPredicados++;
	return 0;
}


int main(int argc, char ** argv) {

	FILE * f,* f2,*f3,*f4,*f5;
	int tam;
	int * buffer = (int *) malloc(sizeof(int) * 3 * TAMBUFFER);
	int numTripletas;
	int i,j;

	if (argc < 3) {
		fprintf(stderr, "%s <inputFile> <outFile> \n", argv[0]);
		return 1;
	}
	
	if ((f = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	}

	int maximoSujeto=0;
	while ((tam = fread(buffer, sizeof(int), 3 * TAMBUFFER, f)) > 0) {
		numTripletas = tam /3;
		for (i = 0; i < numTripletas; i++) {
			int sujeto = buffer[3 * i+1];
			if (sujeto>maximoSujeto) maximoSujeto=sujeto;
		}
		
	}
 	int NUMSUJETOS=maximoSujeto;
	
	if ((f = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero");
		return 1;
	}


	if ((f2 = fopen(argv[2],"w+"))==NULL){
		fprintf(stderr,"ERROR de apertura del fichero de escritura:%s\n",argv[2]);
		return 1;
	}
	char cadena[1000];
	strcpy(cadena, argv[2]);
	strcat(cadena,".dicc");
	
	if ((f3 = fopen(cadena,"w+"))==NULL){
		fprintf(stderr,"ERROR de apertura del fichero de escritura:%s\n",cadena);
		return 1;
	}
	strcpy(cadena,argv[2]);
	strcat(cadena,".ind");
	
	if ((f4 = fopen(cadena,"w+"))==NULL){
		fprintf(stderr,"ERROR de apertura del fichero de escritura:%s\n",cadena);
		return 1;
	}


	strcpy(cadena,argv[2]);
	strcat(cadena,".info");
	if ((f5 = fopen(cadena,"w+"))==NULL){
		fprintf(stderr,"ERROR de apertura del fichero de escritura:%s\n",cadena);
		return 1;
	}
	
	SSUJETO * sujetosInfo = (SSUJETO *) malloc(sizeof(SSUJETO) * NUMSUJETOS);

	for (i=0;i<=NUMSUJETOS;i++){
		sujetosInfo[i].sujeto=i;
		sujetosInfo[i].slots=TAM_INICIAL;
		sujetosInfo[i].numPredicados=0;
		sujetosInfo[i].valorSujeto=i;
		sujetosInfo[i].predicados=(int *) malloc(sizeof(int)*TAM_INICIAL);
	}
	
	int contador=0;

	while ((tam = fread(buffer, sizeof(int), 3 * TAMBUFFER, f)) > 0) {
		numTripletas = tam /3;
		for (i = 0; i < numTripletas; i++) {
		int j = 0;
			int sujeto = buffer[3 * i + 1];
			int predicado = buffer[3 * i];
			if (!search(sujetosInfo, sujeto, predicado))
				insert(sujetosInfo, sujeto, predicado);
			contador++;
		}
		
	}
	
	qsort(&(sujetosInfo[1]),NUMSUJETOS,sizeof(SSUJETO),compare);

	BLOQUEP * predicados=(BLOQUEP *) malloc(sizeof(BLOQUEP) * NUMSUJETOS);
	
	int indexPredicados=0;
	
	for (i=0;i<NUMSUJETOS;i++){
	  predicados[i].frecuencia=0;
	}
	
	SSUJETO * sujetoAnterior=&(sujetosInfo[1]);
	sujetosInfo[1].identificadorDAC=indexPredicados;
	int frecuenciaAnterior=1;
	for (i=2;i<=NUMSUJETOS;i++){

          
	if (compararSujetos(&(sujetosInfo[i]),sujetoAnterior)){
		predicados[indexPredicados].frecuencia=frecuenciaAnterior;
		predicados[indexPredicados].predicados=sujetoAnterior->predicados;
		predicados[indexPredicados].numPredicados=sujetoAnterior->numPredicados;

		sujetoAnterior=&(sujetosInfo[i]);

		frecuenciaAnterior=1;
		indexPredicados++;
		sujetosInfo[i].identificadorDAC=indexPredicados;

	  }

	  else {
		sujetosInfo[i].identificadorDAC=indexPredicados;
	  	frecuenciaAnterior++;
          }


	}


        predicados[indexPredicados].frecuencia=frecuenciaAnterior;
	predicados[indexPredicados].predicados=sujetoAnterior->predicados;
	predicados[indexPredicados].numPredicados=sujetoAnterior->numPredicados;
	indexPredicados++;


	for (i=0;i<indexPredicados;i++){
		fwrite(predicados[i].predicados,predicados[i].numPredicados,sizeof(int),f3);
		fwrite(&(predicados[i].numPredicados),1,sizeof(int),f4);
	}

	qsort(&(sujetosInfo[1]),NUMSUJETOS,sizeof(SSUJETO),cmpSujeto);
	

	for (i=1;i<=NUMSUJETOS;i++){
		fwrite(&(sujetosInfo[i].identificadorDAC),sizeof(int),1,f2);
	}
	

	fwrite(&NUMSUJETOS,1,sizeof(int),f5);
	fwrite(&indexPredicados,1,sizeof(int),f5);


	fclose(f2);
	fclose(f3);
	fclose(f4);
	fclose(f5);

}

