#ifndef ___SIMPLEPATTERNS
#define ___SIMPLEPATTERNS

#include "kTree.h"
#include "commons.h"


int spoF(FILE * f,TREP ** treps,int nPreds,int s,int p,int o);

int soF(FILE * f,TREP ** treps, int nPreds, int s,int o);

int soIndexF(FILE * f,TREP ** treps, INDICEDAC dac, int s,int o);

int soDobleIndexF(FILE * f,TREP ** treps, INDICEDAC dac,INDICEDAC dac2, int s, int o);
	
int spF(FILE * f,TREP ** treps,int nPreds, int s,int p);

int poF(FILE * f,TREP ** treps, int nPreds,int p,int o);

int sF(FILE * f,TREP ** treps,int nPreds, int s);

int sIndexF(FILE * f,TREP ** treps,INDICEDAC dac, int s);

int pF(FILE * f,TREP ** treps,int nPreds, int p);

int oF(FILE * f,TREP ** treps,int nPreds,int o);

int oIndexF(FILE * f,TREP ** treps,INDICEDAC dac,int o);

int pOrdenadoF(FILE * f,TREP ** treps, int nPreds, int p);
	
int propagarSPF(FILE * f,TREP ** treps, INDICEDAC dac, int s,int p);	
#endif
