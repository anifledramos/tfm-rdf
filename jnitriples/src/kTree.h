#ifndef KTREE_H
#define KTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directcodes.h"
#include "hash.h"
#include "commons.h"
#include "bitrankw32int.h"
#include "basic.h"
#include "mymalloc.h"
#include "kTreeAux.h"
#include "kTreeCreation.h"

//VECINOS DIRECTOS
int compactTreeAdjacencyList(TREP * trep, int x, uint * resultados);

int compactTreeInverseList(TREP * trep, int x, uint * resultados);

//CHECK CELDA
uint   compactTreeCheckLinkQuery(TREP * trep, uint p, uint q);

//RANGO
int  compactTreeRangeQuery(TREP * trep, int ordenar,SET * pares);

//VECINOS PARA MERGE
uint compactTreeAdjacencyListMerge(TREP * trep, int x,
		Result2 ** vectorResultados, int indicePredicado, int predicado);

uint  compactTreeInverseListMerge(TREP * trep, int x,
		Result2 ** vectorResultados, int indicePredicado, int predicado);



#endif
