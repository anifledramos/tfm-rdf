#include <stdio.h>
#include "mymalloc.h"

unsigned long numBytes;
unsigned long numBytes2;

long inicializaMemoria(){
	numBytes=0;
	numBytes2=0;
	return 0;}

void * mymalloc(size_t size){
	numBytes+=size;
	void * mem=malloc(size);
	if (mem==NULL){
		fprintf(stderr,"Memoria insuficiente: %lu (1)- %lu\n",numBytes,numBytes2);
		exit(1);
	}
	return mem;
}

void * mymalloc2(size_t size){
	numBytes2+=size;
	void * mem=malloc(size);
        if (mem==NULL){
                fprintf(stderr,"Memoria insuficiente:%lu (2)\n",numBytes2);
                exit(1);
        }
	return mem;
}

long getMemoria(){
	return numBytes;
}

long getMemoria2(){
	return numBytes2;
}

