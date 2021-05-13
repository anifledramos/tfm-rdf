#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define TAMBUFFER 1024

int main(int argc, char ** argv) {

	FILE * f,* f2;
	int tam;
	int * buffer = (int *) malloc(sizeof(int) * 3 * TAMBUFFER);
	int numTripletas;
	int i;

	if (argc < 3) {
		fprintf(stderr, "%s <inputFile> <outFile>\n", argv[0]);
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

	int contador=0;
	while ((tam = fread(buffer, sizeof(int), 3 * TAMBUFFER, f)) > 0) {
		numTripletas = tam /3;
		for (i = 0; i < numTripletas; i++) {
			int sujeto = buffer[3 * i + 2];
			int predicado = buffer[3 * i];
			fprintf(f2,"%d\t%d\n",sujeto,predicado);
		}
	}
	
	fclose(f);
}

