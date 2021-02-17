#include "commons.h"

/* Time meassuring */
double ticks;
double total_secsUser,total_secsSystem;
struct tms t1,t2;

void startClock() {
	times (&t1);
}

void stopClock() {
	times (&t2);
	total_secsUser=(float)(t2.tms_utime-t1.tms_utime)/ (float)ticks;
	total_secsSystem=(float)(t2.tms_stime-t1.tms_stime)/(float)ticks;
}



int obtenerPredicados(INDICEDAC dac, int indice) {
	int code = accessFTI(dac.dac, indice+1);
	//code++;
	int i=0;
	int first=GetField(dac.indicePredicados,dac.bitsIndex,code);
	
	resDAC.numeroPredicados = GetField(dac.indicePredicados,dac.bitsIndex,code+1)
			- first;
	for (i=0;i<resDAC.numeroPredicados;i++){
		resDAC.predicados[i]=GetField(dac.predicados,dac.bitsPred,first+i);
	}
	return 0;
}

int obtenerPredicados2(INDICEDAC dac, int indice) {
	int code = accessFTI(dac.dac, indice + 1);
	int i=0;

	int first=GetField(dac.indicePredicados,dac.bitsIndex,code);

	resDAC2.numeroPredicados = GetField(dac.indicePredicados,dac.bitsIndex,code+1)
			- first;

	for (i=0;i<resDAC2.numeroPredicados;i++){
		resDAC2.predicados[i]=GetField(dac.predicados,dac.bitsPred,first+i);
	}
	return 0;
}
/*
INDICEDAC * cargarDAC(char * baseFich, int numPredicados) {

	int i;

	FILE * fdicc, *find, *fdac, *finfo;


	char cadena[200];
	strcpy(cadena, baseFich);
	strcat(cadena, ".dicc");
	if ((fdicc = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return NULL;
	}

	strcpy(cadena, baseFich);
	strcat(cadena, ".ind");

	if ((find = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return NULL;
	}

	strcpy(cadena, baseFich);
	strcat(cadena, ".dacc");

	if ((fdac = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return NULL;
	}

	strcpy(cadena, baseFich);
	strcat(cadena, ".info");

	if ((finfo = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return NULL;
	}


	int NUMRISTRAS, NUMSUJETOS;

	
	if (1!=fread(&NUMSUJETOS, sizeof(int),1, finfo)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}
	if (1!=fread(&NUMRISTRAS, sizeof(int),1, finfo)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}

	uint * index = (uint *) malloc(sizeof(int) * (NUMRISTRAS + 2));

	if (NUMRISTRAS!=fread(&(index[1]), sizeof(uint), NUMRISTRAS,find)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}


	int count = 0;
	//index[0] = 0;
	for (i = 1; i <= NUMRISTRAS; i++) {
		//if (index[i]>1) {fprintf(stderr,"mas de un pred:%d-count:%d-index[i]:%d\n",i,count,index[i]);}
		count += index[i];
	//	index[i] = count;
	}
	uint * predicados = (uint *) malloc(sizeof(uint) * (count));
	if (count!=fread(predicados, sizeof(uint),count, fdicc)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}

	FTRepI *sDAC = loadFTI(fdac);
	
	INDICEDAC * indiceSP = crearIndice(NUMRISTRAS, NUMSUJETOS, numPredicados,
	index, predicados, sDAC, 0);
	
	
	fclose(fdicc);
	fclose(find);
	fclose(fdac);
	fclose(finfo); 
	return indiceSP;
}


void guardarDAC(INDICEDAC * dac, char * basefilename) {
	char cadena[200];
	sprintf(cadena, "%s.info", basefilename);
	FILE * finfo = fopen(cadena, "w+");
	fwrite(&dac->NUMSUJETOS, sizeof(int), 1, finfo);
	fwrite(&dac->NUMRISTRAS, sizeof(int), 1, finfo);
	fclose(finfo);
	sprintf(cadena, "%s.ind", basefilename);
	FILE * find = fopen(cadena, "w+");
	if (dac->indexOrig) {
		fwrite(&(dac->indexOrig[1]), sizeof(uint), dac->NUMRISTRAS, find);
	}
	fclose(find);
	sprintf(cadena, "%s.dicc", basefilename);
	FILE * fdicc = fopen(cadena, "w+");
	if (dac->predicadosOrig) {
		fwrite(dac->predicadosOrig, sizeof(uint), dac->count, fdicc);
	}
	fclose(fdicc);
	sprintf(cadena, "%s.dacc", basefilename);
	FILE * fdacc = fopen(cadena, "w+");
	saveFTI(dac->dac, fdacc);
	fclose(fdacc);
	return;
}
*/	

INDICEDAC * cargarDAC(FILE * f, int numPredicados) {

	int i;

	FILE * fdicc, *find, *fdac, *finfo;


	int NUMRISTRAS, NUMSUJETOS;

	finfo = f;
	
	if (1!=fread(&NUMSUJETOS, sizeof(int),1, finfo)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}
	if (1!=fread(&NUMRISTRAS, sizeof(int),1, finfo)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}

	uint * index = (uint *) malloc(sizeof(int) * (NUMRISTRAS + 2));

	find = f;
	if (NUMRISTRAS!=fread(&(index[1]), sizeof(uint), NUMRISTRAS,find)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}


	int count = 0;
	//index[0] = 0;
	for (i = 1; i <= NUMRISTRAS; i++) {
		//if (index[i]>1) {fprintf(stderr,"mas de un pred:%d-count:%d-index[i]:%d\n",i,count,index[i]);}
		count += index[i];
	//	index[i] = count;
	}
	fdicc = f;
	uint * predicados = (uint *) malloc(sizeof(uint) * (count));
	if (count!=fread(predicados, sizeof(uint),count, fdicc)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}

	fdac = f;
	FTRepI *sDAC = loadFTI(fdac);
	
	INDICEDAC * indiceSP = crearIndice(NUMRISTRAS, NUMSUJETOS, numPredicados,
	index, predicados, sDAC, 0);
	
	
	return indiceSP;
}


void guardarDAC(INDICEDAC * dac, FILE * f) {
	FILE * finfo = f;
	fwrite(&dac->NUMSUJETOS, sizeof(int), 1, finfo);
	fwrite(&dac->NUMRISTRAS, sizeof(int), 1, finfo);
	FILE * find = f;
	if (dac->indexOrig) {
		fwrite(&(dac->indexOrig[1]), sizeof(uint), dac->NUMRISTRAS, find);
	}
	FILE * fdicc = f;
	if (dac->predicadosOrig) {
		fwrite(dac->predicadosOrig, sizeof(uint), dac->count, fdicc);
	}
	FILE * fdacc = f;
	saveFTI(dac->dac, fdacc);
	return;
}



void inicializarEstructuras(int numPreds, int numSO) {
	ticks=(double)sysconf(_SC_CLK_TCK);
	numeroSO=numSO;
	vectores = (Result2 **) mymalloc2(numPreds * sizeof(Result2 *));
	vectores2 = (Result2 **) mymalloc2(numPreds * sizeof(Result2 *));

	infoTOTAL[0] = (uint *) mymalloc2(sizeof(uint) * NUM_ELEMENTOS*2);
	infoTOTAL[1] = (uint *) mymalloc2(sizeof(uint) * NUM_ELEMENTOS*2);

	element = (uint *) mymalloc2(sizeof(uint) * MAX_INFO);
	basex = (uint *) mymalloc2(sizeof(uint) * MAX_INFO);
	basey = (uint *) mymalloc2(sizeof(uint) * MAX_INFO);
	indicesRep = (int *) mymalloc2(sizeof(int) * MAX_INFO);
	esFilas = (int *) mymalloc2(sizeof(int) * MAX_INFO);
	basep1 = (uint *) mymalloc2(sizeof(uint) * MAX_INFO);
	baseq1 = (uint *) mymalloc2(sizeof(uint) * MAX_INFO);

	validos = (int *) mymalloc2(sizeof(int) * NUM_ELEMENTOS*2);
	realValues = (int *) mymalloc2(sizeof(int) * NUM_ELEMENTOS*2);
	validosFilas = (int *) mymalloc2(sizeof(int) * NUM_ELEMENTOS*2);
	validosFilas2 = (int *) mymalloc2(sizeof(int) * NUM_ELEMENTOS*2);
	validos2 = (int * )mymalloc2(sizeof(int) * NUM_ELEMENTOS*2);
	realFilas = (int *) mymalloc2(sizeof(int) * NUM_ELEMENTOS*2);
	resDAC.predicados=(int *)mymalloc2(sizeof(int)*numPreds);
	resDAC2.predicados=(int *)mymalloc2(sizeof(int)*numPreds);

	paresBuff1 = (SET *) mymalloc2(sizeof(SET) * NUM_ELEMENTOS);
	paresBuff2 = (SET *) mymalloc2(sizeof(SET)* NUM_ELEMENTOS);

	foutput=NULL;
}

void liberarMerge() {
	int i;
	for (i = 1; i <= vectores[0][0].value; i++)
		free(vectores[0][i].sources);
	free(vectores[0]);
}
void liberarMerge2() {
	int i;
	for (i = 1; i <= vectores2[0][0].value; i++)
		free(vectores2[0][i].sources);
	free(vectores2[0]);
}

void printTime(){
	fprintf(stdout,"usuario:%fms\n",total_secsUser*1000);
	fprintf(stdout,"sistema:%fms\n",total_secsSystem*1000);
}





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


int searchAux(SSUJETO * sinfos, int sujeto, int predicado) {
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


#define TAMBUFFER 1024
#define TAM_INICIAL 100

#define OFFSET_SP 1
#define OFFSET_OP 2

INDICEDAC * crearINDICEDAC(FILE * file, int tipo, int numPredicados) {

	int tam;
	int * buffer = (int *) malloc(sizeof(int) * 3 * TAMBUFFER);
	int numTripletas;
	int i,j;

	int OFFSET = -1;
	if (tipo == PORSUJETO) {
		OFFSET = OFFSET_SP;
	}
	if (tipo == POROBJETO) {
		OFFSET = OFFSET_OP;
	}

	fseek(file, 0, SEEK_SET);
	int maximoSujeto=0;
	while ((tam = fread(buffer, sizeof(int), 3 * TAMBUFFER, file)) > 0) {
		numTripletas = tam /3;
		for (i = 0; i < numTripletas; i++) {
			int sujeto = buffer[3 * i+OFFSET];
			if (sujeto>maximoSujeto) maximoSujeto=sujeto;
		}
		
	}
 	int NUMSUJETOS=maximoSujeto;
	
	SSUJETO * sujetosInfo = (SSUJETO *) malloc(sizeof(SSUJETO) * (NUMSUJETOS+1));

	for (i=0;i<=NUMSUJETOS;i++){
		sujetosInfo[i].sujeto=i;
		sujetosInfo[i].slots=TAM_INICIAL;
		sujetosInfo[i].numPredicados=0;
		sujetosInfo[i].valorSujeto=i;
		sujetosInfo[i].predicados=(int *) malloc(sizeof(int)*TAM_INICIAL);
	}
	
	int contador=0;

	fseek(file, 0L, SEEK_SET);
	while ((tam = fread(buffer, sizeof(int), 3 * TAMBUFFER, file)) > 0) {
		numTripletas = tam /3;
		for (i = 0; i < numTripletas; i++) {
		int j = 0;
			int sujeto = buffer[3 * i + OFFSET];
			int predicado = buffer[3 * i];
			if (!searchAux(sujetosInfo, sujeto, predicado))
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

	qsort(&(sujetosInfo[1]),NUMSUJETOS,sizeof(SSUJETO),cmpSujeto);

	int NUMRISTRAS = indexPredicados;
	uint * sujetos = (uint *) malloc (sizeof(uint) * (NUMSUJETOS + 1));
	uint * index = (uint *) malloc(sizeof(int)*(NUMRISTRAS + 2));

	for (i=1;i<=NUMSUJETOS;i++){
		sujetos[i] = sujetosInfo[i].identificadorDAC;
	}
	for (i = 1; i <= NUMRISTRAS; i++) {
		index[i] = predicados[i-1].numPredicados;
	}
	int count = 0;
	//index[0] = 0;
	for (i = 1 ; i <= NUMRISTRAS; i++)  {
		count += index[i];
//		index[i] = count;
	}

	
	uint * predicadosNEW = (uint *) malloc(sizeof(uint) * count);
	//index[NUMRISTRAS+1]=count;
	int pos = 0;
	for (i = 0; i < NUMRISTRAS; i++) {
		for (j = 0; j < predicados[i].numPredicados; j++) {
			predicadosNEW[pos] = predicados[i].predicados[j];
			pos++;
		}
	}
	printf("SUJ: ");
	FILE * oFile = fopen("mysujetos", "w+");
	printf ("pos=%d, count = %d\n", pos, count);
    FTRepI * sujetoDAC=createFTI(&(sujetos[1]),NUMSUJETOS);

    saveFTI(sujetoDAC,oFile);	
	fclose(oFile);
	
	printf("%d %d %d\n", NUMSUJETOS, NUMRISTRAS, numPredicados);
	printf("INDEX: ");
	for (i = 1; i <= NUMRISTRAS; i++) {
		printf("%d ", index[i]);
	}
	printf("\n");
	
	printf("DICC:" );
	for (i = 0; i < count; i++) {
		printf("%d ", predicadosNEW[i]);
	}
	printf("\n");

	INDICEDAC * indice = crearIndice(NUMRISTRAS, NUMSUJETOS, numPredicados, index, predicadosNEW, sujetoDAC, 1);	
}


























INDICEDAC * crearIndice(int NUMRISTRAS, int NUMSUJETOS, int numPredicados,
	int * index, uint * predicados, FTRepI * sDAC, int readWrite) {

	INDICEDAC *  indiceSP=mymalloc(sizeof(INDICEDAC));
	int i;

	if (readWrite) {
		indiceSP->NUMRISTRAS = NUMRISTRAS;
		indiceSP->NUMSUJETOS = NUMSUJETOS;
		indiceSP->indexOrig = (int *) malloc((NUMRISTRAS+2)*sizeof(int));
		for (i = 0; i < NUMRISTRAS+1; i++) {
			indiceSP->indexOrig[i] = index[i];
		}
		indiceSP->predicadosOrig = predicados;
	} else {
		indiceSP->indexOrig = NULL;
		indiceSP->predicadosOrig = NULL;
	}
    int count = 0;
    index[0] = 0;
    for (i = 1; i <= NUMRISTRAS; i++) {
        //if (index[i]>1) {fprintf(stderr,"mas de un pred:%d-count:%d-index[i]:%d\n",i,count,index[i]);}
        count += index[i];
        index[i] = count;
    }
	index[NUMRISTRAS+1] = count;
	/* COMPACTANDO INDICE DE PREDICADOS */


	// Numero de bits necesarios
	double numBitsD=log(count+1)/log(2);
	int numBits=((int)ceil(numBitsD));

	//Reserva de memoria para el vector
	//Número de bytes necesarios para el vector completo (+1 por el redondeo)
	int numNeededBytes=(numBits*(NUMRISTRAS+2)/8)+1;
	uint * compactIndex= (uint *) mymalloc(numNeededBytes);
	//fprintf(stderr,"indice diccionario:%d\n",numNeededBytes);
	//Creación vector
	//ojooooooooo cambiado el 1 por el 0
	printf("%d bytes, %d numBits, %d maxindex\n", numNeededBytes, numBits, NUMRISTRAS);
	for (i=0;i<=NUMRISTRAS;i++){
		
		SetField(compactIndex,numBits,i,index[i]);
	}
	//fprintf(stderr,"Compactando indice de predicados: %d bytes -> %d bytes\n",sizeof(uint)*(NUMRISTRAS+2),numNeededBytes);
	//Liberación memoria anterior
	if (!readWrite) {
		free(index);
	}
	//Linkado con la estructura DAC
	indiceSP->indicePredicados=compactIndex;
	indiceSP->bitsIndex=numBits;
	/*************************************/


	/* COMPACTANDO PREDICADOS */
	// Numero de bits necesarios
	numBitsD=log(numPredicados+1)/log(2);
	numBits=((int)ceil(numBitsD));

	//Reserva de memoria para el vector
	//Número de bytes necesarios para el vector completo (+1 por el redondeo)
	numNeededBytes=(numBits*(count)/8)+1;
	numNeededBytes+=(sizeof(uint)-(numNeededBytes%sizeof(uint)));
	
	//TODO: pq +1?
	uint * compactPredicados= (uint *) mymalloc(numNeededBytes+1);
	//fprintf(stderr,"diccionario:%d bytes\n",numNeededBytes+1);
	//Creación vector
	//TODO: CAMBIADO <= POR <
	for (i=0;i<count;i++){
		//int x=predicados[i];
		//fprintf(stderr,"numBIts:%d-i:%d-predicado:%d\n",numBits,i,predicados[i]);
		SetField(compactPredicados,numBits,i,predicados[i]);
	}

//	fprintf(stderr,"Compactando indice de predicados: %d bytes -> %d bytes\n",sizeof(uint)*(count),numNeededBytes);
	//Liberación memoria anterior
	if (!readWrite) {
		free(predicados);
	}
	//Linkado con la estructura DAC
	indiceSP->predicados=compactPredicados;
	indiceSP->bitsPred=numBits;
	/*************************************/
//	long antes=getMemoria();
	//FTRepI * sDAC = loadFTI(fdac);
	//long despues=getMemoria();
	//fprintf(stderr,"DAC: %ld bytes\n",despues-antes);
	//indiceSP.indicePredicados = index;
	indiceSP->dac = sDAC;
	indiceSP->count = count;

	return indiceSP;
}
