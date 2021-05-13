#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "k2triples-builder.h"
#include "k2triples-ops.h"

int main(int argc, char ** argv) {

	if (argc != 3) {
		printf("Usage: %d <input> <output>\n");
		exit(-1);
	}
	FILE * f = fopen(argv[1], "r");
	K2TRIPLES * k2triples = createK2TRIPLES(f);
	fclose(f);
	FILE * out = fopen(argv[2], "w+");	
	storeK2TRIPLES(k2triples, out);
	fclose(out);

//	destroyK2TRIPLES(k2triples);
	exit(0);
	
}
