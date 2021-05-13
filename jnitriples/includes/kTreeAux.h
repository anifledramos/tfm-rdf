/*
 * kTreeAux.h
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#ifndef KTREEAUX_H_
#define KTREEAUX_H_

//#include <basic.h>
//#include <mymalloc.h>
//#include <bitrankw32int.h>
#include <directcodes.h>
//#include <commons.h>
//#include <hash.h>
//
///* MANEJO DE LISTA DE RESULTADOS TEMPORALES */
//#define getPQ(p) (p%(MAX_INFO))
//
///*#define addPQ(p,a) ((p+a)%MAX_INFO)
//
//#define deletePQ(p,b) (p>b?p-b:p+MAX_INFO-b)
//
//#define PQ++(p) ((p+1-1)%MAX_INFO)
//
//#define PQ--(p) (p?p-1:MAX_INFO-1)
//*/
//
//#define TAMINICIAL 256
//#define K1 4
//#define K2 2
//
//typedef struct {
//	unsigned int* sources;
//	unsigned int value;
//} Result;
//
//typedef struct SET{
//	int x;
//	int y;
//	int z;
//	int w;
//} SET;
//
//#define L 2
//#define K2_2 8
//#define K2_3 64
//#define K2_3_char 8
//
//uint * infoTOTAL[2];
//uint * infoTOTAL2[2];
//
//SET * paresBuff1;
//SET * paresBuff2;
//
//
//uint * basep1;
//uint * basep2;
//uint * baseq1;
//uint * baseq2;
//uint * element;
//uint * basex;
//uint * basey;
//int * indicesRep;
//int * realValues;
//int * validos;
//int * validos2;
//int * validosFilas;
//int * validosFilas2;
//int * esFilas;
//int * realFilas;
//
//#define MAX 100
//#ifndef uint
//#define uint unsigned int
//#endif
//
//uint numberNodes;
//uint numberLeaves;
//uint numberTotalLeaves;
//int max_real_level1, max_Level1, max_Level2;
//
//typedef struct node {
//	char * data;
//	struct node** child;
//} NODE;
//
//#define swap( x, y ) { int temp; temp=*(x); *(x)=*(y); *(y)=temp; }
//
//typedef struct QUEUE {
//	NODE *element;
//	struct QUEUE *link;
//	int basex;
//	int basey;
//} QUEUE;/*for implementing BFS*/
//
//typedef struct QUEUE2 {
//	int element;
//	struct QUEUE2 *link;
//	int basey;
//	int basex;
//} QUEUE2;/*for implementing BFS*/
//
//typedef struct QUEUECONS {
//	NODE *element;
//	struct QUEUECONS *link;
//
//} QUEUECONS;/*for implementing BFS*/
//
//QUEUE * finalQUEUE;
//QUEUE2 * finalQUEUE2;
//QUEUECONS * finalQUEUECONS;
//
//uint *div_level_table;
//
//unsigned int * positionInTH;
//unsigned int addrInTH;
//unsigned int zeroNode;

typedef struct matrixRep {
	bitRankW32Int * bt; //Bitmap representando el arbol
	bitRankW32Int * bn; //Bitmap representando las hojas con informacion
	uint bt_len; //Numero de bits de BT
	uint bn_len; //Numero de bits de BN
	uint nleaves; //Numero de hojas con informacion
	uint cutBt;uint lastBt1_len;uint numberOfNodes;uint numberOfEdges;uint * leavesInf; //Informacion de las hojas
	FTRep* compressIL;
} MREP;

typedef struct treeRep {
	uint part;uint tamSubm;uint numberOfNodes;uint numberOfEdges;uint repK1;uint repK2;

	uint maxRealLevel1; //for divlevel
	uint maxLevel1; //Nivel maximo del arbol ref K1
	uint maxLevel2; //Nivel maximo del arbol ref K2

	uint * div_level_table1;uint * div_level_table2;

	uint fila;uint columna;

	int iniq;
	int finq;

	MREP *** submatrices;

	unsigned char * words; //Palabras del vocabulario de hojas ordenadas por frecuencia
	uint lenWords; //tama�o de las palabras del vocabulario (en bytes)
	uint zeroNode; //Numero de palabras diferentes del vocabulario de hojas

} TREP;

//int hasAnyBitSet(char * b);
//
//QUEUE * AddItem(QUEUE * listpointer, NODE * elem, int cantx, int canty);
////void AddItemSimple(TREP *trep, NODE * elem);
//void AddItem2(TREP *trep, int elem, int cantx, int canty);
//QUEUE * RemoveItem(QUEUE * listpointer);
//void ClearQueue(QUEUE * listpointer);
//void AddItemJoin(TREP *trep, int elem, int cantx, int canty, int indice);
//void AddItemJoin2(TREP *trep, int elem, int cantx, int canty, int indice,
//		int esFila);
//void RemoveItem2(TREP * trep);
//QUEUECONS * AddItemCONS(QUEUECONS * listpointer, NODE * elem);
//QUEUECONS * RemoveItemCONS(QUEUECONS * listpointer);
//uint exp_pow(uint base, uint pow);
//
//void RemoveItem3(TREP * trep);
//int comparaFrecListaDesc(const void *a, const void *b);
//int checkLimites(TREP * trep);

#endif /* KTREEAUX_H_ */
