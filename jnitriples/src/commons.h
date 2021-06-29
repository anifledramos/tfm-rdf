#ifndef ___COMMONS
#define ___COMMONS

//i->version para indices SP y OP
//sin i->version para k2-trees
#include "kTreeAux.h"
#include "mymalloc.h"
#include "directcodesI.h"
#include "merge.h"
#include "basic.h"
#include <time.h>
#define DEBUG 0
#define DEBUG2 0
//#define MAX_INFO 4294967295
#define MAX_INFO 1024*102*5*2
#define PORSUJETO 1
#define POROBJETO 2
//#define NUM_ELEMENTOS 1024*1024*60
#define NUM_ELEMENTOS 1024*102*5*2

typedef struct indiceDAC {
	uint * predicados;
	uint bitsPred;
	uint * indicePredicados;
	uint bitsIndex;
	FTRepI * dac;
    //Para read-write
    int NUMRISTRAS;
	int NUMSUJETOS;
	int count;
	int * indexOrig;
	uint * predicadosOrig;
} INDICEDAC;

typedef struct resultado {
	int * predicados;
	int numeroPredicados;
} RESULTADODAC;

Result2 ** vectores;

Result2 ** vectores2;

RESULTADODAC resDAC;

RESULTADODAC resDAC2;

FILE * foutput;

int numeroSO;

int obtenerPredicados(INDICEDAC dac, int indice);

int obtenerPredicados2(INDICEDAC dac, int indice);

INDICEDAC * crearINDICEDAC(FILE * file, int tipo, int numPredicados);


//INDICEDAC * cargarDAC(char * baseFich, int numPredicados);
INDICEDAC * crearIndice(int NUMRISTRAS, int NUMSUJETOS, int numPredicados, int * index, uint * predicados, FTRepI * dac, int readWrite);

//void guardarDAC(INDICEDAC * dac, char * baseFich);
void guardarDAC(INDICEDAC * dac, FILE * f);
INDICEDAC * cargarDAC(FILE * f, int numPredicados);

void inicializarEstructuras(int numPreds, int numSO);

void liberarMerge();

void liberarMerge2();

void startClock();

void stopClock();

void printTime();

#endif
