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

INDICEDAC * cargarDAC(char * baseFich, int numPredicados) {

	INDICEDAC *  indiceSP=mymalloc(sizeof(INDICEDAC));
	int i;

	FILE * fdicc, *find, *fdac, *finfo;


	char cadena[200];
	strcpy(cadena, baseFich);
	strcat(cadena, ".dicc");
	if ((fdicc = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return indiceSP;
	}

	strcpy(cadena, baseFich);
	strcat(cadena, ".ind");

	if ((find = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return indiceSP;
	}

	strcpy(cadena, baseFich);
	strcat(cadena, ".dacc");

	if ((fdac = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return indiceSP;
	}

	strcpy(cadena, baseFich);
	strcat(cadena, ".info");

	if ((finfo = fopen(cadena, "r")) == NULL) {
		fprintf(stderr, "ERROR de apertura de fichero %s\n", cadena);
		return indiceSP;
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
	index[0] = 0;
	for (i = 1; i <= NUMRISTRAS; i++) {
		//if (index[i]>1) {fprintf(stderr,"mas de un pred:%d-count:%d-index[i]:%d\n",i,count,index[i]);}
		count += index[i];
		index[i] = count;
	}
	uint * predicados = (uint *) malloc(sizeof(uint) * (count));
	if (count!=fread(predicados, sizeof(uint),count, fdicc)){
		fprintf(stderr,"cargarDAC:error de lectura\n");return NULL;
	}



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
	for (i=0;i<=NUMRISTRAS;i++){
		
		SetField(compactIndex,numBits,i,index[i]);
	}
	//fprintf(stderr,"Compactando indice de predicados: %d bytes -> %d bytes\n",sizeof(uint)*(NUMRISTRAS+2),numNeededBytes);
	//Liberación memoria anterior
	free(index);
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
	free(predicados);
	//Linkado con la estructura DAC
	indiceSP->predicados=compactPredicados;
	indiceSP->bitsPred=numBits;
	/*************************************/
	long antes=getMemoria();
	FTRepI * sDAC = loadFTI(fdac);
	long despues=getMemoria();
	//fprintf(stderr,"DAC: %ld bytes\n",despues-antes);
	//indiceSP.indicePredicados = index;
	indiceSP->dac = sDAC;

	return indiceSP;
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
