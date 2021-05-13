#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "directcodesI.h"
#include "commons.h"
#include "k2triples-ops.h"

/*
void storeK2TRIPLES(K2TRIPLES *k2triples, char * filename){
	char * fn = (char *) malloc(1024 * sizeof(char));
	sprintf(fn, "%s", filename);
	FILE * f = fopen(fn, "w+");
	fwrite(&k2triples->npreds, sizeof(uint), 1, f);
	fwrite(&k2triples->nso, sizeof(uint), 1, f);
	fclose(f);
	int i;
	for (i = 0; i < k2triples->npreds; i++) {
		sprintf(fn, "%s-%d", filename, i);
		printf("Saving %d\n", i);
		saveTreeRep(k2triples->trees[i], fn);
	}
	sprintf(fn, "%s-sp", filename);
	guardarDAC(k2triples->indiceSP, fn);
	sprintf(fn, "%s-op", filename);
	guardarDAC(k2triples->indiceOP, fn);
	free(fn);
}

K2TRIPLES * loadK2TRIPLES(char * filename){
	char * fn = (char *) malloc(1024 * sizeof(char));
	sprintf(fn, "%s", filename);
	FILE * f = fopen(fn, "r");
	K2TRIPLES * k2triples = (K2TRIPLES *) malloc(sizeof(K2TRIPLES));
	fread(&k2triples->npreds, sizeof(uint), 1, f);
	fread(&k2triples->nso, sizeof(uint), 1, f);
	fclose(f);
	k2triples->trees = (TREP **) malloc(k2triples->npreds * sizeof(TREP*));
	int i;
	for (i = 0; i < k2triples->npreds; i++) {
		sprintf(fn, "%s-%d", filename, i);
		k2triples->trees[i] = loadTreeRepresentation(fn);
	}

	sprintf(fn, "%s-sp", filename);
	k2triples->indiceSP = cargarDAC(fn, k2triples->npreds);
	sprintf(fn, "%s-op", filename);
	k2triples->indiceOP = cargarDAC(fn, k2triples->npreds);
	free(fn);
	return k2triples;
}
*/


void storeK2TRIPLES(K2TRIPLES *k2triples, FILE * f){
	fwrite(&k2triples->npreds, sizeof(uint), 1, f);
	fwrite(&k2triples->nso, sizeof(uint), 1, f);
	int i;
	for (i = 0; i < k2triples->npreds; i++) {
		saveTreeRep(k2triples->trees[i], f);
	}
	guardarDAC(k2triples->indiceSP, f);
	guardarDAC(k2triples->indiceOP, f);
}

K2TRIPLES * loadK2TRIPLES(FILE * f){
	K2TRIPLES * k2triples = (K2TRIPLES *) malloc(sizeof(K2TRIPLES));
	fread(&k2triples->npreds, sizeof(uint), 1, f);
	fread(&k2triples->nso, sizeof(uint), 1, f);
	k2triples->trees = (TREP **) malloc(k2triples->npreds * sizeof(TREP*));
	int i;
	for (i = 0; i < k2triples->npreds; i++) {
		k2triples->trees[i] = loadTreeRepresentation(f);
	}

	k2triples->indiceSP = cargarDAC(f, k2triples->npreds);
	k2triples->indiceOP = cargarDAC(f, k2triples->npreds);
	return k2triples;
}


void destroyK2TRIPLES(K2TRIPLES *k2triples){
	int i;
	for (i = 0 ;i < k2triples->npreds; i++) {
		destroyTreeRepresentation(k2triples->trees[i]);
	}
	
	//TODO : DACS
	free(k2triples->trees);
	free(k2triples);
}

size_t sizeK2TRIPLES(K2TRIPLES *k2triples){
	return 0;
	//TODO
}



