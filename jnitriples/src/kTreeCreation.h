/*
 * kTreeCreation.h
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#ifndef KTREECREATION_H_
#define KTREECREATION_H_
#include "kTreeAux.h"
#include "directcodes.h"
/*------------------------------------------------------------------
 Initilizes the structures used.
 ---------------------------------------------------------------- */
void initialize(unsigned int sizeVoc);
NODE * createKTree(int _K1, int _K2, int maxreallevel1, int maxlevel1,
		int maxlevel2);
MREP * loadRepresentation(char * basename);
void destroyRepresentation(MREP * rep);
MREP * createRepresentation(NODE * root, uint numberOfNodes, uint numberOfEdges);
void insertNode(NODE * root, int x, int y);
TREP * createTreeRep(uint nodesOrig, uint edges, uint part, uint subm,
		uint max_real_level1, uint max_level1, uint max_level2, uint _K1,
		uint _K2);
void insertIntoTreeRep(TREP * trep, MREP * rep, uint i, uint j);
//void saveTreeRep(TREP * trep, char * basename);
//TREP * loadTreeRepresentation(char * basename);
void saveTreeRep(TREP * trep, FILE * file);
TREP * loadTreeRepresentation(FILE * file);
void destroyTreeRepresentation(TREP *trep);
void compressInformationLeaves(TREP * trep);
#endif /* KTREECREATION_H_ */
